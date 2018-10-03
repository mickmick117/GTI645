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

void P1Parallele(int rank, int nbItterations, int initValue)
{
	int i = rank/8;
	int j = rank%8;
	
	
	for (int k = 1; k <= nbItterations; k++)
	{
		initValue = initValue + (i + j) * k;
	}
	
	if(rank =! 0)
	{
		MPI_Send(&initValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
}

void P2Parallele(int rank, int nbItterations, int initValue)
{
	int i = rank/8;
	int j = rank%8;
	
	if (j == 0)
	{
		for (int k = 1; k <= nbItterations; k++)
		{
			initValue = initValue + (i*k);
			MPI_Send(&initValue, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		int previousValue;
		for (int k = 1; k <= nbItterations; k++)
		{
			MPI_Recv(&previousValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			initValue = initValue + previousValue * k;
			if(j != 8-1)
			{
				MPI_Send(&initValue, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
			}
		}
	}
	
	if(rank =! 0)
	{
		MPI_Send(&initValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
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
	//initMatrixValues(initValue);

	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	clock_t begin = clock();
	// Faire le tp
	
	int token;
	
	if(nbProbleme == 1)
	{
		if (rank != 0)
		{	
			P1Parallele(rank, nbItterations, initValue);
		} 
		else 
		{
			P1Parallele(rank, nbItterations, initValue);
			
			for(int process=1; process < world_size; process++)
			{
				int i = process/8;
				int j = process%8;
				MPI_Recv(&matrix[i][j], 1, MPI_INT, process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			
			clock_t end = clock();
			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			printMatrix();
			printf("Time : %f\n", time_spent);
		}	
	}
	else if(nbProbleme == 2)
	{		
		if (rank != 0)
		{	
			P2Parallele(rank, nbItterations, initValue);
		} 
		else 
		{
			P2Parallele(rank, nbItterations, initValue);
			
			for(int process=1; process < world_size; process++)
			{
				int i = process/8;
				int j = process%8;
				MPI_Recv(&matrix[i][j], 1, MPI_INT, process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			
			clock_t end = clock();
			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			printMatrix();
			printf("Time : %f\n", time_spent);
		}	
	}
	
	MPI_Finalize();
	return 0;
}

