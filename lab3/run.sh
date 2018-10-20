##################################################################
# Version			Lab3
# Commande			./run.sh c p n
# 					n: 		le nombre de lignes;
# 					m: 		le nombre de colonnes;
# 					np: 	le nombre de pas de temps;
#					td: 	le temps discrétisé;
# 					h: 		la taille d'un côté d'une subdivision;
##################################################################

#!/bin/bash
mpirun -np 64 Lab3 $1 $2 $3 $4 $5