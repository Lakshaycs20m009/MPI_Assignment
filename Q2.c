#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define pi 3.14
int main(int argc, char *argv[])
{
   MPI_Init(&argc, &argv);
   int rank, np, n;
   n = atoi(argv[1]);
   // scanf("%d", &n);
   int a1[n], a2[n];
   MPI_Status status;

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   printf("elements before transfer for rank %d", rank);
   srand(time(NULL) + rank);
   for (int i = 0; i < n; i++)
   {
      a1[i] = rand() % 10 * pi;
   }
   for (int i = 0; i < n; i++)
   {
      printf("\t %d", a1[i]);
   }
   printf("\n");
   int send_to = (rank + 1) % np;
   int recv_by;
   if (rank == 0)
   {
      recv_by = np - 1;

   }
   else
      recv_by = rank - 1;
   MPI_Send(a1, n, MPI_INT, send_to, 0, MPI_COMM_WORLD);
   MPI_Recv(a2, n, MPI_INT, recv_by, 0, MPI_COMM_WORLD, &status);
   printf("elements after transfer for rank %d", rank);
   for (int i = 0; i < n; i++)
   {
      printf("\t %d", a2[i]);
   }
   MPI_Finalize();
   return 0;
}


