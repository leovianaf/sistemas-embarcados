// servo.h
#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>

void servo_init();
void servo_set_base(uint16_t position);
void servo_set_mount(uint16_t position);

#endif
