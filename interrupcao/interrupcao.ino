/*
 * MAPEAMENTO DE HARDWARE:
 * Entradas:
 * - Pino D2 (Porto D, Bit 2) - Interrupção Externa INT0 (Gera ISR na Descida / FALLING)
 * - Pino D3 (Porto D, Bit 3) - Interrupção Externa INT1 (Gera ISR na Subida / RISING)
 * Ambas configuradas com resistores de Pull-Up internos ativados.
 */

#include "interrupcao.h"

volatile unsigned long tempoSubida = 0;
volatile unsigned long tempoDescida = 0;
volatile bool novaSubida = false;
volatile bool novaDescida = false;

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