//  Rastreador Solar de Um Eixo com Controle Embarcado em AVR
//  Baseado em: Henukh et al., "Solar Tracker Design Based on Arduino Nano
//             to Improve Solar Energy Efficiency", Technium Vol.16, 2023.
//
//  O ADC e o PWM do servo são controlados diretamente pelos registradores do
//  ATmega328P. O display utiliza as bibliotecas Wire, Adafruit GFX e SH110X.
//  =========================================================================
//  ADAPTAÇÕES EM RELAÇÃO AO ARTIGO ORIGINAL:
//   Artigo original       →  Esta versão
//  =========================================================================
//   3 sensores LDR        →  2 sensores LDR
//   2 eixos (H + V)       →  1 eixo (V apenas)
//  =========================================================================
//  MAPEAMENTO DE HARDWARE:
//  Sinal PWM do servo     : Pino D9 (PB1/OC1A)
//  Leitura do LDR superior: Pino A0 (PC0/ADC0)
//  Leitura do LDR inferior: Pino A1 (PC1/ADC1)
//  SDA do display         : Pino A4 (PC4/SDA)
//  SCL do display         : Pino A5 (PC5/SCL)
//  * Necessário compartilhar GND entre todos os componentes *
//  =========================================================================
#include "config.h"
#include "ldr.h"
#include "servo.h"
#include "display.h"
#include <util/delay.h>

static uint16_t current_base_pos = SERVO_CENTER;

void setup() {
    adc_init();
    servo_init();
    display_init();
}

void loop() {
    uint16_t topLDR    = adc_read(LDR_TOP_CHANNEL);
    uint16_t bottomLDR = adc_read(LDR_BOTTOM_CHANNEL);

    int verticalError = (int)topLDR - (int)bottomLDR;

    if (verticalError < -ERROR_THRESHOLD) {
        if (current_base_pos > SERVO_MIN + SERVO_STEP) {
            current_base_pos += SERVO_STEP;
        } else {
            current_base_pos = SERVO_MIN;
        }
    } else if (verticalError > ERROR_THRESHOLD) {
        if (current_base_pos < SERVO_MAX - SERVO_STEP) {
            current_base_pos -= SERVO_STEP;
        } else {
            current_base_pos = SERVO_MAX;
        }
    }
    servo_set_base(current_base_pos);
    
    display_update(topLDR, bottomLDR, verticalError, current_base_pos);

    _delay_ms(50);
}
