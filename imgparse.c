#include "imgparse.h"

int check(int i, int max) {
    if (i < 0)
        return 0;
    if (i >= max)
        return max - 1;

    return i;
}

int normalize(int value, int maxval) {
    if (value < 0)
        return 0;
    if (value > maxval)
        return maxval;
    return value;
}

void parseImageHeader(FILE *input, int *width, int *height, int *maxval) {
    int lines = 0;
    char line[150];
    char *endptr;

    while (lines < 3) {
        fgets(line, 150, input);

        if (line[0] == '#')
            continue;

        if (lines == 0) {
            if (line[1] != '6') {
                printfe("Program supports only P6 ppm images");
            }
        }
        else if (lines == 1) {
            *width = strtol(line, &endptr, 10);
            *height = strtol(endptr + 1, NULL, 10);
        }
        else
            *maxval = strtol(line, NULL, 10);

        lines++;
    }
}

void parseImage(FILE *input, Pixel *iimage, int width, int height) {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    int i = 0;
    int size = width * height;

    while(1) {
        r = fgetc(input);
        g = fgetc(input);
        b = fgetc(input);
        
        if (feof(input)) 
            break;

        iimage[i].r = r;
        iimage[i].g = g;
        iimage[i].b = b;

        if (++i > size) {
            printf("%d\n", i);
            printfe("Error while parsing input image");
        }
    }
}

Pixel applyKernel(Pixel pixels[][3], double kernel[][3], int maxval) {
    Pixel pixel;

    pixel.r = normalize(pixels[0][0].r * kernel[0][0] + 
                        pixels[0][1].r * kernel[0][1] +
                        pixels[0][2].r * kernel[0][2] + 
                        pixels[1][0].r * kernel[1][0] +
                        pixels[1][1].r * kernel[1][1] +
                        pixels[1][2].r * kernel[1][2] +
                        pixels[2][0].r * kernel[2][0] +
                        pixels[2][1].r * kernel[2][1] +
                        pixels[2][2].r * kernel[2][2], maxval);

    pixel.g = normalize(pixels[0][0].g * kernel[0][0] + 
                        pixels[0][1].g * kernel[0][1] +
                        pixels[0][2].g * kernel[0][2] + 
                        pixels[1][0].g * kernel[1][0] +
                        pixels[1][1].g * kernel[1][1] +
                        pixels[1][2].g * kernel[1][2] +
                        pixels[2][0].g * kernel[2][0] +
                        pixels[2][1].g * kernel[2][1] +
                        pixels[2][2].g * kernel[2][2], maxval);

    pixel.b = normalize(pixels[0][0].b * kernel[0][0] + 
                        pixels[0][1].b * kernel[0][1] +
                        pixels[0][2].b * kernel[0][2] + 
                        pixels[1][0].b * kernel[1][0] +
                        pixels[1][1].b * kernel[1][1] +
                        pixels[1][2].b * kernel[1][2] +
                        pixels[2][0].b * kernel[2][0] +
                        pixels[2][1].b * kernel[2][1] +
                        pixels[2][2].b * kernel[2][2], maxval);

    return pixel;
}

void applyKernelGlobal(Pixel *iimage, double kernel[][3], Pixel *oimage, 
                int width, int height, int maxval) {

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {

            oimage[i * width + j].r = normalize(iimage[check(i - 1, height) * width + check(j - 1, width)].r * kernel[0][0] +
                                       iimage[check(i - 1, height) * width +                   j].r * kernel[0][1] +
                                       iimage[check(i - 1, height) * width + check(j + 1, width)].r * kernel[0][2] +
                                       iimage[                   i * width + check(j - 1, width)].r * kernel[1][0] +
                                       iimage[                   i * width +                   j].r * kernel[1][1] +
                                       iimage[                   i * width + check(j + 1, width)].r * kernel[1][2] +
                                       iimage[check(i + 1, height) * width + check(j - 1, width)].r * kernel[2][0] +
                                       iimage[check(i + 1, height) * width +                   j].r * kernel[2][1] +
                                       iimage[check(i + 1, height) * width + check(j + 1, width)].r * kernel[2][2], maxval);

            oimage[i * width + j].g = normalize(iimage[check(i - 1, height) * width + check(j - 1, width)].g * kernel[0][0] +
                                       iimage[check(i - 1, height) * width +                   j].g * kernel[0][1] +
                                       iimage[check(i - 1, height) * width + check(j + 1, width)].g * kernel[0][2] +
                                       iimage[                   i * width + check(j - 1, width)].g * kernel[1][0] +
                                       iimage[                   i * width +                   j].g * kernel[1][1] +
                                       iimage[                   i * width + check(j + 1, width)].g * kernel[1][2] +
                                       iimage[check(i + 1, height) * width + check(j - 1, width)].g * kernel[2][0] +
                                       iimage[check(i + 1, height) * width +                   j].g * kernel[2][1] +
                                       iimage[check(i + 1, height) * width + check(j + 1, width)].g * kernel[2][2], maxval);

            oimage[i * width + j].b = normalize(iimage[check(i - 1, height) * width + check(j - 1, width)].b * kernel[0][0] +
                                       iimage[check(i - 1, height) * width +                   j].b * kernel[0][1] +
                                       iimage[check(i - 1, height) * width + check(j + 1, width)].b * kernel[0][2] +
                                       iimage[                   i * width + check(j - 1, width)].b * kernel[1][0] +
                                       iimage[                   i * width +                   j].b * kernel[1][1] +
                                       iimage[                   i * width + check(j + 1, width)].b * kernel[1][2] +
                                       iimage[check(i + 1, height) * width + check(j - 1, width)].b * kernel[2][0] +
                                       iimage[check(i + 1, height) * width +                   j].b * kernel[2][1] +
                                       iimage[check(i + 1, height) * width + check(j + 1, width)].b * kernel[2][2], maxval);
        }
    }
}

