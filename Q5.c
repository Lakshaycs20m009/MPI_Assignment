# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>

int main(int argc, char* argv[])
{

	int rank, np, n;
	n = atoi(argv[1]);
	double a_local[n / 2], b_local[n / 2];
	for (int i = 0; i < n / 2; i++)
	{
		a_local[i] = rand() % 10;
		b_local[i] = rand() % 10;
	}

	double s_local, s;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	s_local = 0;
	for (int i = 0; i < n / 2; i++)
	{
		s_local = s_local + a_local[i] * b_local[i];
	}

	if (rank == 0)
	{
		MPI_Send(&s_local, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

	}
	if (rank == 1)
	{
		MPI_Recv(&s, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

		s = s + s_local;
	}
	if (rank == 0)
	{
		MPI_Recv(&s, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &status);
		printf("result %f", s);

	}
	if (rank == 1)
	{
		MPI_Send(&s, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);

	}


	MPI_Finalize();
	return 0;
}
