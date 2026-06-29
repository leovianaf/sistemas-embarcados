#include "display.h"
#include <avr/io.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64

static Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void display_init() {
    TWSR = 0x00;
    TWBR = 72;
    TWCR = (1 << TWEN);

    display.begin(0x3C, true);
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
}

void display_update(uint16_t ldr_top, uint16_t ldr_bottom, int error, uint16_t base_pos) {
    display.clearDisplay();
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println(F("Solar Tracker 1X"));

    display.setCursor(0, 10);
    display.println(F("----------------"));

    display.setCursor(0, 20);
    display.print(F("LDR Sup: "));
    display.println(ldr_top);

    display.setCursor(0, 30);
    display.print(F("LDR Inf: "));
    display.println(ldr_bottom);

    display.setCursor(0, 42);
    display.print(F("Erro V:  "));
    display.println(error);

    display.setCursor(0, 52);
    display.print(F("PWM:     "));
    display.println(base_pos);

    display.display();
}
