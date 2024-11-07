#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum ImageFormat {
    IMAGE_FORMAT_JPG,
    IMAGE_FORMAT_PNG,
    IMAGE_FORMAT_BMP,
    IMAGE_FORMAT_TGA,
    IMAGE_FORMAT_RAW
} ImageFormat;

typedef enum ImageType {
    IMAGE_TYPE_IN,
    IMAGE_TYPE_OUT
} ImageType;

typedef struct Image {
    int width;
    int height;
    int channels;
    size_t size;
    ImageFormat format;
    ImageType type;
    uint8_t* pData;
    bool (*load)(struct Image*, const char*);
    bool (*new)(struct Image*, int, int, int, ImageFormat);
    void (*write)(struct Image*, const char*, int);
} Image;

void initImage(Image* img);

bool loadImage(Image* img, const char* name);

bool createImage(Image* img, int width, int height, int channels, ImageFormat format);

void writeImage(Image* img, const char* name, int quality);

void freeImage(Image* img);

#endif


