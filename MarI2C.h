#ifndef MAR_I2C_H
#define MAR_I2C_H

#include <avr/io.h>
#include <stdint.h>

class MarI2C {
public:
    static void pocetnaPostava();
    static bool posaljiPaket(uint8_t adresaUredjaja, const uint8_t* podaci, uint16_t duljina);

private:
    static void start();
    static void stop();
    static bool posaljiBajt(uint8_t bajt);
};

#endif
