#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    //IITT_barrier() using waitall
    int np;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
    {
        // The "master" MPI process sends the message.
        int arr[np];
        for (int i = 0; i < np; i++)
        {
            arr[i] = rand() % 10;

        }

        MPI_Request requests[np];

        for (int i = 0; i < np; i++)
        {
            printf("MPI process %d sends the value %d\n", my_rank, arr[i]);
            MPI_Isend(&arr[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);

        }


        // Wait for routines to complete
        MPI_Waitall(np - 1, requests, MPI_STATUSES_IGNORE);
        printf("Process %d: all messages have been sent.\n", my_rank);
    }
    else
    {
        // The "slave" MPI processes receive the message.
        int received;
        MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received value %d.\n", my_rank, received);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}