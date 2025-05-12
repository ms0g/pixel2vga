#ifndef VGA_H
#define VGA_H

#include <stdint.h>

typedef struct Color {
    float r;
    float g;
    float b;
} Color;

Color vgaClamp(uint8_t r, uint8_t g, uint8_t b);

#endif