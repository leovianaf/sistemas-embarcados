#include <avr/interrupt.h>
#include "bare_serial.h"

#define BUF_SIZE 64

volatile char rx_buf[BUF_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

volatile char tx_buf[BUF_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;

volatile bool aguardando_retorno = false;

void enviar_caractere(char c) {
  uint8_t next_head = (tx_head + 1) % BUF_SIZE;

  while (next_head == tx_tail);

  tx_buf[tx_head] = c;
  tx_head = next_head;

  UCSR0B |= (1 << UDRIE0);
}

void setup() {
  cli();

  serial_begin(9600);

  // Adiciona as interrupções de Recepção (RXCIE0) ao controle da UART
  UCSR0B |= (1 << RXCIE0);

  sei();
}

void loop() {
  if (aguardando_retorno) {
    cli();

    while (rx_tail != rx_head) {
      char c = rx_buf[rx_tail];
      rx_tail = (rx_tail + 1) % BUF_SIZE;
      sei();
      enviar_caractere(c);
      cli();
    }

    sei();
    enviar_caractere('\r');
    enviar_caractere('\n');
    aguardando_retorno = false;
  }
}

ISR(USART_RX_vect) {
  char c = UDR0;

  if (c == '\r') {
    aguardando_retorno = true;
  } else if (c != '\n') {
    rx_buf[rx_head] = c;
    rx_head = (rx_head + 1) % BUF_SIZE;

    if (rx_head == rx_tail) {
      rx_tail = (rx_tail + 1) % BUF_SIZE;
    }
  }
}

ISR(USART_UDRE_vect) {
  if (tx_head != tx_tail) {
    UDR0 = tx_buf[tx_tail];
    tx_tail = (tx_tail + 1) % BUF_SIZE;
  } else {
    UCSR0B &= ~(1 << UDRE0);
  }
}
