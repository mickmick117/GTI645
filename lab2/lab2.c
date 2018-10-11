#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#define BILLION 1E9
#define WAIT_TIME 50000

const int row = 12;
const int column = 12;
int [row][column] matrix;

void setMatrixValue(int i, int j, int value)
{
	matrix[i][j] = value;
}

void probleme1Seq(int prob, int initValue, int iteration) 
{
	
	if(prob == 1)
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
						setMatrixValue(i,j, initValue)
					}
					setMatrixValue(i,j, matrix[i][j] + (i + j))					
				}
			}
		}
	}
	else if (prob == 2)
	{
		for(int k=1; k <= iteration; k++)
		{
			for(int i=0; i < row; i++)
			{
				for(int j=column; j > 0 ; j--)
				{
					usleep(WAIT_TIME);
					
					if(k == 1)
					{
						setMatrixValue(i,j, initValue)
					}
					
					if(j == column)
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
}

void probleme1Par(int prob, int initValue, int iteration) 
{
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
	probleme1Seq(p, v, n);
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	tempExecutionSequentiel = (requestEnd.tv_sec - requestStart.tv_sec) + (requestEnd.tv_nsec - requestStart.tv_nsec) / BILLION;
    
	
	//Parallele
	/*clock_gettime(CLOCK_REALTIME, &requestStart);
	probleme1Par();
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	tempExecutionParallele = (requestEnd.tv_sec - requestStart.tv_sec) + (requestEnd.tv_nsec - requestStart.tv_nsec) / BILLION;
    */
	

    printf("\nDurée d'exécution séquentiel: %f seconde(s)", tempExecutionSequentiel);
  //  printf("\nDurée d'exécution parallèle: %f seconde(s)", tempExecutionParallele);
  //  printf("\nNombre de thread: %d", omp_get_max_threads());
    printf("\nAccélération: %f", 0.0); // TODO mettre un beeau calcule pour donner la reponse 
    printf("\nEfficacité: %f\n", 0.0); // TODO mettre un beeau calcule pour donner la reponse

    return 0;
}
