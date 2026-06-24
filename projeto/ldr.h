// ldr.h
#ifndef LDR_H
#define LDR_H
#include <stdint.h>

void adc_init();
uint16_t adc_read(uint8_t channel);

#endif
