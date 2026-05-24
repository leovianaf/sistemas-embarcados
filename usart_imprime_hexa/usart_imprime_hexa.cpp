#include "usart_imprime_hexa.h"

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

void serial_print_ulong(unsigned long num) {
  if (num == 0) {
    serial_write_char('0');
    return;
  }
  char texto_invertido[10];
  int i = 0;
  while (num > 0) {
    texto_invertido[i] = (num % 10) + '0';
    num = num / 10;
    i++;
  }
  while (i > 0) {
    i--;
    serial_write_char(texto_invertido[i]);
  }
}

void serial_print_hex(unsigned char num) {
  char hex_chars[] = "0123456789ABCDEF";

  // Imprime o mais significativo (4 bits esquerdos)
  serial_write_char(hex_chars[(num >> 4) & 0x0F]);
  // Imprime o menos significativo (4 bits direitos)
  serial_write_char(hex_chars[num & 0x0F]);
}

void delay_ms(unsigned long ms) {
  for (volatile long d = 0; d < (ms * 3200); d++);
}