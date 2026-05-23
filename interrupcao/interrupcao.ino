/*
 * MAPEAMENTO DE HARDWARE:
 * Entradas:
 * - Pino D2 (Porto D, Bit 2) - Interrupção Externa INT0 (Gera ISR na Descida / FALLING)
 * - Pino D3 (Porto D, Bit 3) - Interrupção Externa INT1 (Gera ISR na Subida / RISING)
 * Ambas configuradas com resistores de Pull-Up internos ativados.
 */

#include "bare_serial.h"

volatile unsigned long tempoSubida = 0;
volatile unsigned long tempoDescida = 0;
volatile bool novaSubida = false;
volatile bool novaDescida = false;

inline uint8_t pausaInterrupcoes() {
  uint8_t estadoAntigo = SREG;
  cli();
  return estadoAntigo;
}

inline void retomaInterrupcoes(uint8_t estadoAntigo) {
  SREG = estadoAntigo;
}

void configPino(char porta, uint8_t pino, uint8_t modo) {
  static volatile uint8_t* const ddrs[]  = { &DDRB,  &DDRC,  &DDRD  };
  static volatile uint8_t* const ports[] = { &PORTB, &PORTC, &PORTD };

  uint8_t i = porta - 'B';
  uint8_t estado = pausaInterrupcoes();

  if (modo == OUTPUT) {
    *ddrs[i] |= (1 << pino);
  } else if (modo == INPUT) {
    *ddrs[i] &= ~(1 << pino);
    *ports[i] &= ~(1 << pino);
  } else if (modo == INPUT_PULLUP) {
    *ddrs[i] &= ~(1 << pino);
    *ports[i] |= (1 << pino);
  }

  retomaInterrupcoes(estado);
}

void isrDescida() {
  tempoDescida = millis();
  novaDescida = true;
}

void isrSubida() {
  tempoSubida = millis();
  novaSubida = true;
}

void setup() {
  serial_begin(9600);

  configPino('D', 2, INPUT_PULLUP);
  configPino('D', 3, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(2), isrDescida, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), isrSubida, RISING);

  serial_println("Aguardando transicoes para calcular o tempo...");
}

void loop() {
  if (novaSubida && novaDescida) {

    uint8_t estado = pausaInterrupcoes();

    unsigned long tSubida = tempoSubida;
    unsigned long tDescida = tempoDescida;
    novaSubida = false;
    novaDescida = false;

    retomaInterrupcoes(estado);

    unsigned long tempoDecorrido;

    if (tSubida > tDescida) {
      tempoDecorrido = tSubida - tDescida;
    } else {
      tempoDecorrido = tDescida - tSubida;
    }

    serial_print("Tempo entre as transicoes: ");
    serial_print_ulong(tempoDecorrido);
    serial_println(" milissegundos.");
  }
}