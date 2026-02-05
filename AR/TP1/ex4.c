#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int size;
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char buf[1024];
    MPI_Status status;

    // 1) calcul de la taille nécessaire
    int len = snprintf(NULL, 0, "Je suis le client %d. J'envoie au client %d.", rank, (rank + 1) % size);

    // 2) allocation (+1 pour le '\0')
    char *msg = malloc(len + 1);
    
    // 3) écriture dans le buffer
    snprintf(msg, len + 1, "Je suis le client %d. J'envoie au client %d.", rank, (rank + 1) % size);

    if (rank == (size - 1)) {
        MPI_Recv(buf, 1024, MPI_CHAR, (rank - 1) % size, 99, MPI_COMM_WORLD, &status);
        MPI_Ssend(msg, strlen(msg) + 1, MPI_CHAR, (rank + 1) % size, 99, MPI_COMM_WORLD);
    } else {
        MPI_Ssend(msg, strlen(msg) + 1, MPI_CHAR, (rank + 1) % size, 99, MPI_COMM_WORLD);
        MPI_Recv(buf, 1024, MPI_CHAR, (rank - 1) % size, 99, MPI_COMM_WORLD, &status);
    }

    printf("%s\n", buf);
    fflush(stdout);

    free(msg);

    MPI_Finalize();
    return 0;
}