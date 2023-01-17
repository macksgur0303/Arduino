#include <NewPing.h>

#define L_InPin_2 2  //왼쪽모터 핀
#define L_InPin_1 3  //왼쪽모터 핀2
#define R_InPin_2 7  //오른쪽모터 핀1
#define R_InPin_1 4  //오른쪽모터 핀2

#define F_Sonar 8  //정면부 초음파 센서 연결 핀
#define R_Sonar 10 //오른쪽 초음파 센서 연결 핀
#define L_Sonar 11 //왼쪽 초음파 센서 연결 핀

#define L_en 5 //왼쪽모터 속도 제어 핀
#define R_en 6 //오른쪽모터 속도 제어 핀

#define MaxDistance 350

NewPing F_sensor(F_Sonar, F_Sonar, MaxDistance);
double F_Sonar_distance = 0.0;
NewPing L_sensor(L_Sonar, L_Sonar, MaxDistance);
double L_Sonar_distance = 0.0;
NewPing R_sensor(R_Sonar, R_Sonar, MaxDistance);
double R_Sonar_distance = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(L_InPin_2, OUTPUT);
  pinMode(L_InPin_1, OUTPUT);
  pinMode(R_InPin_2, OUTPUT);
  pinMode(R_InPin_1, OUTPUT);
  read_sonar_sensor();
}
void read_sonar_sensor(void) //초음파센서 측정
{
    R_Sonar_distance = R_sensor.ping_cm()*10.0;
    L_Sonar_distance = L_sensor.ping_cm()*10.0;
    F_Sonar_distance = F_sensor.ping_cm()*10.0;
}

void motor_control(int Direction, int Speed)
{
  switch(Direction){
  case -1:
    digitalWrite(L_InPin_2,HIGH);
    digitalWrite(L_InPin_1,LOW);
    digitalWrite(R_InPin_2,HIGH);
    digitalWrite(R_InPin_1,LOW);
    analogWrite(L_en, Speed);
    analogWrite(R_en, Speed);
  break;
  
  case 1:
    digitalWrite(L_InPin_1,HIGH);
    digitalWrite(L_InPin_2,LOW);
    digitalWrite(R_InPin_1,HIGH);
    digitalWrite(R_InPin_2,LOW);
    analogWrite(L_en, Speed);
    analogWrite(R_en, Speed);
  break;

  case 2:
    digitalWrite(L_InPin_1,HIGH);
    digitalWrite(L_InPin_2,LOW);
    digitalWrite(R_InPin_1,LOW);
    digitalWrite(R_InPin_2,HIGH);
    analogWrite(L_en, Speed);
    analogWrite(R_en, Speed);
  break;

  case -2:
    digitalWrite(L_InPin_1,LOW);
    digitalWrite(L_InPin_2,HIGH);
    digitalWrite(R_InPin_1,HIGH);
    digitalWrite(R_InPin_2,LOW);
    analogWrite(L_en, Speed);
    analogWrite(R_en, Speed);
  break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Motor Control");
  read_sonar_sensor();
  Serial.print(L_sensor.ping_cm());
  Serial.print("  ");
  Serial.print(F_sensor.ping_cm());
  Serial.print("  ");
  Serial.println(R_sensor.ping_cm());
  delay(1000);
/*
  int i;
  for (i=0; i<170; i++) // 속도최대 255
  {
    motor_control(2,i); //오른쪽
    delay(10);
  }


  for (i=0; i<170; i++) // 속도최대 255
  {
    motor_control(1,i); // 직진
    delay(10);
  }

  for (i=0; i<170; i++) // 속도최대 255
  {
    motor_control(-2,i); //왼쪽
    delay(10);
  }

  for (i=0; i<170; i++) // 속도최대 255
  {
    motor_control(1,i); // 직진
    delay(10);
  }
  
  for (i=0; i<170; i++) // 속도최대 255
  {
    motor_control(-1,i); // 후진
    delay(10);
  }
*/
}
