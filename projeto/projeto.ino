// projeto.ino — Solar Tracker de 1 Eixo (Vertical)
// Baseado em: Henukh et al., "Solar Tracker Design Based on Arduino Nano
//             to Improve Solar Energy Efficiency", Technium Vol.16, 2023.
//
// ADAPTAÇÕES EM RELAÇÃO AO ARTIGO:
//   Artigo original       →  Esta versão
//   ─────────────────────────────────────────
//   3 sensores LDR        →  2 sensores LDR
//   2 eixos (H + V)       →  1 eixo (V apenas)
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
            current_base_pos -= SERVO_STEP;
        } else {
            current_base_pos = SERVO_MIN;
        }
    } else if (verticalError > ERROR_THRESHOLD) {
        if (current_base_pos < SERVO_MAX - SERVO_STEP) {
            current_base_pos += SERVO_STEP;
        } else {
            current_base_pos = SERVO_MAX;
        }
    }
    servo_set_base(current_base_pos);
    
    display_update(topLDR, bottomLDR, verticalError, current_base_pos);

    _delay_ms(50);
}
