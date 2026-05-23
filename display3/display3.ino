/*
 * =========================================================================
 * * MAPEAMENTO DE HARDWARE:
 * * [DISPLAY 1 - UNIDADE (DIREITA)] - Porto D
 * - Segmentos a-g : Pinos D0 a D6 (Port D, Bits 0 a 6)
 * - Ponto (DP)    : Pino D7 (Port D, Bit 7)
 * - Comum (COM)   : Ligado diretamente ao GND

 * * [DISPLAY 2 - DEZENA (ESQUERDA)] - Porto C e Porto B
 * - Segmentos a-f : Pinos A0 a A5 (Port C, Bits 0 a 5)
 * - Segmento g    : Pino D13 (Port B, Bit 5)
 * - Ponto (DP)    : Pino D12 (Port B, Bit 4)
 * - Comum (COM)   : Ligado diretamente ao GND

 * * LÓGICA DE OVERFLOW:
 * - Se o valor for > 99, os displays exibirão "F.F." (Overflow).
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
  *portDDRD = 0xFF;
  *portDDRC = 0x3F;
  *portDDRB |= 0x30;
}

void mostrarNoDisplay1(unsigned char valor, bool ponto) {
  unsigned char bits = hexTable[valor & 0x0F];
  if (ponto) bits |= 0x80;
  *portDataD = bits;
}

void mostrarNoDisplay2(unsigned char valor, bool ponto) {
  unsigned char bits = hexTable[valor & 0x0F];

  *portDataC = (bits & 0x3F);

  if (bits & 0x40) *portDataB |= 0x20;
  else *portDataB &= ~0x20;

  if (ponto) *portDataB |= 0x10;
  else *portDataB &= ~0x10;
}

void displayDecimal(unsigned char valor) {
  unsigned char d1, d2;
  bool overflow = (valor > 99);

  if (overflow) {
    d1 = 0x0F;
    d2 = 0x0F;
  } else {
    d1 = valor / 10;
    d2 = valor % 10;
  }

  mostrarNoDisplay2(d1, overflow);
  mostrarNoDisplay1(d2, overflow);
}

void loop() {
  displayDecimal(42);
  for(volatile long d = 0; d < 1000000; d++);

  displayDecimal(105);
  for(volatile long d = 0; d < 1000000; d++);
}
