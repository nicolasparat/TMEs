#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAGINIT 0
#define NB_SITE 6

#define DIAMETRE 3		/* !!!!! valeur a initialiser !!!!! */

void simulateur(void) {
   int i;

   /* nb_voisins_in[i] est le nombre de voisins entrants du site i */
   /* nb_voisins_out[i] est le nombre de voisins sortants du site i */
   int nb_voisins_in[NB_SITE+1] = {-1, 2, 1, 1, 2, 1, 1};
   int nb_voisins_out[NB_SITE+1] = {-1, 2, 1, 1, 1, 2, 1};

   int min_local[NB_SITE+1] = {-1, 4, 7, 1, 6, 2, 9};

   /* liste des voisins entrants */
   int voisins_in[NB_SITE+1][2] = {{-1, -1},
				{4, 5}, {1, -1}, {1, -1},
				{3, 5}, {6, -1}, {2, -1}};
                               
   /* liste des voisins sortants */
   int voisins_out[NB_SITE+1][2] = {{-1, -1},
				{2, 3}, {6, -1}, {4, -1},
				{1, -1}, {1, 4}, {5,-1}};

   for(i=1; i<=NB_SITE; i++){
      MPI_Send(&nb_voisins_in[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(&nb_voisins_out[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(voisins_in[i], nb_voisins_in[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(voisins_out[i], nb_voisins_out[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD); 
   }
}

/******************************************************************************/

void calcul_min(int rang) {
    int min_local;
    int nb_voisins_in;
    int nb_voisins_out; 
    int voisins_in[2];
    int voisins_out[2];

    MPI_Recv(&nb_voisins_in, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
    MPI_Recv(&nb_voisins_out, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
    MPI_Recv(voisins_in, nb_voisins_in, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
    MPI_Recv(voisins_out, nb_voisins_out, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&min_local, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int j = 0; j < DIAMETRE; j++) {
        for (int i = 0; i < nb_voisins_out; i++) {
            MPI_Send(&min_local, 1, MPI_INT, voisins_out[i], TAGINIT, MPI_COMM_WORLD);
        }
        
        for (int i = 0; i < nb_voisins_in; i++) {
            int val;
            MPI_Recv(&val, 1, MPI_INT, MPI_ANY_SOURCE, TAGINIT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (val < min_local) { min_local = val; }
        }
    }

    std::cout << "Le min local sur " << rang << " est " << min_local << std::endl;
}

int main (int argc, char* argv[]) {
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
