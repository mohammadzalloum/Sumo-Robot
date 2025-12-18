#include <Wire.h>
#include <ctype.h>
#include <math.h>

// ========= ضبط الـ I2C =========
#define SDA_PIN 11
#define SCL_PIN 12
#define PCF_ADDR 0x20   // غيّره إلى 0x21..0x27 حسب A0..A2. ولو كان PCF8574A استخدم 0x38..0x3F

// ========= إعداد الأزرار على PCF =========
// نستخدم P0..P4 (D0..D4) كمدخلات مع سحب لأعلى داخلي (active-low)
const uint8_t BTN_COUNT = 5;
const uint8_t BTN_BITS[BTN_COUNT] = {0,1,2,3,4};  // P0..P4
const uint16_t DEBOUNCE_MS = 30;

// حالة الاستقرار لكل زر
bool stableBit[8];        // 1 = مرفوع (غير مضغوط)، 0 = منخفض (مضغوط)
bool lastRawBit[8];       // آخر قراءة خام
uint32_t lastChangeAt[8]; // آخر لحظة تغيّر فيها الخام

// ========= إعداد البازر (LEDC على ESP32) =========
#define BUZZER_PIN 8
#define LEDC_CH    0
#define LEDC_RES   10  // 10-bit

// ========= أدوات PCF8574 =========
void pcfWrite(uint8_t value) {
  Wire.beginTransmission(PCF_ADDR);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t pcfRead() {
  Wire.requestFrom(PCF_ADDR, (uint8_t)1);
  if (Wire.available()) return Wire.read();
  return 0xFF;
}

// ========= تشغيل نغمة/سكتة =========
void playTone(int freq, int ms) {
  if (freq <= 0) {
    ledcWriteTone(LEDC_CH, 0);
    delay(ms);
    return;
  }
  ledcWriteTone(LEDC_CH, freq);
  delay(ms);
  ledcWriteTone(LEDC_CH, 0);
}

// ========= أدوات RTTTL =========
int noteIndexFromChar(char c) {
  switch (c) {
    case 'c': return 0; case 'd': return 2; case 'e': return 4;
    case 'f': return 5; case 'g': return 7; case 'a': return 9;
    case 'b': return 11; default: return -1;
  }
}

double freqFromNote(int baseIndex, bool sharp, int octave) {
  int idx = baseIndex + (sharp ? 1 : 0);
  int midi = (octave + 1) * 12 + idx;     // C-1 = 0
  double semitones = midi - 69;           // A4=69
  return 440.0 * pow(2.0, semitones / 12.0);
}

void playRTTTL(const char* s) {
  // تخطي الاسم
  while (*s && *s != ':') s++;
  if (*s == ':') s++;

  // قراءة d,o,b الافتراضية
  int defDur = 4, defOct = 6, bpm = 63;
  while (*s && *s != ':') {
    if (*s == 'd' && *(s+1) == '=') {
      s += 2; defDur = 0; while (*s >= '0' && *s <= '9') { defDur = defDur*10 + (*s - '0'); s++; }
    } else if (*s == 'o' && *(s+1) == '=') {
      s += 2; defOct = 0; while (*s >= '0' && *s <= '9') { defOct = defOct*10 + (*s - '0'); s++; }
    } else if (*s == 'b' && *(s+1) == '=') {
      s += 2; bpm = 0; while (*s >= '0' && *s <= '9') { bpm = bpm*10 + (*s - '0'); s++; }
    } else { s++; }
  }
  if (*s == ':') s++;

  long wholenote = (long)(240000.0 / bpm); // ms للنوته الكاملة

  while (*s) {
    // 1) مدة مخصصة؟
    int duration = 0;
    while (*s >= '0' && *s <= '9') { duration = duration*10 + (*s - '0'); s++; }
    if (duration == 0) duration = defDur;

    // 2) نغمة أم سكتة؟
    bool pause = false;
    char n = tolower(*s);
    if (n == 'p') { pause = true; s++; }
    int baseIdx = -1; bool sharp = false;
    if (!pause) {
      baseIdx = noteIndexFromChar(n);
      s++;
      if (*s == '#') { sharp = true; s++; }
    }

    // 3) نقطة (ممدودة)؟
    bool dotted = false;
    if (*s == '.') { dotted = true; s++; }

    // 4) أوكتاف؟
    int octave = defOct;
    if (*s >= '0' && *s <= '9') { octave = *s - '0'; s++; }

    int noteDuration = wholenote / duration;
    if (dotted) noteDuration += noteDuration / 2;

    if (pause || baseIdx < 0) {
      playTone(0, noteDuration);
    } else {
      int f = (int) round(freqFromNote(baseIdx, sharp, octave));
      playTone(f, noteDuration);
    }
    while (*s == ',' || *s == ' ') s++;
  }
}

// ========= دوال الاستجابة لكل زر =========
// P0 -> Pling2
void onBtn0() { 
  Serial.println("D0 -> Pling2");

  playRTTTL("Pling2:d=16,o=7,b=140:f#7,32p,e7");
}
// P1 -> Pling
void onBtn1() { 
  Serial.println("D1 -> Pling");
  playRTTTL("Pling:d=16,o=6,b=140:e6,32p,d6");
}
// P2 -> Tune2
void onBtn2() { 
  Serial.println("D2 -> Tune2");
  playRTTTL("Tune2:d=4,o=6,b=140:d#,8d#,d#,p,d#,c#");
}
// P3 -> Short2
void onBtn3() { 
  Serial.println("D3 -> Short2");
  playRTTTL("Short2:d=4,o=5,b=160:c6,8p,c6");
}
// P4 -> Pling4
void onBtn4() { 
  Serial.println("D4 -> Pling4");
  playRTTTL("Pling4:d=16,o=5,b=100:a6,32p,a5");




  
}

// مصفوفة مؤشرات دوال للنداء حسب الزر
typedef void (*BtnHandler)();
BtnHandler handlers[BTN_COUNT] = { onBtn0, onBtn1, onBtn2, onBtn3, onBtn4 };

void setup() {
  Serial.begin(115200);

  // I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  pcfWrite(0xFF); // كل الأرجل HIGH = مدخلات مع سحب لأعلى

  // تهيئة الحالات
  uint8_t raw = pcfRead();
  for (int i=0; i<8; ++i) {
    bool bit = (raw >> i) & 1;
    stableBit[i] = bit;
    lastRawBit[i] = bit;
    lastChangeAt[i] = millis();
  }

  // بورد البازر
  ledcSetup(LEDC_CH, 2000, LEDC_RES);
  ledcAttachPin(BUZZER_PIN, LEDC_CH);

  Serial.println("Ready. Press P0..P4 for its RTTTL tone.");
}

void loop() {
  uint8_t rawByte = pcfRead();
  uint32_t now = millis();

  // فحص الأزرار P0..P4 فقط
  for (int k=0; k<BTN_COUNT; ++k) {
    uint8_t b = BTN_BITS[k];
    bool raw = (rawByte >> b) & 1;  // 1 = غير مضغوط, 0 = مضغوط

    if (raw != lastRawBit[b]) {
      lastRawBit[b] = raw;
      lastChangeAt[b] = now;
    }

    if ((now - lastChangeAt[b]) > DEBOUNCE_MS && raw != stableBit[b]) {
      stableBit[b] = raw;

      // الحافة الهابطة = ضغط الزر
      if (stableBit[b] == 0) {
        handlers[k]();

        // (اختياري) انتظر الإفلات لتجنّب التكرار أثناء الضغط الطويل
        while ( ((pcfRead() >> b) & 1) == 0 ) delay(5);
      }
    }
  }

  delay(1);
}
