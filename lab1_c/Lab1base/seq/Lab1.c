//Ne pas inclure mpi.h car cette partie est compiler avec gcc et non mpicc
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <time.h>

#define WAIT_TIME 1000


int matrix[8][8];


void initMatrixValues(int initValue)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			matrix[i][j] = initValue;
		}
	}
}

void printMatrix()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			printf("%d,",matrix[i][j]);
		}
		printf("\n");
	}
}

void P1Sequentiel(int initValue, int nbItterations)
{
	initMatrixValues(initValue);

	for (int k = 1; k <= nbItterations; k++)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				usleep(WAIT_TIME);
				matrix[i][j] = matrix[i][j] + (i + j) * k;
			}
		}
	}
}

void P2Sequentiel(int initValue, int nbItterations)
{
	initMatrixValues(initValue);

	for (int k = 1; k <= nbItterations; k++)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				usleep(WAIT_TIME);

				if (j == 0)
				{
					matrix[i][j] = matrix[i][j] + (i*k);
				}
				else
				{
					matrix[i][j] = matrix[i][j] + matrix[i][j - 1] * k;
				}
			}
		}
	}
	
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
	struct timespec begin;
	struct timespec end;

	//clock_t begin = clock();
	clock_gettime(CLOCK_REALTIME, &begin);

	
	// si rpobleme 1 faire 1 sinon faire 2
	if(nbProbleme == 1)
		P1Sequentiel(initValue, nbItterations);
	else
		P2Sequentiel(initValue, nbItterations);
	
	clock_gettime(CLOCK_REALTIME, &end);
	printMatrix();
	printf("%ld.%09ld\n", (long)(end.tv_sec - begin.tv_sec), end.tv_nsec - begin.tv_nsec);
	
	/*clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time : %f\n", time_spent);*/
	
	return 0;
}


