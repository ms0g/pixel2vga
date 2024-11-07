#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vga.h"
#include "image.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 2
#define VERSION_PATCH 0

#define STRINGIFY0(s) # s
#define STRINGIFY(s) STRINGIFY0(s)
#define VERSION STRINGIFY(VERSION_MAJOR) "." STRINGIFY(VERSION_MINOR) "." STRINGIFY(VERSION_PATCH)

typedef struct Args {
    char* format;
    char* image;
    char* outfile;
    int quality;
} Args;

static const Args parseArgs(int argc, char** argv) {
    static const char* usage = "OVERVIEW: A RGB to VGA color converter\n\n"
                               "USAGE: pixel2vga [options] <image file>\n\n"
                               "OPTIONS:\n"
                               "  -f, --format          File format[jpg <quality 0-100>?/png/bmp/tga/raw]\n"
                               "  -o, --outfile         Output file name\n"
                               "  -h, --help            Display available options\n"
                               "  -v, --version         Display the version of this program\n";

    Args args = {NULL, NULL, NULL};
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

            if (strcmp(args.format, "jpg") != 0 &&
                strcmp(args.format, "png") != 0 &&
                strcmp(args.format, "bmp") != 0 &&
                strcmp(args.format, "tga") != 0 &&
                strcmp(args.format, "raw") != 0) {
                fprintf(stderr, "Error: Unknown Format: %s\n", args.format);
                return args;
            }

            int tmp = i;
            args.quality = (int)strtol(argv[++tmp], NULL, 10);

        } else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--outfile")) {
            args.outfile = argv[++i];
        } else {
            args.image = argv[i];
        }
    }

    return args;
}

static void processImage(const Image* inImg, const Image* outImg) {
    for (uint8_t* p = inImg->pData, * pv = outImg->pData;
         p != inImg->pData + inImg->size;
         p += inImg->channels, pv += outImg->channels) {
        uint8_t r = *p;
        uint8_t g = *(p + 1);
        uint8_t b = *(p + 2);

        const Color vgaColor = vgaClamp(r, g, b);

        *pv = (uint8_t) vgaColor.r;
        *(pv + 1) = (uint8_t) vgaColor.g;
        *(pv + 2) = (uint8_t) vgaColor.b;

        if (inImg->channels == 4) {
            *(pv + 3) = *(p + 3);
        }
    }
}

int main(int argc, char** argv) {
    Image img;
    Image vgaImg;
    bool rv;

    initImage(&img);
    initImage(&vgaImg);

    const Args args = parseArgs(argc, argv);

    if (args.image == NULL) goto cleanup;

    rv = img.load(&img, args.image);
    if (!rv) goto cleanup;

    ImageFormat format = (!strcmp(args.format, "jpg") || !strcmp(args.format, "jpeg")) ? IMAGE_FORMAT_JPG :
                              !strcmp(args.format, "png") ? IMAGE_FORMAT_PNG :
                              !strcmp(args.format, "bmp") ? IMAGE_FORMAT_BMP :
                              !strcmp(args.format, "tga") ? IMAGE_FORMAT_TGA : IMAGE_FORMAT_RAW;

    rv = vgaImg.new(&vgaImg, img.width, img.height, img.channels, format);
    if (!rv) goto cleanup;

    processImage(&img, &vgaImg);

    vgaImg.write(&vgaImg, args.outfile, args.quality);

cleanup:
    freeImage(&img);
    freeImage(&vgaImg);

    return 0;
}
