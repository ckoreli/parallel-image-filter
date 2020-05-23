#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__

#include "utils.h"

void parseKernel(double kernel[][3], char *kernelStr);

void parseArgs(int argc, char **argv, int *Np, double kernel[][3], FILE **input, FILE **output);

#endif
