#include <Arduino.h>

const int PWM_PIN = 5;   // اختر دبابيس عامة مثل 2,4,5,6,7,15,36-38
const int CH = 0, FREQ = 20000, RES = 10;

void setup(){
  ledcSetup(CH, FREQ, RES);
  ledcAttachPin(PWM_PIN, CH);
}

void loop(){
  for (int d=0; d<1024; d+=8){ ledcWrite(CH, d); delay(5); }
  for (int d=1023; d>=0; d-=8){ ledcWrite(CH, d); delay(5); }
}

