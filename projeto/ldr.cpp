// ldr.cpp
#include "ldr.h"
#include <avr/io.h>

void adc_init() {
    // Configura a tensão de referência para AVCC com capacitor no pino AREF
    ADMUX = (1 << REFS0);

    // Habilita o ADC (ADEN) e define o prescaler para 128 (ADPS2:0)
    // 16MHz / 128 = 125kHz, que está dentro do ideal de 50-200kHz para o ADC
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
    // Limpa os bits de seleção de canal (MUX3:0) e define o novo canal
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // Inicia a conversão (Start Conversion)
    ADCSRA |= (1 << ADSC);

    // Aguarda a conversão terminar (o bit ADSC se torna 0)
    while (ADCSRA & (1 << ADSC));

    // Retorna o valor lido (deve ser lido o ADCL primeiro, o compilador C cuida disso ao ler ADC)
    return ADC;
}
