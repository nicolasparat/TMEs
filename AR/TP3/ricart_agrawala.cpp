#include <mpi.h>
#include <vector>
#include <iostream>
#include <algorithm>

#define MAX_CS 5

// Messages
#define REQUEST 1
#define REPLY 2
#define FIN 3

// States
#define DEMANDEUR 4
#define NON_DEMANDEUR 5
#define CRITICAL_SECTION 6

void request_cs();
void handle_messages();
void release_cs();

int size;
int rank;

int state = NON_DEMANDEUR;
int horloge = 0;
std::vector<int> waitlist;
int date_last_request;

int nb_replies;
int nb_fin = 0;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < MAX_CS; i++) {
        request_cs();

        while(nb_replies < size - 1) {
            handle_messages();
        }

        state = CRITICAL_SECTION;
        std::cout << "Entering CS - Process: " << rank << " - Clock: " << horloge << std::endl;
        release_cs();
    }

    std::cout << "Waiting to die - Process: " << rank << " - Clock: " << horloge << std::endl;
    
    int msg[2] = { FIN, horloge };

    for (int i = 0; i < size; i++) {
        if (i != rank) {    
            MPI_Send(msg, 2, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    }  

    while(nb_fin < size - 1) {
        handle_messages();
    }

    std::cout << "Dying - Process: " << rank << " - Clock: " << horloge << std::endl;

    MPI_Finalize();
}

void request_cs() {
    horloge += 1;
    date_last_request = horloge;
    state = DEMANDEUR;
    nb_replies = 0;

    std::cout << "Requesting CS - Process: " << rank << " - Clock: " << horloge << std::endl;

    int msg[2] = { REQUEST, horloge };

    for (int i = 0; i < size; i++) {
        if (i != rank) {    
            MPI_Send(msg, 2, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    }   
}

void handle_messages() {
    MPI_Status status;
    int recv_msg[2];

    MPI_Recv(recv_msg, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    int request_type = recv_msg[0];
    int request_clock = recv_msg[1];

    horloge = std::max(horloge, request_clock) + 1;

    switch(request_type) {
        case REQUEST: {
        bool is_anterior = (request_clock < date_last_request) || ((request_clock == date_last_request) && status.MPI_SOURCE < rank);
        bool is_non_prioritaire = state == DEMANDEUR && is_anterior;
        bool is_non_demandeur = state == NON_DEMANDEUR;

            if (is_non_demandeur || is_non_prioritaire) {
                horloge++;
                int msg[2] = { REPLY, horloge };
                MPI_Send(msg, 2, MPI_INT, status.MPI_SOURCE, 99, MPI_COMM_WORLD);
            } else {
                waitlist.push_back(status.MPI_SOURCE);
            }

            break;
        }
        case REPLY:
            nb_replies += 1;
            break;
        case FIN:
            nb_fin += 1;
            break;
    }
}

void release_cs() {
    horloge++;

    std::cout << "Releasing CS - Process: " << rank << " - Clock: " << horloge << std::endl;


    int msg[2] = { REPLY, horloge };

    for (int process : waitlist) {
        MPI_Send(msg, 2, MPI_INT, process, 99, MPI_COMM_WORLD);
    }

    waitlist.clear();
    state = NON_DEMANDEUR;
}