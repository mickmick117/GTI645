PROG = Lab3
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS= -fPIC -m64 -Wall
CC = mpicc

all: $(PROG)

Lab3.o: Lab3.c
	$(CC) $(CFLAGS) -c Lab3.c

Lab3 : Lab3.o
	$(CC) $(LFLAGS) Lab3.o -o Lab3

run:
	mpirun --hostfile hostfile -np 24 Lab3

ps:
	ps -fu $$USER

clean:
	/bin/rm -f *~
	/bin/rm -f *.o

test-mpi:
	mpirun --hostfile hostfile -np 4 Lab3
