// TB6612FNG Pin Tanımlamaları
const int PWMA = 3;  // Sol Motor Hız
const int AIN1 = 4;  // Sol Motor Yön 1
const int AIN2 = 5;  // Sol Motor Yön 2
const int STBY = 6;  // Standby Pini (HIGH olmalı)
const int BIN1 = 7;  // Sağ Motor Yön 1
const int BIN2 = 8;  // Sağ Motor Yön 2
const int PWMB = 11; // Sağ Motor Hız

void setup() {
  // Tüm pinleri çıkış olarak ayarla
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  digitalWrite(STBY, HIGH); // Sürücüyü aktif et
}

void loop() {
  // İLERİ GİT
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 150); // %60 hız

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 150);
  
  delay(2000); // 2 saniye bekle

  // DUR
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN1, LOW);
  delay(1000);

  // GERİ GİT
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 150);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 150);
  
  delay(2000);
}