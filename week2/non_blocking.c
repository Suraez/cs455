#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int data = -1;

    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("Please run with exactly 2 processes.\n");
        }

        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {

        data = 100;

        printf("Process 0: Waiting 3 seconds before sending...\n");
        sleep(3);

        MPI_Send(
            &data,
            1,
            MPI_INT,
            1,
            0,
            MPI_COMM_WORLD
        );

        printf("Process 0: Data sent.\n");
    }

    else if (rank == 1) {

        printf("Process 1: Initial data = %d\n", data);

        printf("Process 1: Calling MPI_Irecv()...\n");

        MPI_Irecv(
            &data,
            1,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            &request
        );

        printf("Process 1: After MPI_Irecv(), data = %d\n", data);

        printf("Process 1: I did NOT wait for the message.\n");

        MPI_Wait(
            &request,
            MPI_STATUS_IGNORE
        );

        printf("Process 1: After MPI_Wait(), data = %d\n", data);
    }

    MPI_Finalize();
    return 0;
}