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

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            char buf[1024];
            MPI_Status status;

            MPI_Recv(buf, 1024, MPI_CHAR, i, 99, MPI_COMM_WORLD, &status);
            printf("MASTER : reçu du client %d : '%s'.\n", status.MPI_SOURCE, buf);
            fflush(stdout);

            // 1) calcul de la taille nécessaire
            int len = snprintf(NULL, 0, "Hello client %d ! Message bien reçu.", status.MPI_SOURCE);
            
            // 2) allocation (+1 pour le '\0')
            char *msg = malloc(len + 1);
            
            // 3) écriture dans le buffer
            snprintf(msg, len + 1, "Hello client %d ! Message bien reçu.", status.MPI_SOURCE);

            MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, i, 99, MPI_COMM_WORLD);

            free(msg);
        }
    } else {
        char buf[1024];
        MPI_Status status;

        // 1) calcul de la taille nécessaire
        int len = snprintf(NULL, 0, "Hello master ! Je suis le client %d.", rank);
            
        // 2) allocation (+1 pour le '\0')
        char *msg = malloc(len + 1);
        
        // 3) écriture dans le buffer
        snprintf(msg, len + 1, "Hello master ! Je suis le client %d.", rank);

        MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, 0, 99, MPI_COMM_WORLD);
        MPI_Recv(buf, 1024, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        printf("%s\n", buf);
        fflush(stdout);

        free(msg);
    }

    MPI_Finalize();
    return 0;
}