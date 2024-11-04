#pragma once

#include <stdint.h>

typedef struct color {
    float r, g, b;
} color;

color vgaclamp(uint8_t r, uint8_t g, uint8_t b);
