#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "vga.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define STRINGIFY0(s) # s
#define STRINGIFY(s) STRINGIFY0(s)
#define VERSION STRINGIFY(VERSION_MAJOR) "." STRINGIFY(VERSION_MINOR) "." STRINGIFY(VERSION_PATCH)

typedef struct args {
    char* format;
    char* image;
    char* outfile;
} args_t;

args_t parseArgs(int argc, char** argv) {
    static const char* usage = "OVERVIEW: A RGB to VGA color converter\n\n"
                               "USAGE: pixel2vga [options] <image file>\n\n"
                               "OPTIONS:\n"
                               "  -f, --format          File format[jpg/png/bmp/tga]\n"
                               "  -o, --outfile         Output file name\n"
                               "  -h, --help            Display available options\n"
                               "  -v, --version         Display the version of this program\n";

    args_t args = {NULL, NULL, NULL};
    if (argc < 6) {
        if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
            printf("%s", usage);
        } else if (argc == 2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) {
            printf("version %s", VERSION);
        } else {
            printf("%s", usage);
            return args;
        }
        return args;
    }

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--format")) {
            args.format = argv[++i];
        } else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--outfile")) {
            args.outfile = argv[++i];
        } else {
            args.image = argv[i];
        }
    }

    return args;
}

int main(int argc, char** argv) {
    uint8_t* img = NULL;
    uint8_t* vga_img = NULL;
    int width, height, channels;

    args_t args = parseArgs(argc, argv);

    if (args.image == NULL) {
        goto cleanup;
    }

    img = stbi_load(args.image, &width, &height, &channels, 0);
    if (img == NULL) {
        fprintf(stderr, "%s File Not Found\n", args.image);
        goto cleanup;
    }

    size_t img_size = width * height * channels;

    vga_img = malloc(img_size);
    if (vga_img == NULL)
        goto cleanup;

    for (uint8_t* p = img, * pv = vga_img; p != img + img_size; p += channels, pv += channels) {
        uint8_t r = *p;
        uint8_t g = *(p + 1);
        uint8_t b = *(p + 2);

        color vga_color = vgaclamp(r, g, b);

        *pv = (uint8_t) vga_color.r;
        *(pv + 1) = (uint8_t) vga_color.g;
        *(pv + 2) = (uint8_t) vga_color.b;

        if (channels == 4) {
            *(pv + 3) = *(p + 3);
        }
    }

    if (!strcmp(args.format, "jpg") || !strcmp(args.format, "jpeg")) {
        stbi_write_jpg(args.outfile, width, height, channels, vga_img, 100);
    } else if (!strcmp(args.format, "png")) {
        stbi_write_png(args.outfile, width, height, channels, vga_img, width * channels);
    } else if (!strcmp(args.format, "bmp")) {
        stbi_write_bmp(args.outfile, width, height, channels, vga_img);
    } else if (!strcmp(args.format, "tga")) {
        stbi_write_tga(args.outfile, width, height, channels, vga_img);
    }

cleanup:
    if (vga_img != NULL)
        free(vga_img);

    if (img != NULL)
        stbi_image_free(img);

    return 0;
}
