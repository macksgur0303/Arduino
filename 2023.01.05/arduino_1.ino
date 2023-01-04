int tones[] = {523,587,659,698,784,880,988,1046};

void setup()
{
  pinMode(12, OUTPUT);
}

void loop()
{
  tone(12, tones[4], 500); // 
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[4], 500); // 
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[5], 500); 
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[5], 500); 
  delay(1000);		  // 1초 동안 시간 지연
  
  tone(12, tones[4], 500); 
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[4], 500); 
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[2], 1000); 
  delay(1000);		  // 1초 동안 시간 지연
  
  tone(12, tones[4], 500); 
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[4], 500); 
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[2], 500);
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[2], 500);
  delay(1000);		  // 1초 동안 시간 지연
  tone(12, tones[1], 1500);
  delay(3000);		  // 13초 동안 시간 지연
  
  
}