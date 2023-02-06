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


void read_sonar_sensor(void) //초음파센서 측정
{
    R_Sonar_distance = R_sensor.ping_cm()*10.0;
    L_Sonar_distance = L_sensor.ping_cm()*10.0;
    F_Sonar_distance = F_sensor.ping_cm()*10.0;
    
    if(L_Sonar_distance == 0)
    {
      L_Sonar_distance = MaxDistance * 20;
    }

    if(F_Sonar_distance == 0)
    {
      F_Sonar_distance =  MaxDistance * 20;
    }

    if(R_Sonar_distance == 0)
    {
      R_Sonar_distance =  MaxDistance * 20;
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(L_InPin_2, OUTPUT);
  pinMode(L_InPin_1, OUTPUT);
  pinMode(R_InPin_2, OUTPUT);
  pinMode(R_InPin_1, OUTPUT);
  read_sonar_sensor();
}
/////////////////////////////////////////////////////////

int read_line_sensor(void) // 라인센서 읽기
{
 int i;
 int sum=0;
 int line_index = 0;
 int line_sensor[5]={0,0,0,0,0};
 
 for(i=0;i<5;i++)
 {
   line_sensor[i]=(digitalRead(A0+i));
   sum += line_sensor[i];
 }
 if(sum==0) line_index = 100;
 if(sum==5) line_index = -10;
 if(sum==1)
 {
   if(line_sensor[0]==1) line_index = -4;
   if(line_sensor[1]==1) line_index = -2;
   if(line_sensor[2]==1) line_index = 10;
   if(line_sensor[3]==1) line_index = 2;
   if(line_sensor[4]==1) line_index = 4;
 }
 if(sum==2)
 {
   if(line_sensor[0]==1 && line_sensor[1]==1) line_index = -3;
   if(line_sensor[1]==1 && line_sensor[2]==1) line_index = -1;
   if(line_sensor[2]==1 && line_sensor[3]==1) line_index = 1;
   if(line_sensor[3]==1 && line_sensor[4]==1) line_index = 3;
 }
 if(sum==3 || sum==4)
 {
   if(line_sensor[0]==0) line_index = 5;
   if(line_sensor[4]==0) line_index = -5;
 }
 //Serial.print(line_index);
 //Serial.println(" ");
 //delay(1000);

 return line_index;
}

void keep_line_going(void) // 라인에 따른 주행
{
  int re = read_line_sensor();
  switch(re)
  {
    case -5 : motor_control_left(1,200); break;
    case -4 : motor_control_left(1,200); break;
    case -3 : motor_control(1,0,100*2); break;
    case -2 : motor_control(1,21*2,100*2); break;     
    case -1 : motor_control(1,55*2,100*2); break;
    case 10 : motor_control(1,84*2,112*2); break;
    case 100 : motor_control(1,84*2,100*2); break;
    case 1 : motor_control(1,84*2,65*2); break;
    case 2 : motor_control(1,84*2,25*2); break;
    case 3 : motor_control(1,84*2,0); break;
    case 4 : motor_control_right(1,200); break;     
    case 5 : motor_control_right(1,200); break;
    case -10 : Stop(1); break;
  }

  if(re != 100)
  {
    if(F_Sonar_distance < 300)
    {
      if(F_Sonar_distance > 0)
      {
        Stop(1);
      }
      
    }
  }
}

///////////////////////////////////////////////////////////

void keep_sensor_going(void) // 초음파 거리에 따른 주행
{
  int re = read_line_sensor();
  
  if(re == 100)
  {

    if(F_Sonar_distance <= 950 && F_Sonar_distance >= 500 && R_Sonar_distance > 1000 ) // 미로 우회전
    {
      motor_control(1,84*2,10*2);
      delay(1800);
      motor_control(1,84*2,100*2);
      delay(1000);
    }

    else
    {
      if(R_Sonar_distance + L_Sonar_distance <= 1170 && R_Sonar_distance - 70 <= L_Sonar_distance && R_Sonar_distance + 70 >= L_Sonar_distance) // 좌 우 거리 같을때 진행
      {
        motor_control(1,84*2,100*2);
      
      }
    
      else if( L_Sonar_distance < R_Sonar_distance) // 우회전
      {
        motor_control(1,84*2,70*2);
        if(L_Sonar_distance < 200) // 비상용 우회전
        {
         motor_control(1,84*2,0);
        }  
      }

      else if( L_Sonar_distance > R_Sonar_distance ) // 좌회전
      {
        motor_control(1,57*2,110*2);
      
        if(R_Sonar_distance < 200) // 비상용 좌회전
        {
          motor_control(1,0,100*2);
        }   
      }

      else
      {
        motor_control(1,90*2,100*2);
      }
    }
    /*
    if(F_Sonar_distance <= 700 && R_Sonar_distance > 900) // 미로 우회전
    {
      motor_control(1,60*2,10*2);
      delay(2000);
    }
    */

    
  } 
}

///////////////////////////////////////////////////////////
void motor_control_left(int Time, int Speed)
{
    digitalWrite(L_InPin_2,HIGH);
    digitalWrite(L_InPin_1,LOW);
    digitalWrite(R_InPin_2,HIGH);
    digitalWrite(R_InPin_1,LOW);
    analogWrite(L_en,Speed);
    analogWrite(R_en,Speed);
    delay(Time);
}

void motor_control_right(int Time, int Speed)
{
    digitalWrite(L_InPin_1,HIGH);
    digitalWrite(L_InPin_2,LOW);
    digitalWrite(R_InPin_1,HIGH);
    digitalWrite(R_InPin_2,LOW);
    analogWrite(L_en,Speed);
    analogWrite(R_en,Speed);
    delay(Time);
}

void Stop(int Time)
{
  digitalWrite(L_InPin_1,LOW);
  digitalWrite(L_InPin_2,LOW);
  digitalWrite(R_InPin_2,LOW);
  digitalWrite(R_InPin_1,LOW);
  delay(Time);
}
  
void motor_control(int Direction, int Speed1, int Speed2)
{
  switch(Direction){
  case -1: // 후진
    digitalWrite(L_InPin_2,HIGH);
    digitalWrite(L_InPin_1,LOW);
    digitalWrite(R_InPin_2,HIGH);
    digitalWrite(R_InPin_1,LOW);
    analogWrite(L_en, Speed1);
    analogWrite(R_en, Speed2);
  break;
  
  case 1: // 직진 // 기종 바꾸고 우회전 되서 변경
    digitalWrite(L_InPin_1,HIGH);
    digitalWrite(L_InPin_2,LOW);
    digitalWrite(R_InPin_2,HIGH);
    digitalWrite(R_InPin_1,LOW);
    analogWrite(L_en, Speed1);
    analogWrite(R_en, Speed2);
  break;

  case 0: // 정지
    digitalWrite(L_InPin_1,HIGH);
    digitalWrite(L_InPin_2,LOW);
    digitalWrite(R_InPin_1,HIGH);
    digitalWrite(R_InPin_2,LOW);
    analogWrite(L_en, Speed1);
    analogWrite(R_en, Speed2);
  break;

  case 2: // 우회전
    digitalWrite(L_InPin_1,HIGH); 
    digitalWrite(L_InPin_2,LOW);
    digitalWrite(R_InPin_1,LOW);
    digitalWrite(R_InPin_2,HIGH);
    analogWrite(L_en, Speed1);
    analogWrite(R_en, Speed2);
  break;

  case -2: // 좌회전
    digitalWrite(L_InPin_1,LOW);
    digitalWrite(L_InPin_2,HIGH);
    digitalWrite(R_InPin_1,HIGH);
    digitalWrite(R_InPin_2,LOW);
    analogWrite(L_en, Speed1);
    analogWrite(R_en, Speed2);
  break;
  }
}
///////////////////////////////////////////////////////////

void serial_print(void)
{
  Serial.print(L_sensor.ping_cm());
  Serial.print("  ");
  Serial.print(F_sensor.ping_cm());
  Serial.print("  ");
  Serial.println(R_sensor.ping_cm());
  Serial.print(L_Sonar_distance);
  Serial.print("  ");
  Serial.print(F_Sonar_distance);
  Serial.print("  ");
  Serial.println(R_Sonar_distance);
  
}
void loop() 
{
  read_sonar_sensor();
  keep_line_going();
  keep_sensor_going();
  serial_print();
}
