#include "MarServo.h"
#include "MarOLED.h"

// oled
#define OLED_ADDR 0x3C

MarOLED oled(OLED_ADDR);

bool oledOK = false;
unsigned long zadnjiOLED = 0;
const unsigned long OLED_INTERVAL = 200;

const byte MOTOR_1 = 10;
const byte MOTOR_2 = 9;

const byte BUZZER = 11;
const byte LIMIT_SVI = 2;

const byte GUMB_M1_ZATVARANJE = 6;
const byte GUMB_M1_OTVARANJE = 3;

const byte GUMB_M2_ZATVARANJE = 5;
const byte GUMB_M2_OTVARANJE = 4;

const byte LED_CRVENA = 8;
const byte LED_ZELENA = 7;

const bool LIMIT_AKTIVAN_KAD_HIGH = true;

int kutM1 = 10;
int kutM2 = 10;

const int KUT_ZATVORENO = 10;
const int KUT_OTVORENO = 90;

const int KORAK = 1;
const unsigned long BRZINA = 20;

unsigned long zadnjePomicanje = 0;
unsigned long zadnjiZvukLimit = 0;

bool proslaNaredba = false;

bool pritisnuto(byte pin) {
    return digitalRead(pin) == HIGH;
}

bool limitAktivan() {
    if (LIMIT_AKTIVAN_KAD_HIGH) {
        return digitalRead(LIMIT_SVI) == HIGH;
    } else {
        return digitalRead(LIMIT_SVI) == LOW;
    }
}

void zvukPotvrde() {
    tone(BUZZER, 1500, 80);
}

void zvukUpozorenja() {
    tone(BUZZER, 500, 180);
}

int postotakOtvorenosti(int kut) {
    long p = map(kut, KUT_ZATVORENO, KUT_OTVORENO, 0, 100);
    if (p < 0) p = 0;
    if (p > 100) p = 100;
    return (int)p;
}

void zaustaviZbogLimita();

// oled
void prikaziOLED(const char* statusM1, const char* statusM2) {
    if (!oledOK) return;
    if (millis() - zadnjiOLED < OLED_INTERVAL) return;
    zadnjiOLED = millis();

    oled.obrisiSpremnik();

    oled.ispisiTekst(0, 0, "GARAZNA VRATA");
    oled.crtajVodoravnuLiniju(0, 127, 10);

    char redak[24];

    snprintf(redak, sizeof(redak), "RAMPA1: %s %d%%", statusM1, postotakOtvorenosti(kutM1));
    oled.ispisiTekst(0, 16, redak);

    snprintf(redak, sizeof(redak), "RAMPA2: %s %d%%", statusM2, postotakOtvorenosti(kutM2));
    oled.ispisiTekst(0, 28, redak);

    oled.ispisiTekst(0, 42, limitAktivan() ? "LIMIT: AKTIVAN" : "LIMIT: OK");
    oled.ispisiTekst(0, 54, limitAktivan() ? "STOP - LIMIT SWITCH" : "SUSTAV SPREMAN");

    oled.prikaziNaEkranu();
}

