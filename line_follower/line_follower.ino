#include <QTRSensors.h>

#define IR_USE_AVR_TIMER1        // IRremote'u Timer1'e al (Timer2, PWM D3/D11 icin serbest kalsin)
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2

// ===================== TB6612FNG PIN TANIMLARI =====================
const uint8_t PWMA = 3;   // Sol motor PWM  (Timer2)
const uint8_t AIN1 = 4;   // Sol motor yön 1
const uint8_t AIN2 = 5;   // Sol motor yön 2
const uint8_t STBY = 6;   // Standby (HIGH = aktif)
const uint8_t BIN1 = 7;   // Sag motor yön 1
const uint8_t BIN2 = 8;   // Sag motor yön 2
const uint8_t PWMB = 11;  // Sag motor PWM  (Timer2)

// ===================== KALIBRASYON LED'I =====================
const uint8_t LED_PIN = 13;

// ===================== QTR-8A SENSOR =====================
const uint8_t SENSOR_COUNT = 8;
QTRSensors qtr;
uint16_t sensorValues[SENSOR_COUNT];

// ===================== PID PARAMETRELERI =====================
float Kp = 0.065f;
float Ki = 0.0003f;
float Kd = 0.55f;

// ===================== HIZ AYARLARI =====================
const int BASE_SPEED  = 100;
const int MAX_SPEED   = 200;
const int CALIB_SPEED = 70;

// ===================== PID DEGISKENLERI =====================
long     integral     = 0;
int      lastError    = 0;
uint16_t lastPosition = 3500;

// ===================== DURUM DEGISKENLERI =====================
bool robotCalisiyor = false;
bool kalibreEdildi  = false;

// ===================== SETUP =====================
void setup() {
  Serial.begin(115200);

  // Motor pinlerini cikis yap
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(STBY, HIGH);

  // QTR-8A yapilandirmasi
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SENSOR_COUNT);
  qtr.setSamplesPerSensor(4);

  // IR alici baslat
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

  stopMotors();

  Serial.println(F(">> Hazir. Kumandadan herhangi bir tusa basin."));
  Serial.println(F("   1. basin  -> Kalibrasyon (10 sn)"));
  Serial.println(F("   2. basin+ -> Baslat / Durdur"));
}

// ===================== ANA DONGU =====================
void loop() {

  // ---------- IR KUMANDA KONTROLU ----------
  if (IrReceiver.decode()) {

    // Tekrar (repeat) sinyallerini yoksay
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      IrReceiver.resume();
      return;
    }

    if (!kalibreEdildi) {
      // Ilk basin: kalibrasyon baslat
      Serial.println(F(">> Kalibrasyon basliyor (10 sn)..."));
      calibrateSensors();
      printCalibrationData();
      kalibreEdildi = true;
      Serial.println(F(">> Kalibrasyon tamamlandi. Tekrar basin -> Baslat."));
    } else {
      // Sonraki basinlar: baslat / durdur
      robotCalisiyor = !robotCalisiyor;

      if (robotCalisiyor) {
        // PID durumunu sifirla
        integral     = 0;
        lastError    = 0;
        lastPosition = 3500;
        Serial.println(F(">> Robot BASLADI."));
      } else {
        stopMotors();
        Serial.println(F(">> Robot DURDU."));
      }
    }

    delay(350);
    IrReceiver.resume();
  }

  // Robot calismiyorsa cizgi takibini yapma
  if (!robotCalisiyor) return;

  // ---------- CIZGI TAKIBI (PID) ----------
  uint16_t position = qtr.readLineBlack(sensorValues);

  // Cizgi algilama kontrolu
  bool lineDetected = false;
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    if (sensorValues[i] > 150) {
      lineDetected = true;
      break;
    }
  }

  if (!lineDetected) {
    // Cizgi kayboldu: son bilinen yone don
    if (lastPosition < 3500) {
      setMotors(-60, 60);   // Sola ara
    } else {
      setMotors(60, -60);   // Saga ara
    }
    return;
  }

  lastPosition = position;

  // PID hesaplamasi
  int error = (int)position - 3500;

  integral += error;
  integral  = constrain(integral, -8000, 8000);

  int derivative = error - lastError;
  lastError = error;

  int correction = (int)(Kp * error + Ki * integral + Kd * derivative);

  // Motor hizlarini hesapla ve sinirla
  int leftSpeed  = constrain(BASE_SPEED + correction, -MAX_SPEED, MAX_SPEED);
  int rightSpeed = constrain(BASE_SPEED - correction, -MAX_SPEED, MAX_SPEED);

  setMotors(leftSpeed, rightSpeed);
}

// ===================== KALIBRASYON =====================
void calibrateSensors() {
  digitalWrite(LED_PIN, HIGH);

  unsigned long start      = millis();
  unsigned long phaseStart = start;
  uint8_t phase = 0;

  while (millis() - start < 10000UL) {
    if (millis() - phaseStart > 1250UL) {
      phase = (phase + 1) % 8;
      phaseStart = millis();
    }

    if (phase % 2 == 0) {
      setMotors(CALIB_SPEED, -CALIB_SPEED);
    } else {
      setMotors(-CALIB_SPEED, CALIB_SPEED);
    }

    qtr.calibrate();
    delay(5);
  }

  stopMotors();

  // LED 3 kez yanip sonsun (kalibrasyon bitti sinyali)
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

// ===================== KALIBRASYON SONUCLARINI YAZDIR =====================
void printCalibrationData() {
  Serial.print(F("Min: "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print('\t');
  }
  Serial.println();

  Serial.print(F("Max: "));
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print('\t');
  }
  Serial.println();

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    int range = qtr.calibrationOn.maximum[i] - qtr.calibrationOn.minimum[i];
    if (range < 100) {
      Serial.print(F("!! UYARI: Sensor "));
      Serial.print(i);
      Serial.println(F(" yeterince kalibre olamadi (aralik < 100)."));
    }
  }
}

// ===================== TEK MOTOR KANALI SURME =====================
void setMotorChannel(uint8_t in1, uint8_t in2, uint8_t pwm, int speed) {
  if (speed >= 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  analogWrite(pwm, constrain(abs(speed), 0, 255));
}

// ===================== IKI MOTORU BIR ARADA SUR =====================
void setMotors(int leftSpeed, int rightSpeed) {
  digitalWrite(STBY, HIGH);
  setMotorChannel(AIN1, AIN2, PWMA, leftSpeed);
  setMotorChannel(BIN1, BIN2, PWMB, rightSpeed);
}

// ===================== MOTORLARI DURDUR =====================
void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}
