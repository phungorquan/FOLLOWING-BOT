/**

   @todo
    - move strings to flash (less RAM consumption)
    - fix deprecated convertation form string to char* startAsAnchor
    - give example description
*/
#include <SPI.h>
#include "DW1000Ranging.h"
#include <DW1000.h>
#include "DW1000Device.h"
// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 3; // irq pin
const uint8_t PIN_SS = SS; // spi select pin
int count = 0;
void setup() {
  Serial.begin(115200);
  delay(800);
  //DW1000.begin(PIN_IRQ, PIN_RST);
  // DW1000.select(PIN_SS);


  //init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin


  //define the sketch as anchor. It will be great to dynamically change the type of module
  DW1000Ranging.attachNewRange(newRange);
  //DW1000Ranging.attachBlinkDevice(newBlink);
  //DW1000Ranging.attachInactiveDevice(inactiveDevice);
  //Enable the filter to smooth the distance
  DW1000Ranging.useRangeFilter(true);

  DW1000Ranging.startAsAnchor("B1:00:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);

}

void loop() {
  DW1000Ranging.loop();

 
}
//String convertFloatToString(float temperature)
//{ // begin function
//
//  char temp[10];
//  String tempAsString;
//
//  // perform conversion
//  dtostrf(temperature, 1, 2, temp);
//
//  // create string object
//  tempAsString = String(temp);
//
//  return tempAsString;
//
//}
void newRange() {
//  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
//  Serial.print("Range: ");
//  Serial.println(DW1000Ranging.getDistantDevice()->getRange());


  float tmp = DW1000Ranging.getDistantDevice()->getRange();
  //int tmp1;
 while(tmp <= 0)
  {
    tmp+=1;
    //tmp1 = tmp*(100);
    
  }
//  tmp1 = tmp * 100;
//
//  float a = 456.23;
//  //a = ;
  
  uint16_t tmp2 = tmp*100;
  uint16_t tmp3 = tmp2 >> 8;

  count++;
  if(count == 4)
  {
   Serial.write(tmp3);
   Serial.write(tmp2);
   //Serial.print(
   count = 0 ;
  }

 
  //Serial.println(" m");
  // Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
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
