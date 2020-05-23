#include "argparse.h"

void parseKernel(double kernel[][3], char *kernelStr) {
    char *endptr;

    kernel[0][0] = strtod(kernelStr, &endptr);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
	    if (i == 0 && j == 0)
	        continue;
	    
	    kernel[i][j] = strtod(endptr + 1, &endptr);
	    if (*endptr == 0 && (i != 2 || j != 2)) {
	        printfe("Kernel format incorrect");
	    }
	}
    }
}

void parseArgs(int argc, char **argv, int *Np, double kernel[][3], FILE **input, FILE **output) {
    int option;
    int pflag, kflag, iflag, oflag, hflag;

    pflag = kflag = iflag = oflag = hflag = 0;

    while ((option = getopt(argc, argv, "p:k:i:o:h")) != -1) {
        switch (option) {
	    case 'p':
	        if (pflag)
		    printfe("Only one -p argument may be passed");

		*Np = strtol(optarg, NULL, 10);

		pflag = 1;
		break;
	    case 'k':
		if (kflag)
	            printfe("Only one -k argument may be passed");

		parseKernel(kernel, optarg);

		kflag = 1;
		break;
	    case 'i':
		if (iflag)
		    printfe("Only one -i argument may be passed");

		*input = safe_fopen(optarg, "rb");

		iflag = 1;
		break;
	    case 'o':
		if (oflag)
		    printfe("Only one -o argument may be passed");

		*output = safe_fopen(optarg, "wb");

		oflag = 1;
		break;
	    case 'h':
		if (hflag)
		    printfe("Only one -h argument may be passed");

		hflag = 1;
		break;
	    default:
		printf("Unknown argument -%c", optopt);
		exit(EXIT_FAILURE);
		break;
	}
    }

    if (hflag) {
        printHelp();
	exit(EXIT_SUCCESS);
    }

    if (!pflag)
        *Np = get_nprocs();

    if (!kflag || !iflag || !oflag)
        printfe("Insufficient input arguments");
}
