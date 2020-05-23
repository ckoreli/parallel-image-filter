#ifndef __IMGPARSE_H__
#define __IMGPARSE_H__

#include "utils.h"

int check(int i, int max);
int normalize(int value, int maxval);
void applyKernelGlobal(Pixel *iimage, double kernel[][3], Pixel *oimage,
                int width, int height, int maxval);
void parseImageHeader(FILE *input, int *width, int *height, int *maxval);
void parseImage(FILE *input, Pixel *iimage, int width, int height);

Pixel applyKernel(Pixel pixels[][3], double kernel[][3], int maxval);

#endif
