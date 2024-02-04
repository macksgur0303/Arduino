#define Acid_Pin          A0   // 연축전지 센서 핀
#define Scap_Pin          A1   // 슈퍼캐패시터 센서 핀
#define Relay_Acid_Pin    6    // 연축전지 릴레이 제어 핀
#define Relay_Scap_Pin    7    // 슈퍼캐패시터 릴레이 제어 핀
#define V_Ref_Acid        5    // 연축전지 참조 전압
#define V_Ref_Scap        5     // 슈퍼캐패시터 참조 전압

int V_Min      = NULL;         // 최소 전압
int V_Max      = NULL;         // 최대 전압
int V_Soc      = NULL;         // 상태 오류 보정
int I_Switch   = NULL;         // 스위치 전류
int I_load     = NULL;         // 부하 전류
int Period     = 1000;         // 시간 측정 주기
int Time_Old   = 0;            // 이전 시간
int Time_Max   = NULL;         // 최대 시간
int Time_Current;              // 현재 시간
int V_Ref_Acid = 5;            // 연축전지 참조 전압
int V_Ref_Scap = 5;            // 슈퍼캐패시터 참조 전압

void setup() {
  Serial.begin(115200);             // 시리얼 통신 초기화 
  pinMode(Relay_Acid_Pin, OUTPUT);  // 연축전지 릴레이 핀 설정
  pinMode(Relay_Scap_Pin, OUTPUT);  // 슈퍼캐패시터 릴레이 핀 설정
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
  time_measure();  // 시간 측정 함수 호출

  if (I_load < I_Switch) // (I_load가 I_Switch보다 작을때)
    moving_by_acid(); // 동작 단계 5
  else // (if의 조건문이 아닐경우)
  {
    moving_by_scap(); // 동작 단계 6
  }
}

void moving_by_acid() // 동작 단계 5
{
  digitalWrite(Relay_Acid_Pin, HIGH);   // 연축전지 릴레이 활성화
  digitalWrite(Relay_Scap_Pin, LOW);    // 슈퍼캐패시터 릴레이 비활성화
  delay(200);

  if (Time_Current >= Time_Max) // 동작 단계 8
  {
    digitalWrite(Relay_Acid_Pin, LOW);   // 연축전지 릴레이 비활성화
    digitalWrite(Relay_Scap_Pin, LOW);   // 슈퍼캐패시터 릴레이 비활성화
    delay(200);
  }
}

void moving_by_scap() // 동작 단계 6
{
  digitalWrite(Relay_Acid_Pin, LOW);    // 연축전지 릴레이 비활성화
  digitalWrite(Relay_Scap_Pin, HIGH);   // 슈퍼캐패시터 릴레이 활성화
  delay(200);

  if (V_Soc > V_Min) // 동작 단계 7(조건 1)
  {
    if (Time_Current >= Time_Max) // 동작 단계 8(조건 2)
    {
      digitalWrite(Relay_Acid_Pin, LOW);   // 연축전지 릴레이 비활성화
      digitalWrite(Relay_Scap_Pin, LOW);   // 슈퍼캐패시터 릴레이 비활성화
      delay(200);
    }
    else(조건 2가 아닐경우)
      compare_value();   // 동작 단계 4 호출(위의 void compare_value 함수로 이동)
  }
}

void loop() {
  int V_acid = (analogRead(Acid_Pin) * V_Ref_Acid / 1023); // 동작 단계 2
  int V_scap = (analogRead(Scap_Pin) * V_Ref_Scap / 1023); // 동작 단계 2

  if (V_acid >= V_Min && V_scap >= V_Min); // 동작 단계 3(V_acid 와 V_scap이 둘다 V_Min이상)
  {
    digitalWrite(Relay_Acid_Pin, HIGH);   // 연축전지 릴레이 활성화
    digitalWrite(Relay_Scap_Pin, LOW);    // 슈퍼캐패시터 릴레이 비활성화
    delay(200);
    compare_value();  // 동작 단계 4 호출(위의 void compare_value 함수로 이동)
  }

  if (V_acid < V_Min && V_scap < V_Min); // 동작 단계 8(V_acid 와 V_scap이 둘다 V_Min보다 작음)
  {
    digitalWrite(Relay_Acid_Pin, LOW);   // 연축전지 릴레이 비활성화
    digitalWrite(Relay_Scap_Pin, LOW);   // 슈퍼캐패시터 릴레이 비활성화
    delay(200);
  }
}
