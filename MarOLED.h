#ifndef MAR_OLED_H
#define MAR_OLED_H

#include <stdint.h>
#include "MarI2C.h"

#define MAROLED_SIRINA   128
#define MAROLED_VISINA   64
#define MAROLED_STRANICE (MAROLED_VISINA / 8)

class MarOLED {
public:
    MarOLED(uint8_t adresaUredjaja);

    bool pocetnaPostava();
    void obrisiSpremnik();
    void postaviPiksel(int16_t x, int16_t y, bool upaljen);
    void crtajVodoravnuLiniju(int16_t x0, int16_t x1, int16_t y);
    void ispisiTekst(int16_t x, int16_t y, const char* tekst);
    void prikaziNaEkranu();

private:
    uint8_t adresa;
    uint8_t spremnik[MAROLED_SIRINA * MAROLED_STRANICE];

    bool posaljiNaredbu(uint8_t naredba);
};

#endif
