# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
#define pi 3.14
int main(int argc, char *argv[]) {

	//Variables to store process id and number of processes
	int rank, np;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	int n, m;
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	srand(time(NULL) + rank);

	int a1[n], a2[n], a3[n];
	for (int i = 0; i < n; i++) {
		a1[i] = pi * (rand() % 10);
		printf("Process Rank %d %d\n", rank, a1[i]);
	}

	while (m-- > 0)
	{
		int X, new_X;
		if (rank == 0) {
			// while (rank == send_pid)
			X = rand() % np;
			printf("Sending from %d to %d\n", rank, X);
			MPI_Send(a1, n, MPI_INT, X, 0, MPI_COMM_WORLD);
		}
		if (rank == X)
		{
			new_X = rand() % np;
			MPI_Recv(a2, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			printf("\n rank %d", X);
			for (int i = 0; i < n; i++)
			{
				printf("data recieved %d \t", a2[i]);

			}

			printf("Sending from %d to %d\n", X, new_X);
			MPI_Send(a2 , n, MPI_INT, new_X, 0, MPI_COMM_WORLD);

		}
		if (rank == new_X)
		{
			MPI_Recv(a3, n, MPI_INT, X, 0, MPI_COMM_WORLD, &status);
			printf("\n rank %d", new_X);

			for (int i = 0; i < n; i++)
			{
				printf("data recieved %d \t", a3[i]);

			}

		}


	}
	MPI_Finalize();
	return 0;
}

