// ====== Pin map كما طلبت ======
const int IR_FR = 16;  // Front Right  (ADC2)
const int IR_FM = 17;  // Front Mid    (ADC2)
const int IR_FL = 18;  // Front Left   (ADC2)
const int IR_BL = 12;  // Back  Left   (ADC2)
const int IR_BM = 13;  // Back  Mid    (ADC2)
const int IR_BR = 14;  // Back  Right  (ADC2)

// ضبط عام
struct IRChan { int pin; int thr; int ema; bool state; };
IRChan chans[] = {
  {IR_FR,0,0,false},{IR_FM,0,0,false},{IR_FL,0,0,false},
  {IR_BL,0,0,false},{IR_BM,0,0,false},{IR_BR,0,0,false}
};

const int HYST = 80;     // هيستريسيس (عدّل حسب تجربتك)
const int N_CAL = 300;   // عدد عينات المعايرة لكل حساس

void calibrate(IRChan &c){
  int mn = 4095, mx = 0;
  for(int i=0;i<N_CAL;i++){
    int v = analogRead(c.pin);
    if(v<mn) mn=v; if(v>mx) mx=v;
    delay(2);
  }
  c.thr = (mn + mx) / 2;   // عتبة وسطية
  c.ema = 0;
}

void setup(){
  Serial.begin(115200);
  delay(200);
  Serial.println("\n== TCRT-5000 6x Analog Test ==");

  // ESP32-S3: دقة وافتراض حتى ~3.3V
  analogReadResolution(12);       // 0..4095
  analogSetAttenuation(ADC_11db); // مدى جهد أوسع

  // معايرة كل حساس (خذ القيم على الأرضية الطبيعية)
  for(auto &c : chans){
    calibrate(c);
    Serial.printf("Calibrated pin=%d  thr=%d\n", c.pin, c.thr);
  }
  Serial.println("Start reading...\n");
}

void loop(){
  static unsigned long t = 0;

  for(auto &c : chans){
    int raw = analogRead(c.pin);
    c.ema = (c.ema==0) ? raw : (c.ema*7 + raw)/8; // EMA α=1/8

    // ملاحظة: غالبًا الأبيض/الحافة يعطي قراءة "أعلى".
    // إن لاحظت العكس على وحدتك، بدّل إشارات < و > أدناه.
    if(!c.state && c.ema > c.thr + HYST) c.state = true;      // أبيض/حافة
    else if(c.state && c.ema < c.thr - HYST) c.state = false; // أسود/أرضية
  }

  if(millis()-t >= 100){
    t = millis();
    Serial.printf("FR[%4d/%4d:%d] FM[%4d/%4d:%d] FL[%4d/%4d:%d]  |  "
                  "BL[%4d/%4d:%d] BM[%4d/%4d:%d] BR[%4d/%4d:%d]\n",
      analogRead(IR_FR), chans[0].ema, chans[0].state,
      analogRead(IR_FM), chans[1].ema, chans[1].state,
      analogRead(IR_FL), chans[2].ema, chans[2].state,
      analogRead(IR_BL), chans[3].ema, chans[3].state,
      analogRead(IR_BM), chans[4].ema, chans[4].state,
      analogRead(IR_BR), chans[5].ema, chans[5].state
    );
  }
}
