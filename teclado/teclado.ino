// Colunas do teclado: PD4 a PD7 configuradas como SAÍDA.
// Linhas do teclado: PB0 a PB3 configuradas como ENTRADA_PULLUP.

#include <util/delay.h>
#include "teclado.h"

const char mapa_teclas[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void configurar_teclado() {
  for (uint8_t col = 4; col <= 7; col++) {
    configPino('D', col, OUTPUT);
    PORTD |= (1 << col);
  }
  for (uint8_t lin = 0; lin <= 3; lin++) {
    configPino('B', lin, INPUT_PULLUP);
  }
}

char ler_teclado() {
  for (uint8_t c = 0; c < 4; c++) {
    PORTD &= ~(1 << (c + 4));
        while ((PINB & 0x0F) != 0x0F) _delay_ms(10);
    _delay_us(10);

    uint8_t linhas = PINB & 0x0F;

    if (linhas != 0x0F) {
      _delay_ms(20); // Debounce de 20ms

      if (linhas == (PINB & 0x0F)) {
        uint8_t l = 0;

        if (!(linhas & 1)) l = 0;
        else if (!(linhas & 2)) l = 1;
        else if (!(linhas & 4)) l = 2;
        else if (!(linhas & 8)) l = 3;

        // Trava para não registrar múltiplos cliques se segurar o botão
        while ((PINB & 0x0F) != 0x0F) _delay_ms(10);

        PORTD |= (1 << (c + 4));
        return mapa_teclas[l][c];
      }
    }
    PORTD |= (1 << (c + 4));
  }
  return 0;
}

void setup() {
  serial_begin(9600);
  configurar_teclado();
}

void loop() {
  char tecla = ler_teclado();

  if (tecla != 0) {
    serial_write_char(tecla);
    serial_write_char('\r');
    serial_write_char('\n');
  }
}