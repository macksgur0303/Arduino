#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

#define Voltage_Pin      A0     // 전압측정 센서 핀
#define Current_Pin      A1     // 전류측정 센서 핀

float Sensor_Offset = NULL;     // 센서 오프셋 값
float Sensitivity   = NULL;     // 센서 감도 값
float Voltage_R1    = NULL;     // 전압 분압기의 저항 R1 값
float Voltage_R2    = NULL;     // 전압 분압기의 저항 R2 값
float V_max         = NULL;     // 최대 전압 값
float V_min         = NULL;     // 최소 전압 값

#define SOC_Dial         A2
#define Mod1_Switch      10
#define Mod2_Switch      9
#define Mod3_Switch      8
#define Process_Btn      7
 
float P_max         = 700;      // 최대 전력 값
float P_min         = 100;      // 최소 전력 값
int SOC_max         = 80;       // 최대 SOC 값
int SOC_min         = 40;       // 최소 SOC 값

void setup() {
  Serial.begin(115200);  // 시리얼 통신 시작
  lcd.begin(16,2);
  
  pinMode(Mod1_Switch, INPUT);
  pinMode(Mod2_Switch, INPUT);
  pinMode(Mod3_Switch, INPUT);
  pinMode(Process_Btn, INPUT_PULLUP);
}

float ma_filter(int Pin){ // 이동 평균 필터 함수 정의
  float sum = 0;

  for(int i = 0; i < 100; i++) {
    sum + analogRead(Pin) * (5 / 1023);  // 아날로그 핀에서 값 읽기 및 변환 (0-5V 범위)
    delay(10);  // 10ms 딜레이
  }

  return sum / 100;  // 평균값 반환
}

int soc_ocv(float Voltage){ 
  if (Voltage <= V_min)
    return 0;  // 최소 전압 이하일 때 SOC 0 반환
  else if (Voltage >= V_max)
    return 100;  // 최대 전압 이상일 때 SOC 100 반환
  else
    return (Voltage - V_min) / (V_max - V_min) * 100;  // 최소 전압과 최대 전압 사이의 백분율로 SOC 계산
}

int decide_Parameter(int flag)
{
  switch(flag)
  {
    case 1:
      if(digitalRead(Mod3_Switch) == HIGH)
        return 1000;
    
      if(digitalRead(Mod2_Switch) == HIGH)
        return 350;
    
      if(digitalRead(Mod1_Switch) == HIGH)
        return 50;
    
      return 0;
      break;

    case 2:
      if(analogRead(SOC_Dial) <= 1023/3)
        return 10;
    
      else if(analogRead(SOC_Dial) <= 1023/3 * 2)
        return 60;
    
      else
        return 100;
    
      return 0;
      break;
  }
}

void wait_btn()
{
  while(1)
  {
    if(digitalRead(Process_Btn) == LOW)
      break;
  }
}
void loop() {
  
  /*
  float I_load = (ma_filter(Current_Pin) - Sensor_Offset) / Sensitivity; // 전류 센서를 사용하여 부하 전류 계산
  float V_load = ma_filter(Voltage_Pin) * (Voltage_R1 + Voltage_R2) / Voltage_R2;   // 전압 센서를 사용하여 부하 전압 계산
  float P_load = I_load * V_load; // 전력 계산
  
  int SOC = soc_ocv(V_load); // OCV 기반 SOC 계산
  */
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("mod setup");

  wait_btn();

  float P_load = decide_Parameter(1);
  int SOC = decide_Parameter(2);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("P_load = ");
  lcd.print(P_load);
  lcd.setCursor(0, 1);
  lcd.print("SOC = ");
  lcd.print(SOC);
  wait_btn();

  
  float P_fc; // 배터리의 최대 전력 계산
  float P_bat_max = (P_max - P_min) - (SOC - SOC_min) * (P_max - P_min) / SOC_min; // 전력 제어 로직
  
  
  if (P_load <= P_min) {
    if (SOC >= SOC_max)
      P_fc = P_load;  // 최소 전력 이하이고 최대 SOC 이상일 때, 부하 전력 유지
    else
      P_fc = P_min;   // 그 외의 경우, 최소 전력 제어
  } 
  
  else {
    if (P_load > P_max)
      P_fc = P_max;   // 최대 전력 초과 시, 최대 전력 제어
    else {
      if (SOC >= SOC_max)
        P_fc = P_load;  // 최대 SOC 이상일 때, 부하 전력 유지
      else if (SOC < SOC_min)
        P_fc = P_max;   // 최소 SOC 미만일 때, 최대 전력 제어
      else
        P_fc = (P_max - P_load) * P_bat_max / (P_max - P_min) + P_load;  // 그 외의 경우, 전력 계산 공식 적용
    }
  }

  Serial.println(P_fc);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("P_fc = ");
  lcd.print(P_fc);

  wait_btn();
}
