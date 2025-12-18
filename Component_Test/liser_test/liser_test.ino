#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// ===== ضبط أرجل الـ I2C على الـ ESP32 =====
#define SDA_PIN 9
#define SCL_PIN 10

// عنوان مبدّل TCA9548A
#define TCA_ADDR 0x70

// إذا كانت لوحتك تكتب SD1..SD8 فعّل true
// إذا كانت SD0..SD7 اجعلها false
const bool LABEL_STARTS_AT_ONE = true;

// القنوات التي عليها الحساسات (حسب تسليكك SD2..SD8)
uint8_t channel_labels[] = {2, 3, 4, 5, 6, 7, 8};

// مصفوفة الحساسات (واحد لكل قناة)
Adafruit_VL53L0X lox[sizeof(channel_labels) / sizeof(channel_labels[0])];
bool sensor_ok[sizeof(channel_labels) / sizeof(channel_labels[0])];

size_t N = sizeof(channel_labels) / sizeof(channel_labels[0]);

// اختيار قناة على TCA9548A بحسب "رقم اللايبل" المطبوع SDx
void tcaSelectByLabel(uint8_t label) {
  uint8_t idx = label;
  if (LABEL_STARTS_AT_ONE) {
    if (label == 0) return; // حماية
    idx = label - 1;        // حوّل 1..8 إلى 0..7
  }
  if (idx > 7) return;       // حماية
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << idx);      // فعّل قناة idx فقط
  Wire.endTransmission();
  delayMicroseconds(300);    // استقرار بسيط
}

// فحص وجود عنوان I2C معيّن على القناة الحالية
bool i2cAddressSeen(uint8_t address) {
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println(F("=== TCA9548A + 7x VL53L0X Test ==="));

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000); // 400kHz أسرع وأفضل لـ VL53L0X

  // تأكد أن المبدّل يرد
  Wire.beginTransmission(TCA_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println(F("[خطأ] لم يتم العثور على TCA9548A على العنوان 0x70! تأكد من التوصيل."));
  } else {
    Serial.println(F("[OK] تم العثور على TCA9548A على 0x70."));
  }

  // تهيئة كل حساس على قناته
  for (size_t i = 0; i < N; i++) {
    uint8_t ch = channel_labels[i];
    tcaSelectByLabel(ch);

    // فحص سريع: هل نرى VL53L0X (0x29) على هذه القناة؟
    if (!i2cAddressSeen(0x29)) {
      Serial.print(F("CH "));
      Serial.print((int)ch);
      Serial.println(F(": لم يتم العثور على VL53L0X (0x29)."));
      sensor_ok[i] = false;
      continue;
    }

    // حاول البدء
    if (!lox[i].begin()) {
      Serial.print(F("CH "));
      Serial.print((int)ch);
      Serial.println(F(": فشل begin() للحساس."));
      sensor_ok[i] = false;
    } else {
      sensor_ok[i] = true;
      Serial.print(F("CH "));
      Serial.print((int)ch);
      Serial.println(F(": VL53L0X جاهز."));
      // ملاحظة: مكتبة Adafruit لا تعطي واجهة مباشرة لتعديل timing budget هنا،
      // لكنها مناسبة للاختبار الافتراضي.
    }
    delay(10);
  }

  Serial.println(F("البدء بالقراءة..."));
}

void loop() {
  // قراءة متسلسلة لكل حساس
  for (size_t i = 0; i < N; i++) {
    if (!sensor_ok[i]) continue;

    uint8_t ch = channel_labels[i];
    tcaSelectByLabel(ch);

    VL53L0X_RangingMeasurementData_t measure;
    lox[i].rangingTest(&measure, false);  // false = بدون طباعة داخلية

    Serial.print(F("CH "));
    Serial.print((int)ch);
    Serial.print(F(": "));

    if (measure.RangeStatus != 4) { // 4 = phase failure/قراءة غير صالحة
      Serial.print((int)measure.RangeMilliMeter);
      Serial.println(F(" mm"));
    } else {
      Serial.println(F("خارج النطاق / قراءة غير صالحة"));
    }

    delay(50); // مهلة صغيرة بين القنوات
  }

  Serial.println(F("------------------------------"));
  delay(250); // تكرار كل ~0.25 ثانية
}
