# Çizgi Takip Eden Robot - Arduino Nano (TB6612FNG)

Bu proje, 6 adet analog IR sensör ve 2 adet TB6612FNG motor sürücü kullanarak siyah çizgiyi takip eden bir robot projesidir.

## Gerekli Malzemeler
- 1x Arduino Nano
- 2x TB6612FNG Motor Sürücü
- 6x Kızılötesi (IR) Analog Sensör (A0-A7 arası)
- 4x DC Motor ve Tekerlek (Sol ve Sağ gruplar halinde)
- 1x Robot Gövdesi
- 1x 7.4V veya 11.1V Lipo Pil

## Bağlantı Şeması

### Sol Motor Sürücü (TB6612FNG #1)
- PWMA -> D5
- AIN1 -> D2
- AIN2 -> D3
- PWMB -> D6
- BIN1 -> D4
- BIN2 -> D7
- STBY -> D8 (Ortak)

### Sağ Motor Sürücü (TB6612FNG #2)
- PWMA -> D9
- AIN1 -> D10
- AIN2 -> D11
- PWMB -> D12
- BIN1 -> A4
- BIN2 -> A5
- STBY -> D8 (Ortak)

### Sensörler (Analog)
- Sensör 1-6 -> Arduino A0, A1, A2, A3, A6, A7 pinleri

## Yazılım Kurulumu
1. `line_follower.ino` dosyasını Arduino IDE ile açın.
2. Kart olarak "Arduino Nano" seçin.
3. Gerekli ayarları (hız, eşik değeri) kod içerisinden yapabilirsiniz:
   - `baseSpeed`: Robotun temel hızı
   - `turnSpeed`: Dönüş hızı
   - `threshold`: Siyah/Beyaz algılama eşiği
4. Kodu Arduino Nano'ya yükleyin.