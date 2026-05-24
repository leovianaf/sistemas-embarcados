/*
 * MAPEAMENTO DE HARDWARE:
 * Entrada: Pino A0 (Porto C, Bit 0) com resistor de Pull-Up interno ativado.
 * Saída: Comunicação Serial UART0 via biblioteca debounce.h.
 */

#include "debounce.h"

volatile unsigned char *portDDRC  = (volatile unsigned char *) 0x27;
volatile unsigned char *portPINC  = (volatile unsigned char *) 0x26;
volatile unsigned char *portPORTC = (volatile unsigned char *) 0x28;

static int contador = 0;

void setup() {
  serial_begin(9600);
  *portDDRC &= ~0x01;
  *portPORTC |= 0x01;
}

void loop() {
  unsigned char estadoAtual = (*portPINC & 0x01);

  delay_ms(1);

  if (estadoAtual == 0) {
    contador++;

    if (contador == 100){
      contador = 0;
      serial_println(">>> ESTÁVEL! Permaneceu em 0 por 100ms.");
    }
  }
  else {
    contador = 0;
  }
}