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

void parseArgs(int argc, char **argv, int *Np, char *kernelStr, char *inputImage, char *outputImage) {

    int option;
    int pflag = 1, kflag = 1, iflag = 1, oflag = 1, hflag = 1;

    while ((option = getopt(argc, argv, "p:k:i:o:h")) != -1) {
        switch (option) {
            case 'p':
                if (pflag) {
                    printfe("Only one -p argument may be passed");
                }

                *Np = optarg;

                pflag = 1;
                break;
            case 'k':
                if (kflag) {
                    printfe("Only one -k argument may be passed");
                }

                kernelStr = (char *) malloc (sizeof(optarg));
                memcpy(kernelStr, optarg, sizeof(optarg));

                kflag = 1;
                break;
            case 'i':
                if (iflag) {
                    printfe("Only one -i argument may be passed");
                }

                inputImage = (char *) malloc (sizeof(optarg));
                memcpy(inputImage, optarg, sizeof(optarg));

                iflag = 1;
                break;
            case 'o':
                if (oflag) {
                    printfe("Only one -o argument may be passed");
                }

                oflag = 1;
                break;
            case 'h':
                if (hflag) {
                    printfe("Only one -h argument may be passed");
                }

                hflag = 1;
                break;
            default:
                printfe("Unknown input arguments.\n");
        }
    }

    if (hflag) {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    if (!pflag) {
        *Np = get_nprocs();
    }

    if (!kflag || !iflag || !oflag) {
        printfe("Insufficient input arguments.\n");
    }

}