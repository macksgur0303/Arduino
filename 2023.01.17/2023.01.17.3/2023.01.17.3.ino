/////////////////// Ultrasonic Sensor ///////////////////
#include <NewPing.h>

#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 150 // Maximum distance (in cm) to ping.

float UltarsonicSensorData[SONAR_NUM];

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(2, 3, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(42, 43, MAX_DISTANCE),
  NewPing(46, 47, MAX_DISTANCE)
};

void read_ultarsonic_sensor(void)
{
  UltarsonicSensorData[0] = sonar[0].ping_cm();
  UltarsonicSensorData[1] = sonar[1].ping_cm();
  UltarsonicSensorData[2] = sonar[2].ping_cm();
}
/////////////////// Ultrasonic Sensor ///////////////////

/////////////////// I2C LCD Display ///////////////////
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/////////////////// I2C LCD Display ///////////////////

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

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Mr.Moon");

  delay(2000);
  lcd.clear();
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

void send_sonar_sensor_data(void)
{
  int i;
  for(i=0; i<SONAR_NUM; i++)
  {
    Serial.print(UltarsonicSensorData[i]);
    Serial.print(" ");
  }
    Serial.println(" ");
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
void display_lcd(void)
{
  int i;
  for(i=0; i<5; i++)
  {
    lcd.setCursor(i,0);
    lcd.print(LineSensorData[i]);
  }

  lcd.setCursor(7,0);
  lcd.print("      ");
  lcd.setCursor(7,0);
  lcd.print(line_index);

  lcd.setCursor(0,1);
  lcd.print("      ");
  lcd.setCursor(0,1);
  lcd.print((int)UltarsonicSensorData[0]);

  lcd.setCursor(6,1);
  lcd.print("      ");
  lcd.setCursor(6,1);
  lcd.print((int)UltarsonicSensorData[1]);

  lcd.setCursor(12,1);
  lcd.print("      ");
  lcd.setCursor(12,1);
  lcd.print((int)UltarsonicSensorData[2]);
  
  lcd.setCursor(7,1);
  lcd.print("      ");
  lcd.setCursor(7,1);
  lcd.print(Motor_Speed);
}
void loop() 
{
  //Serial.println("Motor Control");
  int i;

  read_digital_line_sensor();
  //send_line_sensor_data();
  read_ultarsonic_sensor();
  //send_sonar_sensor_data();
  display_lcd();
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