void setup() {
    MarServo::pocetnaPostava();

    pinMode(GUMB_M1_ZATVARANJE, INPUT);
    pinMode(GUMB_M1_OTVARANJE, INPUT);
    pinMode(GUMB_M2_ZATVARANJE, INPUT);
    pinMode(GUMB_M2_OTVARANJE, INPUT);

    if (LIMIT_AKTIVAN_KAD_HIGH) {
        pinMode(LIMIT_SVI, INPUT);
    } else {
        pinMode(LIMIT_SVI, INPUT_PULLUP);
    }

    pinMode(LED_CRVENA, OUTPUT);
    pinMode(LED_ZELENA, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    MarServo::postaviKutServo1(kutM1);
    MarServo::postaviKutServo2(kutM2);

    digitalWrite(LED_CRVENA, LOW);
    digitalWrite(LED_ZELENA, LOW);

    // oled
    oledOK = oled.pocetnaPostava();

    if (oledOK) {
        oled.obrisiSpremnik();
        oled.ispisiTekst(0, 0, "SUSTAV POKRENUT");
        oled.ispisiTekst(0, 10, "ARDUINO NANO");
        oled.ispisiTekst(0, 20, "OLED OK");
        oled.prikaziNaEkranu();
        delay(1000);
    }

    zvukPotvrde();
}

void loop() {
    bool m1Zatvaranje = pritisnuto(GUMB_M1_ZATVARANJE);
    bool m1Otvaranje = pritisnuto(GUMB_M1_OTVARANJE);

    bool m2Zatvaranje = pritisnuto(GUMB_M2_ZATVARANJE);
    bool m2Otvaranje = pritisnuto(GUMB_M2_OTVARANJE);

    bool biloKojeZatvaranje = m1Zatvaranje || m2Zatvaranje;
    bool biloKojeOtvaranje = m1Otvaranje || m2Otvaranje;
    bool biloKojaNaredba = biloKojeZatvaranje || biloKojeOtvaranje;

    const char* statusM1 = "MIRUJE";
    const char* statusM2 = "MIRUJE";

    if (limitAktivan()) {
        statusM1 = "STOP";
        statusM2 = "STOP";

        zaustaviZbogLimita();
        prikaziOLED(statusM1, statusM2);

        proslaNaredba = false;
        return;
    }

    if (biloKojaNaredba && !proslaNaredba) {
        zvukPotvrde();
    }
    proslaNaredba = biloKojaNaredba;

    if (biloKojeZatvaranje) {
        digitalWrite(LED_CRVENA, HIGH);
        digitalWrite(LED_ZELENA, LOW);
    } else if (biloKojeOtvaranje) {
        digitalWrite(LED_CRVENA, LOW);
        digitalWrite(LED_ZELENA, HIGH);
    } else {
        digitalWrite(LED_CRVENA, LOW);
        digitalWrite(LED_ZELENA, LOW);
    }

    if (millis() - zadnjePomicanje >= BRZINA) {
        zadnjePomicanje = millis();

        if (m1Otvaranje && !m1Zatvaranje) {
            if (kutM1 < KUT_OTVORENO) {
                kutM1 += KORAK;
                MarServo::postaviKutServo1(kutM1);
                statusM1 = "OTVARA";
            } else {
                statusM1 = "OTVORENO";
            }
        } else if (m1Zatvaranje && !m1Otvaranje) {
            if (kutM1 > KUT_ZATVORENO) {
                kutM1 -= KORAK;
                MarServo::postaviKutServo1(kutM1);
                statusM1 = "ZATVARA";
            } else {
                statusM1 = "ZATVORENO";
            }
        }

        if (m2Otvaranje && !m2Zatvaranje) {
            if (kutM2 < KUT_OTVORENO) {
                kutM2 += KORAK;
                MarServo::postaviKutServo2(kutM2);
                statusM2 = "OTVARA";
            } else {
                statusM2 = "OTVORENO";
            }
        } else if (m2Zatvaranje && !m2Otvaranje) {
            if (kutM2 > KUT_ZATVORENO) {
                kutM2 -= KORAK;
                MarServo::postaviKutServo2(kutM2);
                statusM2 = "ZATVARA";
            } else {
                statusM2 = "ZATVORENO";
            }
        }
    }

    prikaziOLED(statusM1, statusM2);
    delay(20);
}

void zaustaviZbogLimita() {
    MarServo::postaviKutServo1(kutM1);
    MarServo::postaviKutServo2(kutM2);

    digitalWrite(LED_ZELENA, LOW);

    bool blink = (millis() / 250) % 2;
    digitalWrite(LED_CRVENA, blink);

    if (millis() - zadnjiZvukLimit > 700) {
        zadnjiZvukLimit = millis();
        zvukUpozorenja();
    }
}
