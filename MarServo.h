#ifndef MAR_SERVO_H
#define MAR_SERVO_H

#include <avr/io.h>
#include <stdint.h>

class MarServo {
public:
    static void pocetnaPostava();
    static void postaviKutServo1(uint8_t kutStupnjevi);
    static void postaviKutServo2(uint8_t kutStupnjevi);

private:
    static uint16_t kutUTicks(uint8_t kutStupnjevi);
};

#endif
