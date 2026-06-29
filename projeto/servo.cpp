#include "servo.h"
#include <avr/io.h>
#include "config.h"

void servo_init() {
    DDRB |= (1 << PB1);

    TCCR1A = (1 << COM1A1) | (1 << WGM11);

    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    ICR1 = 39999;

    OCR1A = SERVO_CENTER;
}

void servo_set_base(uint16_t position) {
    if (position < SERVO_MIN) position = SERVO_MIN;
    if (position > SERVO_MAX) position = SERVO_MAX;
    OCR1A = position;
}
