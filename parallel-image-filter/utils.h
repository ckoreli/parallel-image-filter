#ifndef __UTILS_H__
#define __UTILS_H__

#include "headers.h"

typedef struct pixel
{
    int r;
    int g;
    int b;
} Pixel;

typedef struct workPackage
{
    Pixel pixels[3][3];
    unsigned int pixel_index;
} WorkPackage;

typedef struct workPackageMessage
{
    int mtype;
    WorkPackage pkg;
} WorkPackageMessage;

typedef struct resultPackage
{
    Pixel pixel;
    unsigned int pixel_index;
} ResultPackage;

typedef struct resultPackageMessage
{
    int mtype;
    ResultPackage rpkg;
} ResultPackageMessage;

void printHelp();

void printfe(char *msg);

void *smalloc(size_t size);

FILE *safe_fopen(char *file, char *mode);

#endif