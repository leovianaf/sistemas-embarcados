// servo.cpp
#include "servo.h"
#include <avr/io.h>
#include "config.h"

void servo_init() {
    // Configura os pinos PB1 (D9) e PB2 (D10) como saída no registrador Data Direction
    DDRB |= (1 << PB1) | (1 << PB2);

    // Configura o Timer1 para Fast PWM, TOP = ICR1
    // WGM13=1, WGM12=1, WGM11=1, WGM10=0
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8

    // Calcula o TOP para 50Hz (20ms)
    // F_CPU = 16MHz. Timer Clock = 16M / 8 = 2MHz.
    // 2MHz / 50Hz = 40000. Como começa do 0, ICR1 = 39999.
    ICR1 = 39999;

    // Inicializa no centro
    OCR1A = SERVO_CENTER; // Base
    OCR1B = SERVO_CENTER; // Mount
}

void servo_set_base(uint16_t position) {
    if(position < SERVO_MIN) position = SERVO_MIN;
    if(position > SERVO_MAX) position = SERVO_MAX;
    OCR1A = position;
}

void servo_set_mount(uint16_t position) {
    if(position < SERVO_MIN) position = SERVO_MIN;
    if(position > SERVO_MAX) position = SERVO_MAX;
    OCR1B = position;
}
