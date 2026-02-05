#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>

#define NB_MEALS 4

typedef enum { THINKING, HUNGRY, EATING } state_t;

int main(int argc, char *argv[]) {
    int size;
    int rank;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int counter = 0;
    bool hasRightFork = true;
    bool hasLeftFork = false;   
    state_t state = THINKING;
    
    while (counter < NB_MEALS) {
        if (state == HUNGRY) {
            printf("Philosopher %d is hungry.\n", rank);
            fflush(stdout);

            MPI_Recv();
            // demande de baguette -> checker id
            // id inférieur -> non, supérieur -> oui

            // refus de baguette
            // 

            // acceptation de baguette
            // if not 2 baguettes continue

            state = EATING;
        } 
        
        if (state == EATING) {
            printf("Philosopher %d is eating.\n", rank);
            fflush(stdout);

            sleep(1);
            counter++;

            state = THINKING;
        }
        
        if (state == THINKING) {
            printf("Philosopher %d is thinking.\n", rank);
            fflush(stdout);

            sleep(1);
            state = HUNGRY;

            // Demander fourchettes
            if (!hasLeftFork) {   
                MPI_Send();
            }

            if (!hasRightFork) {
                MPI_Send();
            }
        }
    }

    // Cleanup (donner la baguette au voisin avant de mourir)

    MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, (rank + 1) % size, 99, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}