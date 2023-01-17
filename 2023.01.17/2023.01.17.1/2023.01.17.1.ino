
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

/////////////////// Line Sensor ///////////////////
#define LINE_DETECT_WHITE 1 // 하얀 바탕에 하얀 빛 감지하면 1
int LineSensorPin[5] = {7,8,9,10,11};
int LineSensorData[5] = {0, 0, 0, 0, 0};
/////////////////// Line Sensor ///////////////////
void setup() 
{
  Serial.begin(115200);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);

  Sreeringservo.attach(RC_SERVO_PIN);
  Sreeringservo.write(NEURAL_ANGLE);

  int i;
  for(i=0; i<5; i++)
  {
     pinMode(LineSensorPin[i], INPUT);
  }
}

void read_digital_line_sensor(void)
{
  for(i=0; i<5; i++)
  {
    if(LINE_DETECT_WHITE == 0) // 검정일때
    {
      LineSensorData[i] == 1 - digitalRead(LineSensorPin[i]);
    }
    else
    {
      LineSensorData[i] == digitalRead(LineSensorPin[i]);
    }
  }
}

int line_detection_type()
{
  int i;
  int l_type = 0; // -5 ~ 5
  int sum = 0;
  
  for(i=0, sum = 0; i<5; i++)
  {
    sum += LineSensorData[i];
  }
  if(sum == 1)
  {
    if(LineSensorData[0] == 1)
      l_type = -4;
    if(LineSensorData[1] == 1)
      l_type = -2;
    if(LineSensorData[2] == 1)
      l_type = -0;
    if(LineSensorData[3] == 1)
      l_type = -2;
    if(LineSensorData[4] == 1)
      l_type = -4;
  }

  if(sum == 2)
  {
    if(LineSensorData[0] == 1 && LineSensorData[1] == 1)
      l_type = -3;
    if(LineSensorData[1] == 1 && LineSensorData[1] == 1)
      l_type = -1;
    if(LineSensorData[2] == 1 && LineSensorData[1] == 1)
      l_type = 1;
    if(LineSensorData[3] == 1 && LineSensorData[1] == 1)
      l_type = 3;
  }

  if(sum == 2)
  {
    l_type == 10;
  }
  return l_type;
}
void send_line_sensor_data(void)
{
  int i;
  for(i=0; i<5; i++)
  {
    Serial.print(LineSensorData[i]);
    Serial.print(" ");
  }
  Serial.println("line_detection_type());
  //Serial.("\n");
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

void loop() 
{
  //Serial.println("Motor Control");
  int i;

  read_digital_line_sensor();
  send_line_sensor_data();
  delay(500);
  
  /*
  for (i=0; i<255; i++) // 속도최대 255
  {
    motor_control(1,i);
    delay(10);
  }
  
  steering_control(LEFT_STEER_ANGLE);
  delay(1000);
  steering_control(RIGHT_STEER_ANGLE);
  delay(1000);
  */
}
