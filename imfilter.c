#include "argparse.h"
#include "imgparse.h"
#include "utils.h"
#include "message.h"

int main(int argc, char **argv) {

    FILE *input  = NULL;
    FILE *output = NULL;

    int Np;
    double kernel[3][3];

    // Parsing command line arguments
    parseArgs(argc, argv, &Np, kernel, &input, &output);

    int width, height;
    int maxval;

    // Parsing input image header
    parseImageHeader(input, &width, &height, &maxval);
    
    if (width * height > 1048576)
        printfe("Program can't parse images larger than 1024x1024");

    // Allocating memory for input and output image buffers
    Pixel *iimage = (Pixel *) smalloc (width * height * sizeof(Pixel));
    Pixel *oimage = (Pixel *) smalloc (width * height * sizeof(Pixel));

    // Parsing input image
    parseImage(input, iimage, width, height);

    key_t key;
    int wmq, rmq;

    // Setting key to IPC_PRIVATE
    key = IPC_PRIVATE;

    // Creating workload message queue
    wmq = msgget(key, IPC_CREAT | 0666);

    // Creating result message queue
    rmq = msgget(key, IPC_CREAT | 0666);

    if (wmq == -1 || rmq == -1) {
        printfe("Couldn't create message queues");
    }

    int pid[Np];
    int current;
    int size = width * height;

    for (int i = 0; i < Np; ++i) {
        pid[i] = current = fork();
        if (pid[i] == 0)
            break;
    }

    // Parent process
    if (current != 0) {
        WorkPackage wpkg;
        WorkPackageMessage wpkgm;

        ResultPackageMessage rpkgm;

        struct msqid_ds stats;

        int sent = 0;
        int received = 0;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                // Getting 3x3 pixels from input image
                wpkg.pixels[0][0] = iimage[check(i - 1, height) * width + check(j - 1, width)];
                wpkg.pixels[0][1] = iimage[check(i - 1, height) * width +                   j];
                wpkg.pixels[0][2] = iimage[check(i - 1, height) * width + check(j + 1, width)];
                wpkg.pixels[1][0] = iimage[                   i * width + check(j - 1, width)];
                wpkg.pixels[1][1] = iimage[                   i * width +                   j];
                wpkg.pixels[1][2] = iimage[                   i * width + check(j + 1, width)];
                wpkg.pixels[2][0] = iimage[check(i + 1, height) * width + check(j - 1, width)];
                wpkg.pixels[2][1] = iimage[check(i + 1, height) * width +                   j];
                wpkg.pixels[2][2] = iimage[check(i + 1, height) * width + check(j + 1, width)];

                wpkg.pixel_index = i * width + j;

                wpkgm.mtype = 1;
                wpkgm.wpkg = wpkg;

                // Sending pixels to work message queue
                int m = msgsnd(wmq, &wpkgm, sizeof(WorkPackage), IPC_NOWAIT);

                ++sent;

                if (m == -1) {
                    if (errno == EAGAIN) {
                        // Work message queue full, reading from results
                        while (1) {
                            m = msgrcv(rmq, &rpkgm, sizeof(ResultPackage), 0, IPC_NOWAIT);
                            
                            if (m == -1) {
                                if (errno == ENOMSG)
                                    break;
                                else {
                                    printf("%d\n", errno);
                                    break;
                                }
                            }

                            oimage[rpkgm.rpkg.pixel_index].r = rpkgm.rpkg.pixel.r;
                            oimage[rpkgm.rpkg.pixel_index].g = rpkgm.rpkg.pixel.g;
                            oimage[rpkgm.rpkg.pixel_index].b = rpkgm.rpkg.pixel.b;

                            ++received;
                        } 

                        // Resending failed message
                        msgsnd(wmq, &wpkgm, sizeof(WorkPackage), IPC_NOWAIT);
                    }
                    else {
                        // Unexpected error, exiting
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }

        msgctl(rmq, IPC_STAT, &stats); 
        while (stats.msg_qnum || received != size) {
            // Reading remaining results and writing to output image
            msgrcv(rmq, &rpkgm, sizeof(ResultPackage), 0, IPC_NOWAIT);

            oimage[rpkgm.rpkg.pixel_index].r = rpkgm.rpkg.pixel.r;
            oimage[rpkgm.rpkg.pixel_index].g = rpkgm.rpkg.pixel.g;
            oimage[rpkgm.rpkg.pixel_index].b = rpkgm.rpkg.pixel.b;

            ++received;

            msgctl(rmq, IPC_STAT, &stats);
        }

        if (received != size) {
            printf("Didn't receive all pixels. Exiting");
            exit(EXIT_FAILURE);
        }

        // Removing message queues
        msgctl(wmq, IPC_RMID, NULL);
        msgctl(rmq, IPC_RMID, NULL);
    }
    // Child processes
    else {
        ResultPackage rpkg;
        ResultPackageMessage rpkgm;

        WorkPackageMessage wpkgm;

        int m, r;

        while (1) {
            // Reading message from work message queue
            m = msgrcv(wmq, &wpkgm, sizeof(WorkPackage), 0, 0);
            
//          printf("%6d ", getpid());
            if (m == -1) {
                if (errno == EIDRM) {
                    exit(EXIT_SUCCESS);
                }
                printf("%6d\tpropo pri primanju msg queue error: %d\n", getpid(), errno);
                exit(EXIT_FAILURE);
            }
            else
//              printf("proso\n");

            // Applying kernel to received pixels
            rpkg.pixel       = applyKernel(wpkgm.wpkg.pixels, kernel, maxval);
            rpkg.pixel_index = wpkgm.wpkg.pixel_index;

            rpkgm.mtype = 1;
            rpkgm.rpkg = rpkg;

            // Sending result pixel to result message queue
            r = msgsnd(rmq, &rpkgm, sizeof(ResultPackage), 0);

            if (r == -1) {
                if (errno == EIDRM) {
                    exit(EXIT_SUCCESS);
                }
                exit(EXIT_FAILURE);
            }
        }
    }
    // Waiting for child processes to exit
    int status;

    for (int i = 0; i < Np; ++i) {
        waitpid(pid[i], &status, 0);
        if (WIFEXITED(status)) {
            printf("Process exited with status code %d\n", WEXITSTATUS(status));
        }
    }
    
    // Writing oimage to output
    fprintf(output, "P6\n");
    fprintf(output, "%d %d\n", width, height);
    fprintf(output, "%d\n", maxval);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fprintf(output, "%c%c%c", oimage[i * width + j].r, oimage[i * width + j].g, oimage[i * width + j].b);
//          fputc(oimage[i * width + j].r, output);
//          fputc(oimage[i * width + j].g, output);
//          fputc(oimage[i * width + j].b, output);
        }
    }

    free(iimage);
    free(oimage);

    fclose(input);
    fclose(output);

    return 0;
}
