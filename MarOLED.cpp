#include "MarOLED.h"
#include "MarFont.h"
#include <avr/pgmspace.h>
#include <string.h>

#define SSD1306_KONTROLA_NAREDBA 0x00
#define SSD1306_KONTROLA_PODATAK 0x40

MarOLED::MarOLED(uint8_t adresaUredjaja) : adresa(adresaUredjaja) {
    obrisiSpremnik();
}

bool MarOLED::posaljiNaredbu(uint8_t naredba) {
    uint8_t paket[2] = { SSD1306_KONTROLA_NAREDBA, naredba };
    return MarI2C::posaljiPaket(adresa, paket, 2);
}

bool MarOLED::pocetnaPostava() {
    MarI2C::pocetnaPostava();

    bool ok = true;
    ok &= posaljiNaredbu(0xAE);
    ok &= posaljiNaredbu(0xD5); ok &= posaljiNaredbu(0x80);
    ok &= posaljiNaredbu(0xA8); ok &= posaljiNaredbu(0x3F);
    ok &= posaljiNaredbu(0xD3); ok &= posaljiNaredbu(0x00);
    ok &= posaljiNaredbu(0x40);
    ok &= posaljiNaredbu(0x8D); ok &= posaljiNaredbu(0x14);
    ok &= posaljiNaredbu(0x20); ok &= posaljiNaredbu(0x00);
    ok &= posaljiNaredbu(0xA1);
    ok &= posaljiNaredbu(0xC8);
    ok &= posaljiNaredbu(0xDA); ok &= posaljiNaredbu(0x12);
    ok &= posaljiNaredbu(0x81); ok &= posaljiNaredbu(0xCF);
    ok &= posaljiNaredbu(0xD9); ok &= posaljiNaredbu(0xF1);
    ok &= posaljiNaredbu(0xDB); ok &= posaljiNaredbu(0x40);
    ok &= posaljiNaredbu(0xA4);
    ok &= posaljiNaredbu(0xA6);
    ok &= posaljiNaredbu(0xAF);

    return ok;
}

void MarOLED::obrisiSpremnik() {
    memset(spremnik, 0x00, sizeof(spremnik));
}

void MarOLED::postaviPiksel(int16_t x, int16_t y, bool upaljen) {
    if (x < 0 || x >= MAROLED_SIRINA || y < 0 || y >= MAROLED_VISINA) return;

    uint16_t indeks = x + (y / 8) * MAROLED_SIRINA;
    uint8_t bitMaska = 1 << (y % 8);

    if (upaljen) {
        spremnik[indeks] |= bitMaska;
    } else {
        spremnik[indeks] &= ~bitMaska;
    }
}

void MarOLED::crtajVodoravnuLiniju(int16_t x0, int16_t x1, int16_t y) {
    for (int16_t x = x0; x <= x1; x++) {
        postaviPiksel(x, y, true);
    }
}

void MarOLED::ispisiTekst(int16_t x, int16_t y, const char* tekst) {
    int16_t kursorX = x;

    while (*tekst) {
        char c = *tekst++;

        if (c < FONT_5x7_FIRST_CHAR || c >= (FONT_5x7_FIRST_CHAR + FONT_5x7_COUNT)) {
            c = ' ';
        }
        uint8_t indeksZnaka = c - FONT_5x7_FIRST_CHAR;

        for (uint8_t redak = 0; redak < 7; redak++) {
            uint8_t bitoviRetka = pgm_read_byte(&FONT_5x7[indeksZnaka][redak]);
            for (uint8_t stupac = 0; stupac < 5; stupac++) {
                bool upaljen = (bitoviRetka >> (4 - stupac)) & 0x01;
                postaviPiksel(kursorX + stupac, y + redak, upaljen);
            }
        }
        kursorX += 6;
    }
}

void MarOLED::prikaziNaEkranu() {
    posaljiNaredbu(0x21); posaljiNaredbu(0);   posaljiNaredbu(127);
    posaljiNaredbu(0x22); posaljiNaredbu(0);   posaljiNaredbu(7);

    const uint8_t VELICINA_PAKETA = 32;
    uint8_t buffer[VELICINA_PAKETA + 1];
    buffer[0] = SSD1306_KONTROLA_PODATAK;

    for (uint16_t i = 0; i < sizeof(spremnik); i += VELICINA_PAKETA) {
        memcpy(&buffer[1], &spremnik[i], VELICINA_PAKETA);
        MarI2C::posaljiPaket(adresa, buffer, VELICINA_PAKETA + 1);
    }
}
