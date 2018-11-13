
#define P1 7
#define P2 6
#define T1 5
#define T2 4
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0

void P_Tien(int speed) { 
  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(P1, HIGH);// chân này không có PWM
  analogWrite(P2, 255 - speed);
}
void P_Lui(int speed) { 
  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(P1, LOW);// chân này không có PWM
  analogWrite(P2, speed);
}

// ################################################################################################################################################

void T_Tien(int speed) { 
  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(T2, LOW);// chân này không có PWM
  analogWrite(T1, speed);
}
void T_Lui(int speed) { 
  //speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(T2, HIGH);// chân này không có PWM
  analogWrite(T1, 255 - speed);
}

void setup()
{
  pinMode(P1,OUTPUT);
  pinMode(P2,OUTPUT);
  pinMode(T1,OUTPUT);
  pinMode(T2,OUTPUT);
}

void loop()
{
  T_Lui(200);
  P_Lui(200);
  delay(2000);
  T_Lui(100);
  P_Lui(100);
  delay(2000);
  T_Lui(0);
  P_Lui(0);
  delay(2000);
 // digitalWrite(P1,HIGH);     // 2 bánh đang chạy thẳng thì phải ,
 // digitalWrite(P2,LOW);
 // digitalWrite(T1,HIGH);
 // digitalWrite(T2,LOW);
}

// code dưới đown từ web này , vào coi xem cách cắm dây , nhớ cắm chung đất http://arduino.vn/bai-viet/893-cach-dung-module-dieu-khien-dong-co-l298n-cau-h-de-dieu-khien-dong-co-dc
// Chưa test code trên web , mới test code trên thôi 


