/**
 * 
 * @todo
 *  - move strings to flash (less RAM consumption)
 *  - fix deprecated convertation form string to char* startAsAnchor
 *  - give example description
 */
#include <SPI.h>
#include "DW1000Ranging.h"

// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 3; // irq pin
const uint8_t PIN_SS = SS; // spi select pin
int count =0;
uint16_t tmp2 = 0 ;
uint16_t tmp3 = 0 ;
float tmp = 0;

void setup() {
  Serial.begin(115200);
  delay(2500);
  //init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  //define the sketch as anchor. It will be great to dynamically change the type of module
  DW1000Ranging.attachNewRange(newRange);
  //DW1000Ranging.attachBlinkDevice(newBlink);
  //DW1000Ranging.attachInactiveDevice(inactiveDevice);
  //Enable the filter to smooth the distance
  DW1000Ranging.useRangeFilter(true);
  
  //we start the module as an anchor
  DW1000Ranging.startAsAnchor("8C:17:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY,false);
}

void loop() {
  DW1000Ranging.loop();
}

void newRange() {
  //Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  //Serial.print("\t Range: "); 
  //Serial.println(DW1000Ranging.getDistantDevice()->getRange() + 1);// Serial.print(" m");
  //Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");

  tmp = DW1000Ranging.getDistantDevice()->getRange() + 0.80;
  //tmp+=1.35;
  if(tmp <= 0)
  {//tmp+=1;
    return;
    //tmp1 = tmp*(100);
  }
  //tmp1 = tmp * 100;
 
  count++;
  if(count == 2)
  {
   tmp2 = tmp*100;
   tmp3 = tmp2 >> 8;
   Serial.write(tmp3);
   Serial.write(tmp2);
   //Serial.print(
   count = 0 ;
  }


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
