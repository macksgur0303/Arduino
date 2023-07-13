#include <MsTimer2.h>

#define encoderPinA       2     // encoder A상 핀번호
#define encoderPinB       3     // encoder A상 핀번호
#define motorA            8
#define motorB            9
#define motorEN           11    // pin10 고장남
#define encoder_MAX_count 440   // encoder의 한바퀴 카운트
#define encoderPos20      370   // 20cm 당 encoder pulse 수
#define encoderPos100     1850  // 1m 당 encoder pulse 수
#define encoderDistance   0.054 // encoder pulse수 당 1m

#define Kp 4
#define Ki 0.1
#define Kd 12

long encoderPos = 0;            // 변위용 encoderPos
long encoder = 0;
long encoder_old = 0;
int  encodercount=0;            // rpm측정용 카운트(방향성X)
unsigned long previous = 0;     // 이전 rising이 발생했던 시간
unsigned long current = 0;      // 현재 rising이 발생한 시간
unsigned long period = 0;       // 주기
unsigned long time_new = 0;
unsigned long timer = 0;
unsigned long time_old = 0;
float rpm=0;

float error=0;
float error_s=0;
float error_d=0;
float error_old=0;
float pwm_output=0;
float targetEncoder = 1000;

void SPID_Control()
{
  ///// Steering /////
  error = targetEncoder - encoderPos; 
  error_s += error;
  error_d = error - error_old;
  error_s = (error_s >=  100) ?  100 : error_s;
  error_s = (error_s <= -100) ? -100 : error_s;

  pwm_output = Kp * error + Kd * error_d + Ki * error_s;
  pwm_output = (pwm_output >=  155) ?  155 : pwm_output;
  pwm_output = (pwm_output <= -155) ? -155 : pwm_output;

  
  if (fabs(error) <= 0.2)
  {
    motor_control(0);
    error_s = 0;
  }
  
  else          motor_control(pwm_output);
  
  error_old = error;
}

void motor_control(int Speed)
{
  if (Speed >= 0)
  {
    digitalWrite(motorA , HIGH);
    digitalWrite(motorB , LOW);
    analogWrite(motorEN  ,Speed);
  }

  else
  {
    digitalWrite(motorA , LOW);
    digitalWrite(motorB , HIGH);
    analogWrite(motorEN ,-Speed);
  }
}

void doEncoderInterrupt(void) //엔코더 측정용 인터럽트 루프
{  
  //encodercount++; // 이동거리 측정
    
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
  }*/
}

  void call_back(void)//rpm측정용 타이머
  {
    motor_control(100);
    //SPID_Control();
    /*time_new=millis();
    timer=time_new-time_old;
    time_old=time_new;
    
    
    rpm=(1/(float)period)*60000;*/
  }


void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(motorEN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderInterrupt,RISING); //엔코더 읽기위한 하드웨어 Interrupt(LOW->HIGH)
  MsTimer2::set(20, call_back); // 20ms
  MsTimer2::start();            //타이머 시작
  Serial.begin(115200);
}

/*함수는 초기 설정을 수행*/
void loop() { 
  Serial.print("encoderPos : ");Serial.print(encoderPos);Serial.print("  MsTimer2 : ");Serial.print(timer);Serial.print("   rpm : ");Serial.println(rpm);
  
  }
/*함수는 계속해서 실행되는 루프*/
