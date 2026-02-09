#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>

#define NB_MEALS 4

#define CHOPSTICK_REQUEST 1
#define GRANTED_REQUEST 2
#define DECLINED_REQUEST 3

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

            MPI_Status status;
            MPI_Recv(NULL, 0, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // demande de baguette -> checker id
            // id inférieur -> non, supérieur -> oui
            if (status.MPI_TAG == CHOPSTICK_REQUEST) {
                if (status.MPI_SOURCE < rank) {
                    // refuser
                    MPI_Send(NULL, 0, MPI_BYTE, (rank + 1) % size, 99, MPI_COMM_WORLD);
                } else {
                    // accepter
                    MPI_Send(NULL, 0, MPI_BYTE, (rank + 1) % size, 99, MPI_COMM_WORLD);
                }
            }

            if(refus) {
                // redemander
                MPI_Send(NULL, 0, MPI_BYTE, (rank + 1) % size, 99, MPI_COMM_WORLD);
            }

            if (status.MPI_TAG == GRANTED_REQUEST) {
                if (status.MPI_SOURCE < rank) {
                    hasRightFork = true
                } else {
                    hasLeftFork = true
                }

                // On attend le 2eme message si on n'a pas reçu les 2 baguettes
                if (!hasLeftFork || ! hasRightFork) {
                    continue
                }
            }

            state = EATING;
        } 
        
        if (state == EATING) {
            printf("Philosopher %d is eating.\n", rank);
            fflush(stdout);

            sleep(1);
            counter++;

            // Si demande de baguette reçue
            // Donner la baguette
            if (demande) {
                MPI_Send(NULL, 0, MPI_BYTE, (rank + 1) % size, 99, MPI_COMM_WORLD);
            }

            state = THINKING;
        }
        
        if (state == THINKING) {
            printf("Philosopher %d is thinking.\n", rank);
            fflush(stdout);

            sleep(1);
            state = HUNGRY;

            // Demander fourchettes
            if (!hasLeftFork) {   
                MPI_Send(NULL, 0, MPI_BYTE, (rank + 1) % size, 99, MPI_COMM_WORLD);
            }

            if (!hasRightFork) {
                MPI_Send(NULL, 0, MPI_BYTE, (rank - 1) % size, 99, MPI_COMM_WORLD);
            }
        }
    }

    // Cleanup (donner les baguettes au voisin avant de mourir)

    MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, (rank + 1) % size, 99, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}