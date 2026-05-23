/*
 * =========================================================================
 * * * MAPEAMENTO DE HARDWARE (PORTO D):
 * [Importante: Pinos D0 e D1 estão invertidos em relação ao padrão alfabético]
 * - Segmento b    : Pino D0 (Port D, Bit 0 / TX)
 * - Segmento a    : Pino D1 (Port D, Bit 1 / RX)
 * - Segmento c    : Pino D2 (Port D, Bit 2)
 * - Segmento d    : Pino D3 (Port D, Bit 3)
 * - Segmento e    : Pino D4 (Port D, Bit 4)
 * - Segmento f    : Pino D5 (Port D, Bit 5)
 * - Segmento g    : Pino D6 (Port D, Bit 6)
 * - Ponto (DP)    : Pino D7 (Port D, Bit 7) - [Se conectado]
 * - Comum (COM)   : Ligado diretamente ao GND (Catodo Comum)
 * * * * OBSERVAÇÕES TÉCNICAS:
 * - A tabela hexTable segue a ordem DP-g-f-e-d-c-b-a nos bits 7 a 0.
 * - Devido à conexão física, o Bit 0 do código aciona o segmento 'b' no display.
 * - Para Upload: Desconectar fios de D0 e D1 caso ocorra erro de sincronia.
 * =========================================================================
 */

volatile unsigned char *portDDRD  = (volatile unsigned char *) 0x2A;
volatile unsigned char *portDataD = (volatile unsigned char *) 0x2B;

// Tabela de conversão Hexadecimal -> 7 Segmentos (Catodo Comum)
// Ordem dos bits: DP g f e d c b a
const unsigned char hexTable[] = {
  0x3F, // 0: 0011 1111
  0x06, // 1: 0000 0110
  0x5B, // 2: 0101 1011
  0x4F, // 3: 0100 1111
  0x66, // 4: 0110 0110
  0x6D, // 5: 0110 1101
  0x7D, // 6: 0111 1101
  0x07, // 7: 0000 0111
  0x7F, // 8: 0111 1111
  0x6F, // 9: 0110 1111
  0x77, // A: 0111 0111
  0x7C, // B: 0111 1100
  0x39, // C: 0011 1001
  0x5E, // D: 0101 1110
  0x79, // E: 0111 1001
  0x71  // F: 0111 0001
};

void displayHex(unsigned char valor) {
    valor &= 0x0F;
    *portDataD = hexTable[valor];
}

void setup() {
    *portDDRD = 0xFF;
}

void loop() {
    for (int i = 0; i <= 15; i++) {
        displayHex(i);
        for(volatile long d = 0; d < 200000; d++);
    }
}
