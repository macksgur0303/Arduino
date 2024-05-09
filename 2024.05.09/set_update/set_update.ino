#define Acid_Pin          A0    // 연축전지 센서 핀
#define Scap_Pin          A1    // 슈퍼캐패시터 센서 핀
#define Current_Pin       A2    // 전류측정 센서 핀
#define Relay_Acid_Pin    6     // 연축전지 릴레이 제어 핀
#define Relay_Scap_Pin    7     // 슈퍼캐패시터 릴레이 제어 핀
int LedPin[]       = {3, 4, 5}; // Led 핀

#define V_Ref_Acid        12    // 연축전지 참조 전압
#define V_Ref_Scap        32    // 슈퍼캐패시터 참조 전압
#define Current_Ref       5     // 전류측정 센서 참조 전압
#define Period            1000  // 시간 측정 주기


int V_Soc          = NULL;           // 상태 오류 보정
int Time_Old       = 0;              // 이전 시간
int Time_Max       = NULL;           // 최대 시간
int V_Min;                           // 최소 전압
int V_Max;                           // 최대 전압
int Time_Current;                    // 현재 시간

/*
float Rate_Acid    = 24;
float Rate_Scap    = 1.124;
float C_Rate_Acid  = 0.2;
float C_Rate_Scap  = 2;
*/

float I_Switch     = 4.8;            // 스위치 전류
float I_load;                        // 부하 전류
float Calibration;                   // 전류 측정 센서 켈리브레이션

void setup() {
  Serial.begin(115200);             // 시리얼 통신 초기화 
  pinMode(Relay_Acid_Pin, OUTPUT);  // 연축전지 릴레이 핀 설정
  pinMode(Relay_Scap_Pin, OUTPUT);  // 슈퍼캐패시터 릴레이 핀 설정

  for(int i = 0; i < 3; i++)
    pinMode(LedPin[i], OUTPUT);     // LED 핀 설정
    
  mod_led(1);
  V_Min = V_Ref_Acid - 3;           // V 최솟값 설정
  V_Max = V_Ref_Scap;               // V 최댓값 설정
/*
  float sum = 0;

  for(int i = 0; i < 100; i++)
  {
    sum += ma_filter(Current_Pin, Current_Ref);
    delay(10);
  }

  Calibration = 1.0 / (sum / 100.0);
*/
}

float ma_filter(int Pin, float Ref)
{
  float sum = 0;

  for(int i = 0; i < 100; i++)
  {
    sum + analogRead(Pin) * (Ref / 1023);
    delay(10)
  }

  return sum / 100;
}

void mod_led(int flag)
{
  if(flag != 8) // 동작단계 8일경우 led off
  {
    for (int i = 0; i < 3; i++) 
      digitalWrite(LedPin[i], (flag >> (2 - i)) & 1);
  }

  else
  {
    for (int i = 0; i < 3; i++) 
      digitalWrite(LedPin[i], LOW);
  }

  delay(1000);
}

void time_measure()
{
  Time_Current = millis();  // 현재 시간 측정

  /*
    if (Time_Current - Time_Old >= Period)
      Time_Old = Time_Current;
  */
}

void compare_value() // 동작 단계 4
{
  mod_led(4);
  time_measure();  // 시간 측정 함수 호출

  if (I_load < I_Switch) // (I_load가 I_Switch보다 작을때)
    moving_by_acid(); // 동작 단계 5
  else // (if의 조건문이 아닐경우)
    moving_by_scap(); // 동작 단계 6
}

void moving_by_acid() // 동작 단계 5
{
  mod_led(5);
  digitalWrite(Relay_Acid_Pin, HIGH);   // 연축전지 릴레이 활성화
  digitalWrite(Relay_Scap_Pin, LOW);    // 슈퍼캐패시터 릴레이 비활성화
  delay(200);

  if (Time_Current >= Time_Max) // 동작 단계 8
  {
    mod_led(8);
    digitalWrite(Relay_Acid_Pin, LOW);   // 연축전지 릴레이 비활성화
    digitalWrite(Relay_Scap_Pin, LOW);   // 슈퍼캐패시터 릴레이 비활성화
    delay(200);
  }
}

void moving_by_scap() // 동작 단계 6
{
  mod_led(6);
  if (V_Soc > V_Min) // 동작 단계 7(조건 1)
  {
    mod_led(7);
    digitalWrite(Relay_Acid_Pin, LOW);    // 연축전지 릴레이 비활성화
    digitalWrite(Relay_Scap_Pin, HIGH);   // 슈퍼캐패시터 릴레이 활성화
    delay(200);

    if (Time_Current >= Time_Max) // 동작 단계 8(조건 2)
    {
      mod_led(8);
      digitalWrite(Relay_Acid_Pin, LOW);   // 연축전지 릴레이 비활성화
      digitalWrite(Relay_Scap_Pin, LOW);   // 슈퍼캐패시터 릴레이 비활성화
      delay(200);
    }
    else(조건 2가 아닐경우)
      compare_value();   // 동작 단계 4 호출(위의 void compare_value 함수로 이동)
  }

  else // V_Soc가 V_Min 보다 작거나 같을때
    moving_by_acid(); // 동작 단계 5
}

void loop() {
  mod_led(2);
  int V_acid = ma_filter(Acid_Pin, V_Ref_Acid); // 동작 단계 2
  int V_scap = ma_filter(Scap_Pin, V_Ref_Scap); // 동작 단계 2
  I_load = ma_filter(Current_Pin, Current_Ref) //* Calibration; // moving average 필터에 * 전류 calibration

  mod_led(3);
  if (V_acid >= V_Min && V_scap >= V_Min); // 동작 단계 3(V_acid 와 V_scap이 둘다 V_Min이상)
  {
    digitalWrite(Relay_Acid_Pin, HIGH);   // 연축전지 릴레이 활성화
    digitalWrite(Relay_Scap_Pin, LOW);    // 슈퍼캐패시터 릴레이 비활성화
    delay(200);
    compare_value();  // 동작 단계 4 호출(위의 void compare_value 함수로 이동)
  }

  if (V_acid < V_Min && V_scap < V_Min); // 동작 단계 8(V_acid 와 V_scap이 둘다 V_Min보다 작음)
  {
    mod_led(8);
    digitalWrite(Relay_Acid_Pin, LOW);   // 연축전지 릴레이 비활성화
    digitalWrite(Relay_Scap_Pin, LOW);   // 슈퍼캐패시터 릴레이 비활성화
    delay(200);
  }
}
