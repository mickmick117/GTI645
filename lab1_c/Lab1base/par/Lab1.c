#include "mpi.h"
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

/*void P1Sequentiel(int initValue, int nbItterations)
{
	initMatrixValues(initValue);
	if (rank == 0) {
		for (int k = 1; k <= nbItterations; k++)
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					MPI_Send(&k, 1, MPI_INT, j + (i*8), 0, MPI_COMM_WORLD);
					usleep(WAIT_TIME);
					//printf("rank %i working on element %i\n", rank, i);
					matrix[i][j] = matrix[i][j] + (i + j) * k;
				}
			}
		}
	}
}*/

void P2Sequentiel(int initValue, int nbItterations)
{
	
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

	initMatrixValues(initValue);
	
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	printf("worldSize: %d\n", world_size);

	

	
	clock_t begin = clock();
	// Faire le tp
	
	int token;
	if (rank != 0)
	{
		MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   // printf("Process %d received token %d from process %d\n", rank, token, rank - 1);
		int i = rank/8;
		int j = rank%8;
		for (int k = 1; k <= nbItterations; k++)
		{
			matrix[i][j] = matrix[i][j] + (i + j) * k;
		}
		printMatrix();
	} 
	else 
	{
		token = -1;
	}
	
	MPI_Send(&token, 1, MPI_INT, (rank + 1) % world_size, 0, MPI_COMM_WORLD);

	//processus 0 reçois le message du dernier processus
	if (rank == 0) 
	{
		MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printMatrix();
		printf("Time : %f\n", time_spent);
	}
//	
	/*
	if (rank == 0) {
		for (int k = 1; k <= nbItterations; k++)
		{
			if(k == 0)
			{
				initMatrixValues(initValue);
			}
			
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					MPI_Send(&k, 1, MPI_INT, j + (i*8), 0, MPI_COMM_WORLD);
					usleep(WAIT_TIME);
					//printf("rank %i working on element %i\n", rank, i);
					//matrix[i][j] = matrix[i][j] + (i + j) * k;
				}
			}
		}
	}

	int token;
	MPI_Recv(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	int i = rank/8;
	int j = rank%8;
	matrix[i][j] = matrix[i][j] + (i + j) * token;
	

	// Un seul process doit faire le print.
	if (rank == 0) {
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printMatrix();
		printf("Time : %f\n", time_spent);
	}
	*/

	MPI_Finalize();
	return 0;
}

