#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <time.h>

#define WAIT_TIME 1000


int main(int argc, char* argv[])
{
	if (argc != 4) {
		printf("Le programme prend trois paramètres de lancement dans cet ordre (\" prog c v n \") :\nc = Choix du problème à exécuter (1 ou 2);\nv = La valeur initiale des éléments de la matrice\nn = Le nombre d’altérations.");
		return 0;
	}

	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Je suis rank %d\n", rank);

	int nbProbleme = atoi(argv[1]);
	int initValue = atoi(argv[2]);
	int nbItterations = atoi(argv[3]);

	
	clock_t begin = clock();
	// Faire le tp
	

	// Un seul process doit faire le print.
	if (rank == 0) {
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Time : %f\n", time_spent);
	}
	

	MPI_Finalize();
	return 0;
}

