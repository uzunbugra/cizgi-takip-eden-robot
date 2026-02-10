# Çizgi Takip Eden Robot - Arduino Nano

Bu proje, iki adet IR sensör ve bir L298N motor sürücü kullanarak siyah çizgiyi takip eden bir robot projesidir.

## Gerekli Malzemeler
- 1x Arduino Nano
- 1x L298N Çift Kanallı Motor Sürücü
- 2x Kızılötesi (IR) Engelden Kaçma/Çizgi Sensörü
- 2x DC Motor ve Tekerlek
- 1x Robot Gövdesi
- 1x 7.4V veya 9V Güç Kaynağı (Lipo veya Pil)

## Bağlantı Şeması

### Motor Sürücü (L298N) -> Arduino
- ENA -> D5 (PWM)
- IN1 -> D6
- IN2 -> D7
- IN3 -> D8
- IN4 -> D9
- ENB -> D10 (PWM)

### Sensörler -> Arduino
- Sol Sensör Out -> D2
- Sağ Sensör Out -> D3
- VCC -> 5V
- GND -> GND

## Yazılım Kurulumu
1. `line_follower.ino` dosyasını Arduino IDE ile açın.
2. Kart olarak "Arduino Nano" seçin.
3. Uygun COM portunu seçip yükleyin.
