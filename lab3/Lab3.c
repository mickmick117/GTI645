#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/time.h>
#include <time.h>
//#include <unistd.h>

#define WAIT_TIME 5
const int principalThread = 0;

void initMatrixValues(int row, int column, double **matrix);
void printMatrix(int row, int column, double **matrix);
void DiffusionParallele(int rank);
void DiffusionSequentiel();


int main(int argc, char* argv[])
{
	if (argc != 6) {
		printf("Le programme prend 5 paramètres de lancement dans cet ordre (\" prog n m np td h \") :\nn = le nombre de ligne;\nm = Le nombre de colonne\nnp = Le nombre de pas de temps \ntd = le temps discrétisé \nh = la taille d'un côté d'une subdivision");
		return 0;
	}
	
	int nbLignes = atoi(argv[1]);
	int nbColonnes = atoi(argv[2]);
	int nbPasDeTemps = atoi(argv[3]);
	int tempsDiscretise = atoi(argv[4]);
	int tailleCoteSubdivision = atoi(argv[5]);
	double timeStart, timeEnd, tempExecutionParallele, tempExecutionSequentiel;
	struct timeval tp;
	
	double **matrix = (double **) malloc (nbLignes*sizeof(double*));
	
	for(int i =0; i < nbLignes; i++)
	{
		matrix[i] = (double *) malloc(nbColonnes*sizeof(double));
	}
	//double matrix[nbLignes][nbColonnes];
	
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	//initialisation de ma la matrice
	initMatrixValues(nbLignes, nbColonnes, matrix);
		
	if(rank == principalThread) 
	{
		gettimeofday (&tp, NULL); // Début du chronomètre parallèle
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6; 
	}
	
	DiffusionParallele(rank);
	
	if(rank == principalThread) 
	{
		gettimeofday (&tp, NULL); // Fin du chronomètre parallèle
		timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		tempExecutionParallele = timeEnd - timeStart; //Temps d'exécution en secondes
		
		// code sequentielle
		gettimeofday (&tp, NULL); // Début du chronomètre séquentiel
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6; 
		
		DiffusionSequentiel();
		
		gettimeofday (&tp, NULL); // Fin du chronomètre séquentiel
		timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		tempExecutionSequentiel = timeEnd - timeStart; //Temps d'exécution en secondes		
		
		//print stats
		printMatrix(nbLignes, nbColonnes, matrix);
		printf("\nDurée d'exécution séquentiel: %f seconde(s)", tempExecutionSequentiel);
		printf("\nDurée d'exécution parallèle: %f seconde(s)", tempExecutionParallele);
	}
	
	MPI_Finalize();
	return 0;
}


void initMatrixValues(int row, int column, double **matrix)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			matrix[i][j] = i * (column-i-1) * (2.0*i/column) * j * (row-j-1) * (1.0*j/row);
		}
	}	
}

void printMatrix(int row, int column, double **matrix)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			printf("%f,",matrix[i][j]);
		}
		printf("\n");
	}
}

void DiffusionParallele(int rank)
{
	
}

void DiffusionSequentiel()
{
	usleep(WAIT_TIME);
}

