#include "usart_imprime_hexa.h"

void setup() {
  serial_begin(9600);

  serial_println("--- REGISTRADORES USART0 (DUMP) ---");

  serial_print("UCSR0A: 0x");
  serial_print_ulong(UCSR0A);
  serial_println("");

  serial_print("UCSR0B: 0x");
  serial_print_ulong(UCSR0B);
  serial_println("");

  serial_print("UCSR0C: 0x");
  serial_print_ulong(UCSR0C);
  serial_println("");

  serial_print("UBRR0H: 0x");
  serial_print_ulong(UBRR0H);
  serial_println("");

  serial_print("UBRR0L: 0x");
  serial_print_ulong(UBRR0L);
  serial_println("");
}

void loop() {}