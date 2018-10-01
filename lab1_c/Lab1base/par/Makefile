PROG = Lab1
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS= -fPIC -m64 -Wall
CC = mpicc

all: $(PROG)

Lab1.o: Lab1.c
	$(CC) $(CFLAGS) -c Lab1.c

Lab1 : Lab1.o
	$(CC) $(LFLAGS) Lab1.o -o Lab1

run:
	mpirun --hostfile hostfile -np 24 Lab1

ps:
	ps -fu $$USER

clean:
	/bin/rm -f *~
	/bin/rm -f *.o

test-mpi:
	mpirun --hostfile hostfile -np 4 Lab1
