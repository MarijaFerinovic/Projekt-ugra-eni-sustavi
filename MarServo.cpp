#include "MarServo.h"

static const uint16_t TICKS_PO_PERIODI = 40000;
static const uint16_t TICKS_MIN = 2000;
static const uint16_t TICKS_MAX = 4000;

void MarServo::pocetnaPostava() {
    DDRB |= (1 << DDB1);
    DDRB |= (1 << DDB2);

    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    ICR1 = TICKS_PO_PERIODI;

    OCR1A = TICKS_MIN;
    OCR1B = TICKS_MIN;
}

uint16_t MarServo::kutUTicks(uint8_t kutStupnjevi) {
    if (kutStupnjevi > 180) kutStupnjevi = 180;

    uint32_t raspon = (uint32_t)(TICKS_MAX - TICKS_MIN) * kutStupnjevi;
    return TICKS_MIN + (uint16_t)(raspon / 180);
}

void MarServo::postaviKutServo1(uint8_t kutStupnjevi) {
    OCR1B = kutUTicks(kutStupnjevi);
}

void MarServo::postaviKutServo2(uint8_t kutStupnjevi) {
    OCR1A = kutUTicks(kutStupnjevi);
}
