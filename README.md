# Projekt-ugrađeni-sustavi
# Garažni Sustav

Arduino sustav za upravljanje dvostrukim garažnim vratima , parking rampama, s I2C OLED prikazom stanja u stvarnom vremenu.

## Značajke

- Upravljanje dvjema rampama preko servo motora (4 tipke: otvori/zatvori za svaku rampu)
- Sigurnosne limit sklopke koje trenutačno zaustavljaju motore
- LED i zvučna signalizacija (buzzer)
- Prikaz statusa na OLED ekranu (128x64, I2C)
- Nema gotovih vanjskih biblioteka (`Servo.h`, `Wire.h`, `Adafruit_GFX/SSD1306`) — sve je napisano od nule izravnim radom s AVR registrima

## Komponente

- Arduino Nano (ATmega328P)
- 2x servo motor (SG90)
- OLED ekran 128x64, I2C (SSD1306)
- 4x tipkalo, 2x limit sklopka
- 2x LED (crvena/zelena), piezo buzzer

## Struktura projekta

```
GarazniSustav/
├── GarazniSustav.ino   # glavna programska logika
├── MarServo.h/.cpp     # upravljanje servo motorima (Timer1 PWM)
├── MarI2C.h/.cpp       # I2C komunikacija (TWI registri)
├── MarOLED.h/.cpp      # upravljački program za OLED ekran
└── MarFont.h           # bitmap tablica znakova (5x7 font)
```

## Pokretanje

1. Otvori `GarazniSustav/GarazniSustav.ino` u Arduino IDE-u
2. Odaberi ploču **Arduino Nano** i ispravan COM port
3. Provjeri I2C adresu OLED ekrana (`OLED_ADDR` na vrhu koda, obično `0x3C`)
4. Učitaj kod na ploču

## Dokumentacija

Detaljna tehnička dokumentacija nalazi se u `dokumentacija/Dokumentacija ugrađeni sustavi.docx`.
