/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <events/mbed_events.h>
#include <mbed.h>
#include "DW1000.h"                                 // our DW1000 device driver
#include "MM2WayRanging.h"                          // our self developed ranging application

#include "ble/BLE.h"
#include "ble/services/iBeacon.h"

#define SERIAL_TX p11
#define SERIAL_RX p12

Serial pc(SERIAL_TX, SERIAL_RX);

static iBeacon* ibeaconPtr;

// Anchor
// MOSI 28
// MISO 27
// SCLK 26
// CS 4
// IRQ 25
// Device driver instanceSPI pins: (MOSI, MISO, SCLK, CS, IRQ)
DW1000          dw(p28, p27, p26, p4, p25);
DigitalOut wakeup(p5);

// Tag
// MOSI 7
// MISO 6
// SCLK 5
// CS 16
// IRQ 4
// Device driver instanceSPI pins: (MOSI, MISO, SCLK, CS, IRQ)
//DW1000          dw(p7, p6, p5, p16, p4);
//DigitalOut wakeup(p17);

// Instance of the two way ranging algorithm
MM2WayRanging   node(dw);

const static char     DEVICE_NAME[]        = "TEST_DWTAG";

EventQueue eventQueue(/* event count */ 4 * EVENTS_EVENT_SIZE);

static char live[] = "0";
static uint8_t ianchor = 1;
static uint8_t pianchor = 255;

void rangeAndDisplayAll(){
    if (pianchor != 255) {
        if (node.acknowledgement[pianchor]) {
            pc.printf("Distance to %d at %f: %f\r\n", pianchor, node.measuretimes[pianchor], node.distances[pianchor]);
            live[0]++;
            BLE &ble = BLE::Instance();
            ble.gap().updateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, (uint8_t *)live, sizeof(live));
        }
    }
//    pc.printf("Live: %d\r\n", live[0]);
    node.requestRanging(ianchor);
//    node.requestRanging(1);
    pianchor = ianchor;
    ianchor++;
    if (ianchor > 3) {
        ianchor = 1;
    }
  
//    uint64_t sys_time = dw.getSYSTimestamp();
//    pc.printf("System time: %llu\r\n", sys_time);
//    node.requestRangingAll();

//    for (int i = 1; i <= 1; i++) {
//        pc.printf("%f, ", node.distances[i]);
//        //pc.printf("T:%f", node.roundtriptimes[i]);
//    }
//    pc.printf("\r\n");
}

/**
 * This function is called when the ble initialization process has failled
 */
void onBleInitError(BLE &ble, ble_error_t error)
{
    /* Initialization error handling should go here */
}

/**
 * Callback triggered when the ble initialization process has finished
 */
void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if(ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    /**
     * The Beacon payload has the following composition:
     * 128-Bit / 16byte UUID = E2 0A 39 F4 73 F5 4B C4 A1 2F 17 D1 AD 07 A9 61
     * Major/Minor  = 0x1122 / 0x3344
     * Tx Power     = 0xC8 = 200, 2's compliment is 256-200 = (-56dB)
     *
     * Note: please remember to calibrate your beacons TX Power for more accurate results.
     */
    static const uint8_t uuid[] = {0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4,
                                   0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61};
    uint16_t majorNumber = 1122;
    uint16_t minorNumber = 3344;
    uint16_t txPower     = 0xC8;

    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, (uint8_t *)live, sizeof(live));

    ibeaconPtr = new iBeacon(ble, uuid, majorNumber, minorNumber, txPower);

    ble.gap().setAdvertisingInterval(100); /* 1000ms. */
    ble.gap().startAdvertising();
}

void printMacAddress()
{
    /* Print out device MAC address to the console*/
    Gap::AddressType_t addr_type;
    Gap::Address_t address;
    BLE::Instance().gap().getAddress(&addr_type, address);
    pc.printf("DEVICE MAC ADDRESS: ");
    for (int i = 5; i >= 1; i--){
        pc.printf("%02x:", address[i]);
    }
    pc.printf("%02x\r\n", address[0]);
}


void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
//    wakeup = 1;

    live[0] = 0;
    pc.baud(115200);
    pc.printf("\r\nDecaWave 1.0 up and running!\r\n");
    // basic methods called to check if we have a working SPI connection
    dw.setEUI(0xFAEDCD01FAEDCD01);                      
    pc.printf("DEVICE_ID register: 0x%X\r\n", dw.getDeviceID());
    pc.printf("EUI register: %016llX\r\n", dw.getEUI());
    pc.printf("Voltage: %fV\r\n", dw.getVoltage());
    pc.printf("Status: %d\r\n", dw.getStatus());

    node.isAnchor = false;                       
    node.address = 9;
    pc.printf("This node is a Beacon with address %d\r\n", node.address);
    
    eventQueue.call_every(1000, rangeAndDisplayAll);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);
    
    eventQueue.dispatch_forever();
    return 0;
}

