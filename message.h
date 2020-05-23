#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "utils.h"

typedef struct workPackage {
    Pixel pixels[3][3];
    unsigned int pixel_index;
} WorkPackage;

typedef struct workPackageMessage {
    int mtype;
    WorkPackage wpkg;
} WorkPackageMessage;

typedef struct resultPackage {
    Pixel pixel;
    unsigned int pixel_index;
} ResultPackage;

typedef struct resultPackageMessage {
    int mtype;
    ResultPackage rpkg;
} ResultPackageMessage;

#endif
