#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAGINIT 0
#define TAGDOWN 1
#define TAGUP 2
#define NB_SITE 6

void simulateur(void) {
    int i;

    /* nb_voisins[i] est le nombre de voisins du site i */
    int nb_voisins[NB_SITE+1] = {-1, 3, 3, 2, 3, 5, 2};
    int min_local[NB_SITE+1] = {-1, 12, 11, 8, 14, 5, 17};

    /* liste des voisins */
    int voisins[NB_SITE+1][5] = {{-1, -1, -1, -1, -1},
        {2, 5, 3, -1, -1}, {4, 1, 5, -1, -1}, 
        {1, 5, -1, -1, -1}, {6, 2, 5, -1, -1},
        {1, 2, 6, 4, 3}, {4, 5, -1, -1, -1}
    };

    for(i=1; i<=NB_SITE; i++){
        MPI_Send(&nb_voisins[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
        MPI_Send(voisins[i], nb_voisins[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
        MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD); 
    }
}

int calcul_min(int rang) {
    int nb_voisins;
    int voisins[5];
    int min_local;

    MPI_Status status;

    MPI_Recv(&nb_voisins, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);
    MPI_Recv(voisins, nb_voisins, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);
    MPI_Recv(&min_local, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);
    
    // Placeholder that should not be modified
    int to_send = 0;

    if (rang == 1) {
        int result = -1;

        for (int voisin : voisins) {
            if (voisin != -1) {
                MPI_Send(&to_send, 1, MPI_INT, voisin, TAGDOWN, MPI_COMM_WORLD);
            }
        }

        for (int i = 0; i < nb_voisins; i++) {
            int temp_result;
            MPI_Recv(&temp_result, 1, MPI_INT, MPI_ANY_SOURCE, TAGUP, MPI_COMM_WORLD, &status);
            if (result == -1) {
                result = temp_result;
            } else if (temp_result < result) {
                result = temp_result;
            }
        }

        printf("Result is : %d\n", result);
        return result;
    } else {
        int minimum = min_local;
        int pere = -1;

        for (int i = 0; i < nb_voisins; i++) {  
            int val;
            MPI_Recv(&val, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == TAGUP && val < minimum) {
                minimum = val;
            }

            if (pere == -1) {
                pere = status.MPI_SOURCE;

                for (int voisin : voisins) {
                    if (voisin != pere && voisin != -1) {
                        int to_send = 0;
                        MPI_Send(&to_send, 1, MPI_INT, voisin, TAGDOWN, MPI_COMM_WORLD);
                    }
                }
            }
        }

        MPI_Send(&minimum, 1, MPI_INT, pere, TAGUP, MPI_COMM_WORLD);
        return 0;
    }
}

int main(int argc, char *argv[]) {
    int nb_proc,rang;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);

    if (nb_proc != NB_SITE+1) {
        printf("Nombre de processus incorrect !\n");
        MPI_Finalize();
        exit(2);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rang);

    if (rang == 0) {
        simulateur();
    } else {
        calcul_min(rang);
    }

    MPI_Finalize();
    return 0;
}