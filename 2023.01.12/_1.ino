#include <IRremote.h>

#define IN1 2
#define IN2 4
#define ENL 9

#define IN3 7
#define IN4 6
#define ENR 5

IRrecv ir_recv(10); // 적외선 센서에 연결된 핀번호
decode_results results; // 수신된 적외선 신호를 저장할 변수

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENL, OUTPUT);
  
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENR, OUTPUT);
  ir_recv.enableIRIn(); // 적외선 센서 활성화
  Serial.begin(9600);
}
void motor_control(int dir_l, int speed_l, int dir_r, int speed_r)
{
  if (dir_l == 1)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENL, speed_l);
  }
  else if (dir_l == -1)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENL, speed_l);
  }
  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENL, 0);
  }
  
  if (dir_r == 1)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENR, speed_r);
  }
  else if (dir_r == -1)
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENR, speed_r);
  }
  else
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENR, 0);
  }
}

void loop()
{
  if (ir_recv.decode(&results))
  {
    Serial.println(results.value);
    delay(30);
    ir_recv.resume(); // 다음 신호를 받을 수 있도록 초기화
    
    if (results.value == 16615543)
    {
      motor_control(1, 100, 1, 100); // 방향, 속도 * 2
    }

  }
  
  else if (results.value == 16623703)
  {
    motor_control(0, 100, 0, 100); // 방향, 속도 * 2
  }
  
  else if (results.value == 16619623)
  {
    motor_control(-1, 100, -1, 100); // 방향, 속도 * 2
  }
  
  else if (results.value == 16591063)
  {
    motor_control(-1, 100, 1, 100); // 방향, 속도 * 2
  }
  
  else if (results.value == 16607383)
  {
    motor_control(1, 100, -1, 100); // 방향, 속도 * 2
  }
  else
  {
    motor_control(0, 100, 0, 100); // 방향, 속도 * 2
  }
}