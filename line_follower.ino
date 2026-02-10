// ----- MOTOR PINLERİ (2 x TB6612FNG) -----
// Sürücü 1 - SOL motorlar
#define L_AIN1 2
#define L_AIN2 3
#define L_PWMA 5
#define L_BIN1 4
#define L_BIN2 7
#define L_PWMB 6

// Sürücü 2 - SAĞ motorlar
#define R_AIN1 10
#define R_AIN2 11
#define R_PWMA 9
#define R_BIN1 A4
#define R_BIN2 A5
#define R_PWMB 12

// Her iki sürücünün STBY pinleri D8'e bağlı (ortak)
#define STBY 8

// ----- SENSÖR PINLERİ -----
int sensorPins[6] = {A0, A1, A2, A3, A6, A7};
int sensorValues[6];

// ----- AYARLAR -----
int baseSpeed = 120;   // düz giderken hız
int turnSpeed = 80;    // dönüş hızı
int threshold = 500;   // siyah-beyaz ayırımı

void setup() {
  // SOL sürücü pinleri
  pinMode(L_AIN1, OUTPUT);
  pinMode(L_AIN2, OUTPUT);
  pinMode(L_PWMA, OUTPUT);
  pinMode(L_BIN1, OUTPUT);
  pinMode(L_BIN2, OUTPUT);
  pinMode(L_PWMB, OUTPUT);

  // SAĞ sürücü pinleri
  pinMode(R_AIN1, OUTPUT);
  pinMode(R_AIN2, OUTPUT);
  pinMode(R_PWMA, OUTPUT);
  pinMode(R_BIN1, OUTPUT);
  pinMode(R_BIN2, OUTPUT);
  pinMode(R_PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
}

void loop() {
  readSensors();

  // ORTA ÇİZGİ
  if (sensorValues[2] > threshold && sensorValues[3] > threshold) {
    forward(baseSpeed, baseSpeed);
  }
  // SOLA KAYDI
  else if (sensorValues[0] > threshold || sensorValues[1] > threshold) {
    forward(turnSpeed, baseSpeed);
  }
  // SAĞA KAYDI
  else if (sensorValues[4] > threshold || sensorValues[5] > threshold) {
    forward(baseSpeed, turnSpeed);
  }
  // ÇİZGİ KAYBOLURSA
  else {
    stopMotors();
  }
}

// ----- FONKSİYONLAR -----

void readSensors() {
  for (int i = 0; i < 6; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
}

void forward(int leftSpeed, int rightSpeed) {
  // Güç ver
  digitalWrite(STBY, HIGH);

  // SOL sürücüdeki iki motoru da ileri sür
  digitalWrite(L_AIN1, HIGH);
  digitalWrite(L_AIN2, LOW);
  digitalWrite(L_BIN1, HIGH);
  digitalWrite(L_BIN2, LOW);
  analogWrite(L_PWMA, leftSpeed);
  analogWrite(L_PWMB, leftSpeed);

  // SAĞ sürücüdeki iki motoru da ileri sür
  digitalWrite(R_AIN1, HIGH);
  digitalWrite(R_AIN2, LOW);
  digitalWrite(R_BIN1, HIGH);
  digitalWrite(R_BIN2, LOW);
  analogWrite(R_PWMA, rightSpeed);
  analogWrite(R_PWMB, rightSpeed);
}

void stopMotors() {
  analogWrite(L_PWMA, 0);
  analogWrite(L_PWMB, 0);
  analogWrite(R_PWMA, 0);
  analogWrite(R_PWMB, 0);

  // İstersen tam durdurmak için standby'a da alabilirsin
  // digitalWrite(STBY, LOW);
}