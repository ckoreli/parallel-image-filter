#include "utils.h"
#include "parse.h"

void parseKernel() {
    
}

void parseArgs(int argc, char **argv, int *Np, double kernel[3][3], FILE *inputImage, FILE *outputImage)
{

    int option;
    int pflag = 0, kflag = 0, iflag = 0, oflag = 0, hflag = 0;

    char *kernelStr = NULL;

    while ((option = getopt(argc, argv, "p:k:i:o:h")) != -1)
    {
        switch (option)
        {
        case 'p':
            if (pflag)
            {
                printfe("Only one -p argument may be passed");
            }

            *Np = strtol(optarg, NULL, 10);

            pflag = 1;
            break;
        case 'k':
            if (kflag)
            {
                printfe("Only one -k argument may be passed");
            }

            // kernelStr = (char *) malloc (sizeof(optarg));
            // memcpy(kernelStr, optarg, sizeof(optarg));

            printf("-k %s\n", optarg);

            kflag = 1;
            break;
        case 'i':
            if (iflag)
            {
                printfe("Only one -i argument may be passed");
            }

            // inputImage = (char *) malloc (sizeof(optarg));
            // memcpy(inputImage, optarg, sizeof(optarg));

            printf("-i %s\n", optarg);

            iflag = 1;
            break;
        case 'o':
            if (oflag)
            {
                printfe("Only one -o argument may be passed");
            }

            printf("-o %s\n", optarg);

            oflag = 1;
            break;
        case 'h':
            if (hflag)
            {
                printfe("Only one -h argument may be passed");
            }

            hflag = 1;
            break;
        default:
            printfe("Unknown input arguments.\n");
        }
    }

    if (hflag)
    {
        printHelp();
        exit(EXIT_SUCCESS);
    }

    if (!pflag)
    {
        *Np = get_nprocs();
    }

    if (!kflag || !iflag || !oflag)
    {
        printfe("Insufficient input arguments.\n");
    }
}