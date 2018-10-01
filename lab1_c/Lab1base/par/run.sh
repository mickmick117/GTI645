##################################################################
# Version			Parallele
# Commande			./run.sh c p n
# 					c: 		choix du probleme (1 ou 2);
# 					p: 		valeur initiale des elements de la matrice M^0;
# 					n: 		nombre d'alterations
##################################################################

#!/bin/bash
mpirun -np 64 Lab1 $1 $2 $3