#include <IRremote.h>

IRrecv ir_recv(10);
decode_results results;

void setup()
{
  pinMode(2, OUTPUT);
  ir_recv.enableIRIn(); // 적외선 센서 수신 활성화
  Serial.begin(9600);
}

void loop()
{
  int cds_sensor_value = 0;
  
  cds_sensor_value = analogRead(A0);
  //Serial.println(cds_sensor_value);
  
  if(ir_recv.decode(&results))
  {
    Serial.println(results.value);
    delay(30);
    ir_recv.resume();
    
    if (results.value == 16593103) // 0번을 누른다
    {
      digitalWrite(2, HIGH);
      delay(2000);
    }
    if (results.value == 16582903) // 1번을 누른다
    {
      digitalWrite(2, LOW);
      delay(2000);
    }
  }
  
  if(cds_sensor_value < 200)
  {
    digitalWrite(2, HIGH);
  }
  else
  {
  	digitalWrite(2, LOW);
  }
}