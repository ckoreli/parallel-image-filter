#include "utils.h"

void printHelp() {
    printf("Usage: imfilter [-p NUMBER_OF_PROCESSES] -k KERNEL -i INPUT_IMAGE -o OUTPUT_IMAGE [-h]\n");
}

void printfe(char *msg) {
    printf("%s\n", msg);
    printHelp();
    exit(EXIT_FAILURE);
}

void freematrix(Pixel **mat, int height) {
    for (int i = 0; i < height; ++i)
        free(mat[i]);
    free(mat);
}

void *smalloc (size_t size) {
    void *mem = malloc (size);

    if (mem == NULL) {
	printfe("Not enough available memory to allocate");
    }

    return mem;
}

Pixel **matrixsmalloc(size_t width, size_t height) {
    Pixel **mat = (Pixel **) malloc (height);

    if (mat == NULL) {
        printfe("Not enough available memory to allocate");
    }
    
    for (int i = 0; i < height; ++i) {
        mat[i] = (Pixel *) malloc (width);
        if (mat[i] == NULL) {
            printfe("Not enough available memory to allocate");
        }
    }

    return mat;
}

FILE *safe_fopen(char *file, char *mode)
{
    FILE *f = fopen(file, mode);

    if (f == NULL)
    {
        printf("File not found.\n");
        exit(EXIT_FAILURE);
    }

    return f;
}
