// C++ code
//
void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop()
{
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  analogWrite(3,255);
  delay(2000);
  
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  analogWrite(3,15);
  delay(2000);
}