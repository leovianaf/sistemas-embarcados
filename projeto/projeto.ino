#include "config.h"
#include "ldr.h"
#include "servo.h"
#include "display.h"
#include <util/delay.h> // Função padrão do AVR-Libc, não é função do core Arduino

uint16_t current_base_pos = SERVO_CENTER;

void setup() {
    adc_init();
    servo_init();
    display_init();
}

void loop() {
    // 1. Leitura dos sensores
    uint16_t leftLDR = adc_read(LDR_LEFT_CHANNEL);
    uint16_t rightLDR = adc_read(LDR_RIGHT_CHANNEL);

    // 2. Lógica exata proposta pelo artigo
    int horizontalError = rightLDR - leftLDR;

    if (horizontalError < -ERROR_THRESHOLD) {
        current_base_pos -= 10; // Rotaciona à esquerda gradualmente
    } else if (horizontalError > ERROR_THRESHOLD) {
        current_base_pos += 10; // Rotaciona à direita gradualmente
    }

    // Atualiza PWM em hardware
    servo_set_base(current_base_pos);

    // 3. Atualiza interface gráfica
    display_update(horizontalError, current_base_pos);

    // Taxa de atualização da malha de controle
    _delay_ms(50);
}
