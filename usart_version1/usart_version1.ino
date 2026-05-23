#include <avr/interrupt.h>
#include "bare_serial.h"

void setup() {
  cli();

  serial_begin(9600);
  UCSR0B |= (1 << RXCIE0);

  sei();
}

void loop() {
}

ISR(USART_RX_vect) {
  char c = UDR0;
  serial_write_char(c);
}
