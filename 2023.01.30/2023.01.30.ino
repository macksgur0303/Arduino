#include <NewPing.h>
#define MaxDistance  100

#define F_Sonar 8  //정면부 초음파 센서 연결 핀
#define R_Sonar 10 //오른쪽 초음파 센서 연결 핀
#define L_Sonar 11 //왼쪽 초음파 센서 연결 핀

float old_average=0;
float new_average=0;

 void nomal_average_5data (float a,int K) // 샤랏투 정훈이형
 {
    new_average=(old_average*(K-1)/K)+(a/K);
    old_average=new_average ;
 }


NewPing F_sensor(F_Sonar, F_Sonar, MaxDistance);
double F_Sonar_distance = 0.0;
NewPing L_sensor(L_Sonar, L_Sonar, MaxDistance);
double L_Sonar_distance = 0.0;
NewPing R_sensor(R_Sonar, R_Sonar, MaxDistance);
double R_Sonar_distance = 0.0;

  
void read_sonar_sensor(void){ //초음파센서 측정
    R_Sonar_distance = R_sensor.ping_cm()*10.0;
    L_Sonar_distance = L_sensor.ping_cm()*10.0;
    F_Sonar_distance = F_sensor.ping_cm()*10.0;
    if(R_Sonar_distance == 0){R_Sonar_distance = MaxDistance * 70.0;}
    if(L_Sonar_distance == 0){L_Sonar_distance = MaxDistance * 70.0;}
    if(F_Sonar_distance == 0){F_Sonar_distance = MaxDistance * 70.0;}
  }


void setup() {
 Serial.begin(115200);
}

void loop() {
  
  read_sonar_sensor();
  int A=1;
  int B=1;
  int C=1;
  nomal_average_5data (L_Sonar_distance,A);
  A = A+1;
  Serial.print(new_average);
  Serial.print(" ");

  nomal_average_5data (F_Sonar_distance,B);
  B = B+1;
  Serial.print(new_average);
  Serial.print(" ");

  nomal_average_5data (R_Sonar_distance,C);
  C = C+1;
  Serial.println(new_average);
}
