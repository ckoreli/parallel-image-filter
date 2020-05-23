#ifndef __UTILS_H__
#define __UTILS_H__

#include "headers.h"

typedef struct pixel {
    int r;
    int g;
    int b;
} Pixel;

void printHelp();

void printfe(char *msg);

void *smalloc(size_t size);
void freematrix(Pixel** matrix, int height);

FILE *safe_fopen(char *file, char *mode);

#endif
