#include "vga.h"
#include <math.h>

#define F(n) ((float)n)

static const float vga_palette[256][3] = {
        /* colors 0-15 */
        {F(0x00), F(0x00), F(0x00)},

        {F(0x00), F(0x00), F(0xAA)},
        {F(0x00), F(0xAA), F(0x00)},
        {F(0x00), F(0xAA), F(0xAA)},
        {F(0xAA), F(0x00), F(0x00)},
        {F(0xAA), F(0x00), F(0xAA)},

        {F(0xAA), F(0x55), F(0x00)},
        {F(0xAA), F(0xAA), F(0xAA)},
        {F(0x55), F(0x55), F(0x55)},
        {F(0x55), F(0x55), F(0xFF)},
        {F(0x55), F(0xFF), F(0x55)},

        {F(0x55), F(0xFF), F(0xFF)},
        {F(0xFF), F(0x55), F(0x55)},
        {F(0xFF), F(0x55), F(0xFF)},
        {F(0xFF), F(0xFF), F(0x55)},
        {F(0xFF), F(0xFF), F(0xFF)},

        /* grayscale 16-31 (non gamma corrected) */
        {F(0x00), F(0x00), F(0x00)},
        {F(0x14), F(0x14), F(0x14)},
        {F(0x20), F(0x20), F(0x20)},
        {F(0x2C), F(0x2C), F(0x2C)},
        {F(0x38), F(0x38), F(0x38)},

        {F(0x45), F(0x45), F(0x45)},
        {F(0x51), F(0x51), F(0x51)},
        {F(0x61), F(0x61), F(0x61)},
        {F(0x71), F(0x71), F(0x71)},
        {F(0x82), F(0x82), F(0x82)},

        {F(0x92), F(0x92), F(0x92)},
        {F(0xA2), F(0xA2), F(0xA2)},
        {F(0xB6), F(0xB6), F(0xB6)},
        {F(0xCB), F(0xCB), F(0xCB)},
        {F(0xE3), F(0xE3), F(0xE3)},

        {F(0xFF), F(0xFF), F(0xFF)},
        // HERE ------> 1
        /* hue mix 32-55 (1) */
        {F(0x00), F(0x00), F(0xFF)},
        {F(0x41), F(0x00), F(0xFF)},
        {F(0x7D), F(0x00), F(0xFF)},
        {F(0xBE), F(0x00), F(0xFF)},

        {F(0xFF), F(0x00), F(0xFF)},
        {F(0xFF), F(0x00), F(0xBE)},
        {F(0xFF), F(0x00), F(0x7D)},
        {F(0xFF), F(0x00), F(0x41)},
        {F(0xFF), F(0x00), F(0x00)},

        {F(0xFF), F(0x41), F(0x00)},
        {F(0xFF), F(0x7D), F(0x00)},
        {F(0xFF), F(0xBE), F(0x00)},
        {F(0xFF), F(0xFF), F(0x00)},
        {F(0xBE), F(0xFF), F(0x00)},

        {F(0x7D), F(0xFF), F(0x00)},
        {F(0x41), F(0xFF), F(0x00)},
        {F(0x00), F(0xFF), F(0x00)},
        {F(0x00), F(0xFF), F(0x41)},
        {F(0x00), F(0xFF), F(0x7D)},

        {F(0x00), F(0xFF), F(0xBE)},
        {F(0x00), F(0xFF), F(0xFF)},
        {F(0x00), F(0xBE), F(0xFF)},
        {F(0x00), F(0x7D), F(0xFF)},
        {F(0x00), F(0x41), F(0xFF)},

        /* hue mix 56-79 (2) */
        {F(0x7D), F(0x7D), F(0xFF)},
        {F(0x9E), F(0x7D), F(0xFF)},
        {F(0xBE), F(0x7D), F(0xFF)},
        {F(0xDF), F(0x7D), F(0xFF)},
        {F(0xFF), F(0x7D), F(0xFF)},

        {F(0xFF), F(0x7D), F(0xDF)},
        {F(0xFF), F(0x7D), F(0xBE)},
        {F(0xFF), F(0x7D), F(0x9E)},
        {F(0xFF), F(0x7D), F(0x7D)},
        {F(0xFF), F(0x9E), F(0x7D)},

        {F(0xFF), F(0xBE), F(0x7D)},
        {F(0xFF), F(0xDF), F(0x7D)},
        {F(0xFF), F(0xFF), F(0x7D)},
        {F(0xDF), F(0xFF), F(0x7D)},
        {F(0xBE), F(0xFF), F(0x7D)},

        {F(0x9E), F(0xFF), F(0x7D)},
        {F(0x7D), F(0xFF), F(0x7D)},
        {F(0x7D), F(0xFF), F(0x9E)},
        {F(0x7D), F(0xFF), F(0xBE)},
        {F(0x7D), F(0xFF), F(0xDF)},

        {F(0x7D), F(0xFF), F(0xFF)},
        {F(0x7D), F(0xDF), F(0xFF)},
        {F(0x7D), F(0xBE), F(0xFF)},
        {F(0x7D), F(0x9E), F(0xFF)},
        /* hue mix 80-103 (3) */
        {F(0xB6), F(0xB6), F(0xFF)},

        {F(0xC7), F(0xB6), F(0xFF)},
        {F(0xDB), F(0xB6), F(0xFF)},
        {F(0xEB), F(0xB6), F(0xFF)},
        {F(0xFF), F(0xB6), F(0xFF)},
        {F(0xFF), F(0xB6), F(0xEB)},

        {F(0xFF), F(0xB6), F(0xDB)},
        {F(0xFF), F(0xB6), F(0xC7)},
        {F(0xFF), F(0xB6), F(0xB6)},
        {F(0xFF), F(0xC7), F(0xB6)},
        {F(0xFF), F(0xDB), F(0xB6)},

        {F(0xFF), F(0xEB), F(0xB6)},
        {F(0xFF), F(0xFF), F(0xB6)},
        {F(0xEB), F(0xFF), F(0xB6)},
        {F(0xDB), F(0xFF), F(0xB6)},
        {F(0xC7), F(0xFF), F(0xB6)},

        {F(0xB6), F(0xFF), F(0xB6)},
        {F(0xB6), F(0xFF), F(0xC7)},
        {F(0xB6), F(0xFF), F(0xDB)},
        {F(0xB6), F(0xFF), F(0xEB)},
        {F(0xB6), F(0xFF), F(0xFF)},

        {F(0xB6), F(0xEB), F(0xFF)},
        {F(0xB6), F(0xDB), F(0xFF)},
        {F(0xB6), F(0xC7), F(0xFF)},
        // HERE ------> 2
        /* hue mix 104-127 (4) dark 1 */
        {F(0x00), F(0x00), F(0x71)},
        {F(0x1C), F(0x00), F(0x71)},
        {F(0x38), F(0x00), F(0x71)},
        {F(0x55), F(0x00), F(0x71)},

        {F(0x71), F(0x00), F(0x71)},
        {F(0x71), F(0x00), F(0x55)},
        {F(0x71), F(0x00), F(0x38)},
        {F(0x71), F(0x00), F(0x1C)},
        {F(0x71), F(0x00), F(0x00)},

        {F(0x71), F(0x1C), F(0x00)},
        {F(0x71), F(0x38), F(0x00)},
        {F(0x71), F(0x55), F(0x00)},
        {F(0x71), F(0x71), F(0x00)},
        {F(0x55), F(0x71), F(0x00)},

        {F(0x38), F(0x71), F(0x00)},
        {F(0x1C), F(0x71), F(0x00)},
        {F(0x00), F(0x71), F(0x00)},
        {F(0x00), F(0x71), F(0x1C)},
        {F(0x00), F(0x71), F(0x38)},

        {F(0x00), F(0x71), F(0x55)},
        {F(0x00), F(0x71), F(0x71)},
        {F(0x00), F(0x55), F(0x71)},
        {F(0x00), F(0x38), F(0x71)},
        {F(0x00), F(0x1C), F(0x71)},

        /* hue mix 56-79 (2) */
        {F(0x38), F(0x38), F(0x71)},
        {F(0x45), F(0x38), F(0x71)},
        {F(0x55), F(0x38), F(0x71)},
        {F(0x61), F(0x38), F(0x71)},
        {F(0x71), F(0x38), F(0x71)},

        {F(0x71), F(0x38), F(0x61)},
        {F(0x71), F(0x38), F(0x55)},
        {F(0x71), F(0x38), F(0x45)},
        {F(0x71), F(0x38), F(0x38)},
        {F(0x71), F(0x45), F(0x38)},

        {F(0x71), F(0x55), F(0x38)},
        {F(0x71), F(0x61), F(0x38)},
        {F(0x71), F(0x71), F(0x38)},
        {F(0x61), F(0x71), F(0x38)},
        {F(0x55), F(0x71), F(0x38)},

        {F(0x45), F(0x71), F(0x38)},
        {F(0x38), F(0x71), F(0x38)},
        {F(0x38), F(0x71), F(0x45)},
        {F(0x38), F(0x71), F(0x55)},
        {F(0x38), F(0x71), F(0x61)},

        {F(0x38), F(0x71), F(0x71)},
        {F(0x38), F(0x61), F(0x71)},
        {F(0x38), F(0x55), F(0x71)},
        {F(0x38), F(0x45), F(0x71)},
        /* hue mix 80-103 (3) */
        {F(0x51), F(0x51), F(0x71)},

        {F(0x59), F(0x51), F(0x71)},
        {F(0x61), F(0x51), F(0x71)},
        {F(0x69), F(0x51), F(0x71)},
        {F(0x71), F(0x51), F(0x71)},
        {F(0x71), F(0x51), F(0x69)},

        {F(0x71), F(0x51), F(0x61)},
        {F(0x71), F(0x51), F(0x59)},
        {F(0x71), F(0x51), F(0x51)},
        {F(0x71), F(0x59), F(0x51)},
        {F(0x71), F(0x61), F(0x51)},

        {F(0x71), F(0x69), F(0x51)},
        {F(0x71), F(0x71), F(0x51)},
        {F(0x69), F(0x71), F(0x51)},
        {F(0x61), F(0x71), F(0x51)},
        {F(0x59), F(0x71), F(0x51)},

        {F(0x51), F(0x71), F(0x51)},
        {F(0x51), F(0x71), F(0x59)},
        {F(0x51), F(0x71), F(0x61)},
        {F(0x51), F(0x71), F(0x69)},
        {F(0x51), F(0x71), F(0x71)},

        {F(0x51), F(0x69), F(0x71)},
        {F(0x51), F(0x61), F(0x71)},
        {F(0x51), F(0x59), F(0x71)},
        // HERE ------> 3
        /* hue mix 104-127 (4) dark 1 */
        {F(0x00), F(0x00), F(0x41)},
        {F(0x10), F(0x00), F(0x41)},
        {F(0x20), F(0x00), F(0x41)},
        {F(0x30), F(0x00), F(0x41)},

        {F(0x41), F(0x00), F(0x41)},
        {F(0x41), F(0x00), F(0x30)},
        {F(0x41), F(0x00), F(0x20)},
        {F(0x41), F(0x00), F(0x10)},
        {F(0x41), F(0x00), F(0x00)},

        {F(0x41), F(0x10), F(0x00)},
        {F(0x41), F(0x20), F(0x00)},
        {F(0x41), F(0x30), F(0x00)},
        {F(0x41), F(0x41), F(0x00)},
        {F(0x30), F(0x41), F(0x00)},

        {F(0x20), F(0x41), F(0x00)},
        {F(0x10), F(0x41), F(0x00)},
        {F(0x00), F(0x41), F(0x00)},
        {F(0x00), F(0x41), F(0x10)},
        {F(0x00), F(0x41), F(0x20)},

        {F(0x00), F(0x41), F(0x30)},
        {F(0x00), F(0x41), F(0x41)},
        {F(0x00), F(0x30), F(0x41)},
        {F(0x00), F(0x20), F(0x41)},
        {F(0x00), F(0x10), F(0x41)},

        /* hue mix 56-79 (2) */
        {F(0x20), F(0x20), F(0x41)},
        {F(0x28), F(0x20), F(0x41)},
        {F(0x30), F(0x20), F(0x41)},
        {F(0x3C), F(0x20), F(0x41)},
        {F(0x41), F(0x20), F(0x41)},

        {F(0x41), F(0x20), F(0x3C)},
        {F(0x41), F(0x20), F(0x30)},
        {F(0x41), F(0x20), F(0x28)},
        {F(0x41), F(0x20), F(0x20)},
        {F(0x41), F(0x28), F(0x20)},

        {F(0x41), F(0x30), F(0x20)},
        {F(0x41), F(0x3C), F(0x20)},
        {F(0x41), F(0x41), F(0x20)},
        {F(0x3C), F(0x41), F(0x20)},
        {F(0x30), F(0x41), F(0x20)},

        {F(0x28), F(0x41), F(0x20)},
        {F(0x20), F(0x41), F(0x20)},
        {F(0x20), F(0x41), F(0x28)},
        {F(0x20), F(0x41), F(0x30)},
        {F(0x20), F(0x41), F(0x3C)},

        {F(0x20), F(0x41), F(0x41)},
        {F(0x20), F(0x3C), F(0x41)},
        {F(0x20), F(0x30), F(0x41)},
        {F(0x20), F(0x28), F(0x41)},
        /* hue mix 80-103 (3) */
        {F(0x2C), F(0x2C), F(0x41)},

        {F(0x30), F(0x2C), F(0x41)},
        {F(0x34), F(0x2C), F(0x41)},
        {F(0x3C), F(0x2C), F(0x41)},
        {F(0x41), F(0x2C), F(0x41)},
        {F(0x41), F(0x2C), F(0x3C)},

        {F(0x41), F(0x2C), F(0x34)},
        {F(0x41), F(0x2C), F(0x30)},
        {F(0x41), F(0x2C), F(0x2C)},
        {F(0x41), F(0x30), F(0x2C)},
        {F(0x41), F(0x34), F(0x2C)},

        {F(0x41), F(0x3C), F(0x2C)},
        {F(0x41), F(0x41), F(0x2C)},
        {F(0x3C), F(0x41), F(0x2C)},
        {F(0x34), F(0x41), F(0x2C)},
        {F(0x30), F(0x41), F(0x2C)},

        {F(0x2C), F(0x41), F(0x2C)},
        {F(0x2C), F(0x41), F(0x30)},
        {F(0x2C), F(0x41), F(0x34)},
        {F(0x2C), F(0x41), F(0x3C)},
        {F(0x2C), F(0x41), F(0x41)},

        {F(0x2C), F(0x3C), F(0x41)},
        {F(0x2C), F(0x34), F(0x41)},
        {F(0x2C), F(0x30), F(0x41)},

        /* all black */
        {F(0),    F(0),    F(0)},
        {F(0),    F(0),    F(0)},
        {F(0),    F(0),    F(0)},
        {F(0),    F(0),    F(0)},

        {F(0),    F(0),    F(0)},
        {F(0),    F(0),    F(0)},
        {F(0),    F(0),    F(0)},
        {F(0),    F(0),    F(0)},
};

color vgaclamp(uint8_t r, uint8_t g, uint8_t b) {
    float rd = (float) r;
    float gd = (float) g;
    float bd = (float) b;

    float closest = +INFINITY;
    
    int index;
    for (int i = 0; i < 248; i++) {
        const float* sample = vga_palette[i];
        
        float rs = sample[0];
        float gs = sample[1];
        float bs = sample[2];
        
        float dst = sqrtf(powf(rs - rd, 2) + powf(gs - gd, 2) + powf(bs - bd, 2));
        
        if (dst < closest) {
            closest = dst;
            index = i;
        }
    }
   
    return (color){vga_palette[index][0], vga_palette[index][1], vga_palette[index][2]};
}