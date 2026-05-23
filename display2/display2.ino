/*
 * =========================================================================
 * * * MAPEAMENTO DE HARDWARE:
 * * [DISPLAY 1 - NIBBLE BAIXA / DIREITA] - Conectado ao Porto D
 * - Segmentos a-g : Pinos D0 a D6 (Port D, Bits 0 a 6)
 * - Comum (COM)   : Ligado diretamente ao GND (Catodo Comum)
 * NOTA: Desconectar D0/D1 durante o Upload se houver erro de sincronia.
 * * [DISPLAY 2 - NIBBLE ALTA / ESQUERDA] - Conectado aos Portos C e B
 * - Segmentos a-f : Pinos A0 a A5 (Port C, Bits 0 a 5)
 * - Segmento g    : Pino D13 (Port B, Bit 5 / LED Interno L)
 * - Comum (COM)   : Ligado diretamente ao GND (Catodo Comum)

 * * * LÓGICA DE EXIBIÇÃO:
 * - O código fatia o byte recebido em duas partes de 4 bits (Nibbles).
 * - Nibble Alta (4 bits da esquerda) -> Display 2 (Porto C/B).
 * - Nibble Baixa (4 bits da direita) -> Display 1 (Porto D).
 * =========================================================================
 */

volatile unsigned char *portDDRD  = (volatile unsigned char *) 0x2A;
volatile unsigned char *portDataD = (volatile unsigned char *) 0x2B;
volatile unsigned char *portDDRC  = (volatile unsigned char *) 0x27;
volatile unsigned char *portDataC = (volatile unsigned char *) 0x28;
volatile unsigned char *portDDRB  = (volatile unsigned char *) 0x24;
volatile unsigned char *portDataB = (volatile unsigned char *) 0x25;

const unsigned char hexTable[] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
  0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

void setup() {
  *portDDRD = 0x7F;
  *portDDRC = 0x3F;
  *portDDRB |= 0x20;
}

void mostrarNoDisplay1(unsigned char valor) {
  *portDataD = hexTable[valor & 0x0F];
}

void mostrarNoDisplay2(unsigned char valor) {
  unsigned char bits = hexTable[valor & 0x0F];

  *portDataC = (bits & 0x3F);

  if (bits & 0x40) {
    *portDataB |= 0x20;
  } else {
    *portDataB &= ~0x20;
  }
}

void displayByte(unsigned char byteValor) {
  unsigned char nibbleAlta = (byteValor >> 4) & 0x0F;
  unsigned char nibbleBaixa = byteValor & 0x0F;

  mostrarNoDisplay2(nibbleAlta);
  mostrarNoDisplay1(nibbleBaixa);
}

void loop() {
  displayByte(0x87);
  for(volatile long d = 0; d < 1000000; d++);

  displayByte(0x08);
  for(volatile long d = 0; d < 1000000; d++);
}