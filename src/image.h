#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum ImageFormat {
    IMAGE_FORMAT_JPG,
    IMAGE_FORMAT_PNG,
    IMAGE_FORMAT_BMP,
    IMAGE_FORMAT_TGA,
    IMAGE_FORMAT_RAW
};

enum ImageType {
    IMAGE_TYPE_IN,
    IMAGE_TYPE_OUT
};

typedef struct Image {
    int width;
    int height;
    int channels;
    size_t size;
    enum ImageFormat format;
    enum ImageType type;
    uint8_t* pData;
    bool (*load)(struct Image*, const char*);
    bool (*new)(struct Image*, int, int, int, enum ImageFormat);
    void (*write)(struct Image*, const char*);
} Image;

void initImage(Image* img);

bool loadImage(Image* img, const char* name);

bool createImage(Image* img, int width, int height, int channels, enum ImageFormat format);

void writeImage(Image* img, const char* name);

void freeImage(Image* img);

#endif


