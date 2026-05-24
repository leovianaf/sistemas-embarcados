#include "teclado.h"

void serial_begin(unsigned long baud) {
  unsigned int ubrr = (F_CPU / 16 / baud) - 1;
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void configPino(char porta, uint8_t pino, uint8_t modo) {
  static volatile uint8_t* const ddrs[]  = { &DDRB,  &DDRC,  &DDRD  };
  static volatile uint8_t* const ports[] = { &PORTB, &PORTC, &PORTD };

  uint8_t i = porta - 'B';
  uint8_t estado = pausaInterrupcoes();

  if (modo == OUTPUT) {
    *ddrs[i] |= (1 << pino);
  } else if (modo == INPUT) {
    *ddrs[i] &= ~(1 << pino);
    *ports[i] &= ~(1 << pino);
  } else if (modo == INPUT_PULLUP) {
    *ddrs[i] &= ~(1 << pino);
    *ports[i] |= (1 << pino);
  }

  retomaInterrupcoes(estado);
}