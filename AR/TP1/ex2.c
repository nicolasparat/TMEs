#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int size;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Processus %d sur %d : Hello MPI\n", rank + 1, size);

    MPI_Finalize();
    return 0;
}