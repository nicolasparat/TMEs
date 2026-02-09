#include <mpi.h>

#define MAX_CS 5

int main(int argc, char *argv[]) {
    // int nb_proc = argv[1];    
    MPI_Init(&argc, &argv);
    
    int horloge = 0;

    for (int i = 0; i < MAX_CS; i++) {
        
    }
}

void request_sc() {
    horloge += 1;
    MPI_Send();
}