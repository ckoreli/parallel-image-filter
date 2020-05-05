#include "parse.h"

int main(int argc, char **argv) {

    int Np = 0;
    double kernel[3][3];
    FILE *input = NULL;
    FILE *output = NULL;

    parseArgs(argc, argv, &Np, kernel, input, output);

    printf("Np == %d\n", Np);

    printf("Kernel:\n");
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%.2lf ", kernel[i][j]);
        }
        printf("\n");
    }

    // key_t key;
    // int wmq, rmq;

    int pom = get_nprocs();

    printf("%d\n", pom);

    // Setting key to IPC_PRIVATE
    // key = IPC_PRIVATE;

    // Creating workload message queue
    // and result message queue.
    // wmq = msgget(key, IPC_CREAT | 0666);
    // rmq = msgget(key, IPC_CREAT | 0666);

    return 0;
}

// imfilter -p 4 -k "0,-1,0,-1,4,-1,0,-1,0" -i input.ppm -o output.ppm