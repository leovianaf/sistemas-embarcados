#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_init();
void display_update(uint16_t ldr_top, uint16_t ldr_bottom, int error, uint16_t base_pos);

#endif
