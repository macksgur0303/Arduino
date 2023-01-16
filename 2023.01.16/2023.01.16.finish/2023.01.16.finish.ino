 #define L_InPin_2 2  //왼쪽모터 핀
 #define L_InPin_1 3  //왼쪽모터 핀2
 #define R_InPin_2 7  //오른쪽모터 핀1
 #define R_InPin_1 4  //오른쪽모터 핀2

 #define L_en 5 //왼쪽모터 속도 제어 핀
 #define R_en 6 //오른쪽모터 속도 제어 핀
  
void setup() {
  Serial.begin(115200);
  pinMode(L_InPin_2, OUTPUT);
  pinMode(L_InPin_1, OUTPUT);
  pinMode(R_InPin_2, OUTPUT);
  pinMode(R_InPin_1, OUTPUT);
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
  Serial.println("Motor Control");
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
  
}
