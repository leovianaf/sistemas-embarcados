#ifndef USART_IMPRIME_HEXA_H
#define USART_IMPRIME_HEXA_H

#include <Arduino.h>

inline void serial_write_char(char c) {
  while ( !(UCSR0A & (1 << UDRE0)) );
  UDR0 = c;
}

void serial_begin(unsigned long baud);
void serial_print(const char *str);
void serial_println(const char *str);
void serial_print_ulong(unsigned long num);

#endif