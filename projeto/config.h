#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>

// Pinos ADC para os LDRs (PC0 e PC1)
#define LDR_LEFT_CHANNEL  0
#define LDR_RIGHT_CHANNEL 1

// Limites de erro baseados no artigo
#define ERROR_THRESHOLD 50

// Limites físicos dos Servos SG90 (valores em ticks do Timer1)
// 1ms a 2ms em um timer com prescaler 8 a 16MHz = 2000 a 4000 ticks
#define SERVO_MIN 1000 // Aprox 0.5ms (0 graus)
#define SERVO_MAX 4800 // Aprox 2.4ms (180 graus)
#define SERVO_CENTER 3000 // 1.5ms (90 graus)

#endif
