#ifndef __PARSE_H__
#define __PARSE_H__

#include "headers.h"

void parseKernel(char *kernelStr, double **kernel);

void parseArgs(int argc, char **argv, int *Np, double kernel[3][3], FILE *input, FILE *output);

#endif