#include "utils.h"

void printHelp() {
    printf("Usage: imfilter [-p NUMBER_OF_PROCESSES] -k KERNEL -i INPUT_IMAGE -o OUTPUT_IMAGE [-h]");
}

void printfe(char *msg) {
    printf("%s\n", msg);
    printHelp();
    exit(EXIT_FAILURE);
}

void *smalloc (size_t size) {
    void *mem = malloc (size);

    if (mem == NULL) {

    }

    return mem;
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