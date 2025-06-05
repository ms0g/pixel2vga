#include "image.h"
#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static const int DEFAULT_JPG_QUALITY = 100;

void initImage(Image* img) {
    memset(img, 0, sizeof(Image));

    img->load = loadImage;
    img->new = createImage;
    img->write = writeImage;
}

ImageFormat getImageFormat(const char* ext) {
    return !strcmp(ext, "jpg") || !strcmp(ext, "jpeg")
               ? IMAGE_FORMAT_JPG
               : !strcmp(ext, "png")
                     ? IMAGE_FORMAT_PNG
                     : !strcmp(ext, "bmp")
                           ? IMAGE_FORMAT_BMP
                           : !strcmp(ext, "tga")
                                 ? IMAGE_FORMAT_TGA
                                 : IMAGE_FORMAT_RAW;
}

bool loadImage(Image* img, const char* name) {
    img->pData = stbi_load(name, &img->width, &img->height, &img->channels, 0);

    if (img->pData == NULL) {
        fprintf(stderr, "Error: File Not Found: %s\n", name);
        return false;
    }

    img->size = img->width * img->height * img->channels;
    img->aType = ALLOCATION_TYPE_STB_ALLOCATED;

    return true;
}

bool createImage(Image* img, const int width, const int height, const int channels, const ImageFormat format) {
    img->width = width;
    img->height = height;
    img->channels = channels;
    img->format = format;
    img->aType = ALLOCATION_TYPE_CUSTOM_ALLOCATED;
    img->size = width * height * channels;
    img->pData = (uint8_t*) malloc(img->size);

    if (img->pData == NULL) {
        fprintf(stderr, "Error: Image Buffer Not Allocated\n");
        return false;
    }

    return true;
}

void writeImage(const Image* img, const char* name, const int quality) {
    switch (img->format) {
        case IMAGE_FORMAT_JPG:
            stbi_write_jpg(name, img->width, img->height, img->channels, img->pData,
                           quality ? quality : DEFAULT_JPG_QUALITY);
            break;
        case IMAGE_FORMAT_PNG:
            stbi_write_png(name, img->width, img->height, img->channels, img->pData, img->width * img->channels);
            break;
        case IMAGE_FORMAT_BMP:
            stbi_write_bmp(name, img->width, img->height, img->channels, img->pData);
            break;
        case IMAGE_FORMAT_TGA:
            stbi_write_tga(name, img->width, img->height, img->channels, img->pData);
            break;
        case IMAGE_FORMAT_RAW: {
            FILE* outfile = fopen(name, "wb");
            fwrite(img->pData, sizeof(uint8_t), img->size, outfile);
            fclose(outfile);
            break;
        }
    }
}

void freeImage(Image* img) {
    if (img->pData == NULL) return;

    switch (img->aType) {
        case ALLOCATION_TYPE_STB_ALLOCATED:
            stbi_image_free(img->pData);
            break;
        case ALLOCATION_TYPE_CUSTOM_ALLOCATED:
            free(img->pData);
            break;
    }

    img->pData = NULL;
}
