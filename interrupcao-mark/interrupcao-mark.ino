/*
 * MAPEAMENTO DE HARDWARE (Bare Metal - ATmega328P):
 * - PD2 (INT0) - Interrupção Externa (Gera ISR na Descida / FALLING)
 * - PD3 (INT1) - Interrupção Externa (Gera ISR na Subida / RISING)
 * Ambos com Pull-Up interno ativado.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL 
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD) - 1)

volatile uint8_t flagDescida = 0;
volatile uint8_t flagSubida = 0;

void serial_init() {
    UBRR0H = (BRC >> 8);
    UBRR0L = BRC;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void serial_print(const char* str) {
    while (*str) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = *str++; 
    }
}
// --------------------------------------------------

int main(void) {
    DDRD &= ~((1 << DDD2) | (1 << DDD3));
    
    PORTD |= (1 << PORTD2) | (1 << PORTD3);

    EICRA |= (1 << ISC01) | (1 << ISC11) | (1 << ISC10);
    EICRA &= ~(1 << ISC00);

    EIMSK |= (1 << INT0) | (1 << INT1);

    serial_init();
    sei(); 

    serial_print("Sistema pronto. Aguardando transicoes...\r\n");

    while (1) {
        if (flagDescida) {
            cli();
            flagDescida = 0;
            sei();
            serial_print("Transição de DESCIDA no pino PD2 detectada!\r\n");
        }

        if (flagSubida) {
            cli();
            flagSubida = 0;
            sei();
            serial_print("Transição de SUBIDA no pino PD3 detectada!\r\n");
        }
    }
    
    return 0;
}

ISR(INT0_vect) {
    flagDescida = 1;
}

ISR(INT1_vect) {
    flagSubida = 1;
}