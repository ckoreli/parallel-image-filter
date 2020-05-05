#include "utils.h"

int main(int argc, char **argv) {

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