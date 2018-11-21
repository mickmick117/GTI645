#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WAIT_TIME 5
#define EXIT 9999
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

const int principalThread = 0;
const int valuesArraySize = 7;

int nbLignes;
int nbColonnes;
int nbPasDeTemps;
double tempsDiscretise;
double hauteur;

void initMatrixValues(int row, int column, double **matrix);
void printMatrix(int row, int column, double **matrix);
void DiffusionParallele(int row, int column, double **matrix, int rank);
void ThreadCalculation();
void DiffusionSequentiel(int row, int column, double **matrix);


int main(int argc, char* argv[])
{
	if (argc != 6) {
		printf("Le programme prend 5 paramètres de lancement dans cet ordre (\" prog n m np td h \") :\nn = le nombre de ligne;\nm = Le nombre de colonne\nnp = Le nombre de pas de temps \ntd = le temps discrétisé \nh = la taille d'un côté d'une subdivision");
		return 0;
	}
	
	nbLignes = atoi(argv[1]);
	nbColonnes = atoi(argv[2]);
	nbPasDeTemps = atoi(argv[3]);
	tempsDiscretise = atof(argv[4]);
	hauteur = atof(argv[5]);
	double timeStart, timeEnd, tempExecutionParallele, tempExecutionSequentiel;
	struct timeval tp;
	
	double **matrix = (double **) malloc (nbLignes*sizeof(double*));
	
	for(int i =0; i < nbLignes; i++)
	{
		matrix[i] = (double *) malloc(nbColonnes*sizeof(double));
	}
	
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	//initialisation de ma la matrice
	initMatrixValues(nbLignes, nbColonnes, matrix);
			
	if(rank == principalThread) 
	{
		printf("\nMATRICE INITIALE: \n\n");
		printMatrix(nbLignes, nbColonnes, matrix);
		
		gettimeofday (&tp, NULL); // Début du chronomètre parallèle
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6; 
	}
	
	//DiffusionParallele(nbLignes, nbColonnes, matrix, rank);
	
	if(rank == principalThread) 
	{
		gettimeofday (&tp, NULL); // Fin du chronomètre parallèle
		timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		tempExecutionParallele = timeEnd - timeStart; //Temps d'exécution en secondes
		
		// code sequentielle
		gettimeofday (&tp, NULL); // Début du chronomètre séquentiel
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6; 
		
		DiffusionParallele(nbLignes, nbColonnes, matrix, 1);
		
		gettimeofday (&tp, NULL); // Fin du chronomètre séquentiel
		timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		tempExecutionSequentiel = timeEnd - timeStart; //Temps d'exécution en secondes		
		
		//print stats
		printf("MATRICE FINALE: \n\n");
		printMatrix(nbLignes, nbColonnes, matrix);
		printf("\nDurée d'exécution séquentiel: %f seconde(s)", tempExecutionSequentiel);
		printf("\nDurée d'exécution parallèle: %f seconde(s) \n\n", tempExecutionParallele);
	}
	else
	{
		ThreadCalculation();
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
			matrix[i][j] = i * (row-i-1) * (2.0*i/row) * j * (column-j-1) * (1.0*j/column);
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

void DiffusionParallele(int row, int column, double **matrix, int rank)
{
	double values[valuesArraySize];
	double returnValues [3];
	int threadIndex;
	int threadNumber = 1;
	column -= 2;
	row -= 2;

	for (int k = 0; k < nbPasDeTemps; k++)
	{		
		for (int y = 1; y <= (column + row) - 1; y++)
		{
			threadIndex = 1;
			int maxVal = y - (column - 1);
			for (int x = MAX(1, maxVal); x <= MIN(y, row); x++)
			{
				//printf("threadIndex: %d,",threadIndex);
				//printf("\n");
				
				threadNumber = (threadIndex - 1) % 63 + 1;				
				threadIndex++;
				
				//printf("threadNumber: %d,",threadNumber);
				//printf("\n");
				
				int yy = (y-x)+1;				
				//set values
				values[0] = x;
				values[1] = yy;
				
				values[2] = matrix[x][yy];
				values[3] = matrix[x - 1][yy];
				values[4] = matrix[x + 1][yy];
				values[5] = matrix[x][yy - 1];
				values[6] = matrix[x][yy + 1];			
	
				MPI_Send(values, valuesArraySize, MPI_DOUBLE, threadNumber, 0, MPI_COMM_WORLD);
				//matrix[x][yy] = (1 - 4 * tempsDiscretise / (hauteur*hauteur))
				//	* matrix[x][yy] + (tempsDiscretise / (hauteur*hauteur))
				//	* (matrix[x - 1][yy] + matrix[x + 1][yy] + matrix[x][yy - 1] + matrix[x][yy + 1]);
			}
			
			for (int i = 0; i < (threadIndex-1); i++)
			{
				//printf("recv: %d \n",i);
				MPI_Recv (returnValues, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				//printf("%f,", returnValues[0]);
				//printf("%f \n \n", returnValues[1]);
				matrix[(int)returnValues[0]][(int)returnValues[1]] = returnValues[2];
			}
		}
	}
	
	//tell the thread to end
	values[0] = EXIT;
	
	for(int j=1; j < 64; j++)
	{
		MPI_Send(values, valuesArraySize, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
	}	
}

void ThreadCalculation()
{
	int loop = 1; // true
	int returnValue;
	double values[valuesArraySize];
	double returnValues [3];

	while(loop == 1)
	{
		MPI_Recv (values, valuesArraySize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(values[0] != EXIT)
		{
			usleep(WAIT_TIME);
			returnValue = (1 - 4 * tempsDiscretise / (hauteur*hauteur))
						* values[2] + (tempsDiscretise / (hauteur*hauteur))
						* (values[3] + values[4] + values[5] + values[6]);
			// set values
			returnValues [0] = values[0];			
			returnValues [1] = values[1];
			returnValues [2] = returnValue;
			MPI_Send(returnValues, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
		else
		{
			loop = 0; // false
		}
	}
}

void DiffusionSequentiel(int row, int column, double **matrix)
{
	for(int k = 0; k < nbPasDeTemps; k++)
	{
		for (int i = 1; i < row-1; i++)
		{
			for (int j = 1; j < column-1; j++)
			{
				usleep(WAIT_TIME);
				matrix[i][j] = (1- 4 * tempsDiscretise/(hauteur*hauteur))
								* matrix[i][j] + (tempsDiscretise/(hauteur*hauteur)) 
								* (matrix[i-1][j] + matrix[i+1][j] + matrix[i][j-1] + matrix[i][j+1]);
			}
		}
	}	
}

