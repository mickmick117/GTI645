#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#define BILLION 1E9
#define WAIT_TIME 50000

const int row = 12;
const int column = 12;
const int nbThread = 64;
int matrix [12][12];

void initMatrixValues(int initValue)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			matrix[i][j] = initValue;
		}
	}
}

void printMatrix()
{
	printf("\n");
	printf("\n");
	
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			printf("%d,",matrix[i][j]);
		}
		printf("\n");
	}
}

void setMatrixValue(int i, int j, int value)
{
	matrix[i][j] = value;
}

void probleme1Seq(int initValue, int iteration) 
{
	for(int k=1; k <= iteration; k++)
	{
		for(int i=0; i < row; i++)
		{
			for(int j=0; j < column; j++)
			{
				usleep(WAIT_TIME);
				if(k == 1)
				{
					setMatrixValue(i,j, initValue);
				}
				setMatrixValue(i,j, matrix[i][j] + (i + j));				
			}
		}
	}
}

void probleme2Seq(int initValue, int iteration)
{
	for(int k=1; k <= iteration; k++)
	{
		for(int i=0; i < row; i++)
		{
			for(int j=(column-1); j >= 0 ; j--)
			{
				usleep(WAIT_TIME);
				
				if(k == 1)
				{
					setMatrixValue(i,j, initValue);
				}
				
				if(j == (column-1))
				{
					setMatrixValue(i,j, matrix[i][j] + i);
				}
				else
				{
					setMatrixValue(i,j, matrix[i][j] + matrix[i][j+1]);
				}				
			}
		}
	}
}

void probleme1Par(int initValue, int iteration) 
{
	initMatrixValues(initValue);
	
	
	for(int k=1; k <= iteration; k++)
	{		
		#pragma omp parallel for
		for(int thread= 0; thread < nbThread; thread++)
		{
			int nbElementsEach = (row * column) / nbThread;
			int nbThreadOneElementMore = (row * column) % nbThread;
			
			if(thread < nbThreadOneElementMore)
			{
				nbElementsEach++;
			}
			
			for(int element = 0; element < nbElementsEach; element++)
			{
				usleep(WAIT_TIME);		
				
				int index = thread + (element*nbThread);
				int i = index/row;
				int j = index%column;
				
				setMatrixValue(i,j, matrix[i][j] + (i + j));	
			}				
		}
	}
}

void probleme2Par(int initValue, int iteration) 
{
	initMatrixValues(initValue);
	
	for(int k=1; k <= iteration; k++)
	{
		#pragma omp parallel for
		for(int i= 0; i < row; i++)
		{			
			for(int j=(column-1); j >= 0 ; j--)
			{
				usleep(WAIT_TIME);		
				
				if(j == (column-1))
				{
					setMatrixValue(i,j, matrix[i][j] + i);
				}
				else
				{
					setMatrixValue(i,j, matrix[i][j] + matrix[i][j+1]);
				}	
			}
		}
	}
}

int main(int argc, char const *argv[])
{
    // Lire les arguments.
    if (argc != 4)
	{
        printf("\nNombre de paramètre invalide en entrée.\n");
        printf("p : Le numéro du problème à résoudre, soit 1 ou 2.\n");
		printf("v : La valeur de toutes les cellules de la matrice au temps k = 0.\n");
		printf("n : La valeur de k pour laquelle on veut calculer l’état de la matrice et afficher le résultat.\n");
        return 1;
    }
    int p = atof(argv[1]);
    int v = atof(argv[2]);
    int n = atof(argv[3]);
    printf("\nChoix du problème à exécuter: %d", p);
    printf("\nLa valeur initiale des éléments de la matrice 𝑀: %d", v);
    printf("\nLe nombre d’altérations: %d", n);

	
	// Timers
    struct timespec requestStart, requestEnd;
	double tempExecutionParallele, tempExecutionSequentiel;

	
	
	// Exemple avec probleme 1 
	
	//Sequentiel
	clock_gettime(CLOCK_REALTIME, &requestStart);
	
	if(p == 1)
		probleme1Seq(v, n);
	else
		probleme2Seq(v, n);
	
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	tempExecutionSequentiel = (requestEnd.tv_sec - requestStart.tv_sec) + (requestEnd.tv_nsec - requestStart.tv_nsec) / BILLION;
    
	
	//Parallele
	clock_gettime(CLOCK_REALTIME, &requestStart);
	
	if(p == 1)
		probleme1Par(v, n);
	else
		probleme2Par(v, n);
	
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	tempExecutionParallele = (requestEnd.tv_sec - requestStart.tv_sec) + (requestEnd.tv_nsec - requestStart.tv_nsec) / BILLION;
    
	
	printMatrix();
    printf("\nDurée d'exécution séquentiel: %f seconde(s)", tempExecutionSequentiel);
    printf("\nDurée d'exécution parallèle: %f seconde(s)", tempExecutionParallele);
    printf("\nNombre de thread: %d", omp_get_max_threads());
    printf("\nAccélération: %f", tempExecutionSequentiel - tempExecutionParallele); // TODO mettre un beeau calcule pour donner la reponse 
    printf("\nEfficacité: %f\n", tempExecutionSequentiel/tempExecutionParallele); // TODO mettre un beeau calcule pour donner la reponse

    return 0;
}
