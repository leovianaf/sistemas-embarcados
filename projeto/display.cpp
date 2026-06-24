// display.cpp
#include "display.h"
#include <avr/io.h>
#include <Wire.h> // Abstração I2C permitida para inicializar o buffer do display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void display_init() {
    // Inicialização bare metal do barramento I2C (TWI)
    // SCL clock = F_CPU / (16 + 2 * TWBR * Prescaler)
    // Para 100kHz com F_CPU 16MHz e prescaler 1: TWBR = 72
    TWSR = 0x00; // Prescaler = 1
    TWBR = 72;   // Bit rate
    TWCR = (1 << TWEN); // Habilita TWI

    // Inicializa display via biblioteca gráfica
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);
}

void display_update(int error, uint16_t base_pos) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Solar Tracker Native");

    display.setCursor(0, 20);
    display.print("Erro H: ");
    display.println(error);

    display.setCursor(0, 40);
    display.print("PWM Base: ");
    display.println(base_pos);

    display.display();
}
