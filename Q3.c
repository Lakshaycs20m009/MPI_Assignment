#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define max_rows 10000000

int a[max_rows];



// size of array

// Temporary array for slave process
int a2[1000];


int main(int argc, char* argv[])
{

	int rank, np,
	    elements_per_process,
	    n_elements_recieved;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		a[i] = i;
	}



	// master process
	if (rank == 0) {




		int index, i;
		elements_per_process = n / np;

		if (np > 1) {

			for (i = 1; i < np - 1; i++) {
				index = i * elements_per_process;

				MPI_Send(&elements_per_process,
				         1, MPI_INT, i, 0,
				         MPI_COMM_WORLD);
				MPI_Send(&a[index],
				         elements_per_process,
				         MPI_INT, i, 0,
				         MPI_COMM_WORLD);
			}

			// last process adds remaining elements
			index = i * elements_per_process;
			int elements_left = n - index;

			MPI_Send(&elements_left,
			         1, MPI_INT,
			         i, 0,
			         MPI_COMM_WORLD);
			MPI_Send(&a[index],
			         elements_left,
			         MPI_INT, i, 0,
			         MPI_COMM_WORLD);
		}

		// master process add its own sub array
		int sum = 0;
		for (i = 0; i < elements_per_process; i++)
			sum += a[i];

		// collects partial sums from other processes
		int tmp;
		for (i = 1; i < np; i++) {
			MPI_Recv(&tmp, 1, MPI_INT,
			         MPI_ANY_SOURCE, 0,
			         MPI_COMM_WORLD,
			         &status);
			int sender = status.MPI_SOURCE;

			sum += tmp;
		}

		// prints the final sum of array
		printf("Sum of array is : %d\n", sum);
	}
	// slave processes
	else {
		MPI_Recv(&n_elements_recieved,
		         1, MPI_INT, 0, 0,
		         MPI_COMM_WORLD,
		         &status);

		// stores the received array segment
		// in local array a2
		MPI_Recv(&a2, n_elements_recieved,
		         MPI_INT, 0, 0,
		         MPI_COMM_WORLD,
		         &status);

		// calculates its partial sum
		int partial_sum = 0;
		for (int i = 0; i < n_elements_recieved; i++)
			partial_sum += a2[i];

		// sends the partial sum to the root process
		MPI_Send(&partial_sum, 1, MPI_INT,
		         0, 0, MPI_COMM_WORLD);
	}

	// cleans up all MPI state before exit of process
	MPI_Finalize();

	return 0;
}

