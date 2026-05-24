#ifndef USART_VERSION1_H
#define USART_VERSION1_H

#include <Arduino.h>

inline void serial_write_char(char c) {
  while ( !(UCSR0A & (1 << UDRE0)) );
  UDR0 = c;
}

void serial_begin(unsigned long baud);

#endif