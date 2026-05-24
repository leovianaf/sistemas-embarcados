#include "usart_version2.h"

void serial_begin(unsigned long baud) {
  unsigned int ubrr = (F_CPU / 16 / baud) - 1;
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}