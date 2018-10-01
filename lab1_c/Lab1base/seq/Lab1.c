//Ne pas inclure mpi.h car cette partie est compiler avec gcc et non mpicc
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <time.h>

#define WAIT_TIME 1000




void P1Sequentiel(int initValue, int nbItterations) {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			usleep(WAIT_TIME);
}


int main(int argc, char* argv[])
{
	if (argc != 4) {
		printf("Le programme prend trois paramètres de lancement dans cet ordre (\" prog c v n \") :\nc = Choix du problème à exécuter (1 ou 2);\nv = La valeur initiale des éléments de la matrice\nn = Le nombre d’altérations.");
		return 0;
	}

	int nbProbleme = atoi(argv[1]);
	int initValue = atoi(argv[2]);
	int nbItterations = atoi(argv[3]);

	clock_t begin = clock();
	
	// si rpobleme 1 faire 1 sinon faire 2
	if(nbProbleme == 1)
		P1Sequentiel(initValue, nbItterations);
	
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time : %f\n", time_spent);
	
	return 0;
}

