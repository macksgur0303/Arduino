// C++ code
//
void setup()
{
  pinMode(13, OUTPUT); // 핀의 모드를 설정한다
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(13, HIGH);
  Serial.println("LED ON");
  delay(2000); // Wait for 1000 millisecond(s)
  
  digitalWrite(13, LOW);
  Serial.println("LED OFF");
  delay(2000); // Wait for 1000 millisecond(s)
}