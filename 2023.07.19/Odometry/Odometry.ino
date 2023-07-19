#include <LSM303.h>
#include <Wire.h>
#include <MsTimer2.h>

#define encoderPinA 3
#define encoderPinB 2

LSM303 compass;

double radius = 0;

float error=0;
float error_s=0;
float error_d=0;
float error_old=0;

long encoderPos = 0;            // 변위용 encoderPos
long encoder = 0;
long encoder_old = 0;
int  encodercount=0;

unsigned long previous = 0;     // 이전 rising이 발생했던 시간
unsigned long current = 0;      // 현재 rising이 발생한 시간
unsigned long period = 0;       // 주기
unsigned long time_new = 0;
unsigned long timer = 0;
unsigned long time_old = 0;

struct OdoData{
  double x;
  double y;
  }OdoData odom_data;


void doEncoderInterrupt(void) //엔코더 측정용 인터럽트 루프
{  
  //encodercount++; // 이동거리 측정
  time_new=millis();
  timer=time_new-time_old;
    
  if(digitalRead(encoderPinB) == LOW)
  {
   //Serial.print("encoder_B : LOW");
   encoderPos++; //정방향
  }
  else
  {
   //Serial.print("encoder_B : HIGH");
   encoderPos--; //역방향
  }
  
  /*if(encodercount == encoder_MAX_count)
  {
   current = millis(); //현재 시간 측정
   period = current - previous; //이전 시간과 현재 시간차를 이용한 한바퀴 주기 측정
   previous = current; //현재 시간을 이전시간으로 저장
   encodercount=0; //인덱스 터지기 방지하기 위해 초기화
  */

  time_old=time_new;
}


void odma_calculation(void){
  angle = imu_data.orientation.z;
  radius = (encoder1data-encoder_old)/angle;
  encoder_old = encoder1data;
  
  if(angle >= 0){
    odom_data.y += radius*(1 - cos(angle));
    odom_data.x += radius*sin(angle);
    }
  else{
    odom_data.y -= radius*(1 - cos(angle));
    odom_data.x += radius*sin(angle);
    }
}


void setup()
{
  pinMode(EncoderAPin, INPUT);
  pinMode(EncoderBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(EncoderAPin), Encoder, RISING);
  MsTimer2::set(50, call_back); // 50ms
  MsTimer2::start();
  
  Wire.begin();
  compass.init();
  compass.enableDefault();

  compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
  Serial.begin(115200);
}

void loop()
{
  unsigned long current_Millis = millis();

  if (current_Millis - previous_Millis >= width)
  {
    previous_Millis = current_Millis;
  }

  Serial.print("Pulse_counter: "); 
  Serial.println(counter);
  Serial.print("Wheel_tick: "); 
  Serial.println((counter * pulse2_m));

}
