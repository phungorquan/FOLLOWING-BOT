/**
 * 
 * @todo
 *  - move strings to flash (less RAM consumption)
 *  - fix deprecated convertation form string to char* startAsAnchor
 *  - give example description
 */
#include <SPI.h>
#include "DW1000Ranging.h"
#include <DW1000.h>
#include "DW1000Device.h"
// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 3; // irq pin
const uint8_t PIN_SS = SS; // spi select pin

void setup() {
  Serial.begin(115200);
  delay(1000);
   //DW1000.begin(PIN_IRQ, PIN_RST);
 // DW1000.select(PIN_SS);
  
 
  //init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
//   Serial.println(F("DW1000 initialized ..."));
//  // general configuration
//  DW1000.newConfiguration();
//  DW1000.setDeviceAddress(5);
//  DW1000.setNetworkId(10);
//  DW1000.commitConfiguration();
//  Serial.println(F("Committed configuration ..."));
//  // wait a bit
//  delay(1000);
  
  //define the sketch as anchor. It will be great to dynamically change the type of module
 // DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachBlinkDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  //Enable the filter to smooth the distance
  DW1000Ranging.useRangeFilter(true);
  
  //we start the module as an anchor
   // DW1000Ranging.getDistantDevice()->setShortAddress(1);
  DW1000Ranging.startAsAnchor("A1:00:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY,false);

}

void loop() {
  DW1000Ranging.loop();
}

void newRange() {
  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
  Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
}

void newBlink(DW1000Device* device) {
  Serial.print("blink; 1 device added ! -> ");
  Serial.print(" short:");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}
