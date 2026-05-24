#include "debounce.h"

void serial_begin(unsigned long baud) {
  unsigned int ubrr = (F_CPU / 16 / baud) - 1;
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void serial_print(const char *str) {
  while (*str != '\0') {
    serial_write_char(*str);
    str++;
  }
}

void serial_println(const char *str) {
  serial_print(str);
  serial_write_char('\r');
  serial_write_char('\n');
}

void delay_ms(unsigned long ms) {
  for (volatile long d = 0; d < (ms * 3200); d++);
}