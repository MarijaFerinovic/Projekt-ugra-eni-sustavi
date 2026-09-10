#include "MarI2C.h"

#define TWI_START_POSLAN        0x08
#define TWI_REPEATED_START      0x10
#define TWI_ADRESA_W_ACK        0x18
#define TWI_PODATAK_ACK         0x28

void MarI2C::pocetnaPostava() {
    TWSR = 0x00;
    TWBR = 12;

    PORTC |= (1 << PORTC4) | (1 << PORTC5);
}

void MarI2C::start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {
    }
}

void MarI2C::stop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

bool MarI2C::posaljiBajt(uint8_t bajt) {
    TWDR = bajt;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {
    }
    uint8_t status = TWSR & 0xF8;
    return (status == TWI_ADRESA_W_ACK) || (status == TWI_PODATAK_ACK);
}

bool MarI2C::posaljiPaket(uint8_t adresaUredjaja, const uint8_t* podaci, uint16_t duljina) {
    start();

    uint8_t adresaZaSlanje = (adresaUredjaja << 1) | 0x00;
    if (!posaljiBajt(adresaZaSlanje)) {
        stop();
        return false;
    }

    for (uint16_t i = 0; i < duljina; i++) {
        if (!posaljiBajt(podaci[i])) {
            stop();
            return false;
        }
    }

    stop();
    return true;
}
