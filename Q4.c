# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
#include<string.h>

int value(int n, int d) {
	return (n / d + (n % d != 0));
}

int main(int argc, char *argv[]) {

	//Variables to store process id and number of processes
	int rank, np, n;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	n = atoi(argv[1]);
	char *distribution_type = argv[2];
	int type;
	if (strcmp(distribution_type, "a") == 0) //for block distribution
	{
		type = 1;
	}
	else if (strcmp(distribution_type, "b") == 0)


	{
		type = 2;
	}
	else if (strcmp(distribution_type, "c") == 0)
	{
		type = 3;
	}
	int temp_sum = 0;

	if (type == 1)
	{
		int elements_per_process = value(n, np);
		int start = rank * elements_per_process;
		int end = rank < np - 1 ? ((rank + 1) * elements_per_process) - 1 : n - 1;
		int a[elements_per_process];
		int number_of_elements = end - start + 1;


		for (int i = 0; i < number_of_elements; i++) {
			a[i] = start + i;
			temp_sum += a[i];
		}
	}
	else if (type == 2)
	{
		int elements_per_process = value(n, np);
		int a[elements_per_process];
		int index = 0;

		for (int i = rank; i < n; i += np) {
			a[index] = i;
			temp_sum += a[index++];
		}
	}
	else if (type == 3)
	{
		int block_size;
		block_size = atoi(argv[3]);
		int elements_per_process = (value(value(n, block_size), np)) * block_size;
		int a[elements_per_process];
		int number_of_elements = 0, index = 0;
		for (int i = rank * block_size; i < n; i++) {
			if (number_of_elements == block_size) {
				i = i + (np - 1) * block_size;
				if (i > n) break;
				number_of_elements = 0;
			}
			a[index] = i;
			temp_sum += a[index++];
			number_of_elements++;
		}
	}

	if (rank == 0)
	{
		int sum = temp_sum;
		for (int i = 0; i < np; i++)
		{
			int x;
			MPI_Recv(&x, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			sum += x;

		}
		printf("\n Total %d", sum);
	}
	else
	{
		MPI_Send(&temp_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

	}
	MPI_Finalize();

	return 0;
}
