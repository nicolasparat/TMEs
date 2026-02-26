#include <mpi.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

bool isInitiateur;
int size, rang, valeur;
MPI_Status status;

#define CHECK 97
#define ANNOUNCEMENT 98
#define NORMAL 99

void algo() {
    if (isInitiateur) {
        MPI_Send(&valeur, 1, MPI_INT, (rang + 1) % size, NORMAL, MPI_COMM_WORLD);
    }

    while (true) {
        int received_value;
        MPI_Recv(&received_value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == ANNOUNCEMENT) {
            std::cout << "Rank: " << status.MPI_SOURCE << " est élu avec la valeur: " << received_value << " - Depuis " << rang << std::endl;
            break;
        } else if (received_value == valeur && isInitiateur) {
            for (int i = 0; i < size; i++) {
                MPI_Send(&valeur, 1, MPI_INT, i, ANNOUNCEMENT, MPI_COMM_WORLD);
            }
        } else if (received_value < valeur && isInitiateur) {
            std::cout << "Rank: " << rang << " de valeur " << valeur << " a détruit le jeton de valeur: " << received_value << std::endl;
        } else {
            MPI_Send(&received_value, 1, MPI_INT, (rang + 1) % size, NORMAL, MPI_COMM_WORLD);
        }
    }
}

void checkInitiateur() {
    int initiateurExists = isInitiateur;

    if (rang == 0) {
        MPI_Send(&isInitiateur, 1, MPI_INT, (rang + 1) % size, CHECK, MPI_COMM_WORLD);
    }

    MPI_Recv(&initiateurExists, 1, MPI_INT, MPI_ANY_SOURCE, CHECK, MPI_COMM_WORLD, &status);

    if (rang == 0) {
        if (!initiateurExists) {
            std::cout << "Erreur: pas d'initiateur!" << std::endl;
            exit(1);
        }
    } else {
        int result = initiateurExists | isInitiateur;
        MPI_Send(&result, 1, MPI_INT, (rang + 1) % size, CHECK, MPI_COMM_WORLD);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);
    valeur = (rang + 5) % size; // Sûrement améliorable

    srand(getpid());
    isInitiateur = rand() % 2;
    checkInitiateur();

    if (isInitiateur) {
        std::cout << "Rank: " << rang << " est initiateur avec la valeur: " << valeur << std::endl;
    }

    algo();

    MPI_Finalize();
    return 0;
}