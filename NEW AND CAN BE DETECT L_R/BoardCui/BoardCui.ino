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


#define P1 7
#define P2 6
#define T1 5
#define T2 4
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 3; // irq pin
const uint8_t PIN_SS = SS; // spi select pin

const float distance = 0.17;

int count =0;
String s = "";

void setup() {

  
//  pinMode(P1,OUTPUT);
//  pinMode(P2,OUTPUT);
//  pinMode(T1,OUTPUT);
//  pinMode(T2,OUTPUT);
  
  Serial.begin(115200);
  delay(1000);

  
  //init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin

  
  //define the sketch as anchor. It will be great to dynamically change the type of module
  DW1000Ranging.attachNewRange(newRange);
  //DW1000Ranging.attachBlinkDevice(newBlink);
  //DW1000Ranging.attachInactiveDevice(inactiveDevice);
  //Enable the filter to smooth the distance
  DW1000Ranging.useRangeFilter(true);
  
  //we start the module as an anchor
   // DW1000Ranging.getDistantDevice()->setShortAddress(1);
  DW1000Ranging.startAsAnchor("BC:00:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY,false);
 
  
}

void loop() {
  DW1000Ranging.loop();
  

}

//void P_Tien(int speed) { 
//  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(P1, HIGH);// chân này không có PWM
//  analogWrite(P2, 255 - speed);
//}
//void P_Lui(int speed) { 
//  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(P1, LOW);// chân này không có PWM
//  analogWrite(P2, speed);
//}
//
//// ################################################################################################################################################
//
//void T_Tien(int speed) { 
//  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(T2, LOW);// chân này không có PWM
//  analogWrite(T1, speed);
//}
//void T_Lui(int speed) { 
//  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
//  digitalWrite(T2, HIGH);// chân này không có PWM
//  analogWrite(T1, 255 - speed);
//}

void newRange() {
  //Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  //Serial.print("Range: ");
  //Serial.print(DW1000Ranging.getDistantDevice()->getRange());
 float tmp = DW1000Ranging.getDistantDevice()->getRange();
  tmp+=0.30;
  while(tmp <= 0)
  {
    tmp+=1;
    //tmp1 = tmp*(100);
    
  }
  //tmp1 = tmp * 100;
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
  //Serial

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
