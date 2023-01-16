
#define MOTOR_DIR 4
#define MOTOR_PWM 5

/////////////////// Steering Servo Control ///////////////////
#include <Servo.h>
#define RC_SERVO_PIN 6
#define NEURAL_ANGLE 90
#define LEFT_STEER_ANGLE -30
#define RIGHT_STEER_ANGLE 30

Servo Sreeringservo;
int Steering_Angle = NEURAL_ANGLE;
/////////////////// Steering Servo Control ///////////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);

  Sreeringservo.attach(RC_SERVO_PIN);
  Sreeringservo.write(NEURAL_ANGLE);
}

void steering_control(int steer_angle)
{
  Sreeringservo.write(NEURAL_ANGLE + steer_angle);
}

void motor_control(int direction, int speed)
{
  digitalWrite(MOTOR_DIR, direction);
  analogWrite(MOTOR_PWM, speed);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Motor Control");
  int i;
  for (i=0; i<255; i++) // 속도최대 255
  {
    motor_control(1,i);
    delay(10);
  }
  
  steering_control(LEFT_STEER_ANGLE);
  delay(1000);
  steering_control(RIGHT_STEER_ANGLE);
  delay(1000);
}
