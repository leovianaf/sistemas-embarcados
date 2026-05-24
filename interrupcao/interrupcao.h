#ifndef INTERRUPCAO_H
#define INTERRUPCAO_H

#include <Arduino.h>

inline uint8_t pausaInterrupcoes() {
  uint8_t estadoAntigo = SREG;
  cli();
  return estadoAntigo;
}

inline void retomaInterrupcoes(uint8_t estadoAntigo) {
  SREG = estadoAntigo;
}

inline void serial_write_char(char c) {
  while ( !(UCSR0A & (1 << UDRE0)) );
  UDR0 = c;
}

void serial_begin(unsigned long baud);
void serial_print(const char *str);
void serial_println(const char *str);
void serial_print_ulong(unsigned long num);
void configPino(char porta, uint8_t pino, uint8_t modo);

#endif