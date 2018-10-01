PROG = Lab1
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS= -fPIC -m64 -Wall
CC = gcc

all: $(PROG)

Lab1.o: Lab1.c
	$(CC) $(CFLAGS) -c Lab1.c

Lab1 : Lab1.o
	$(CC) $(LFLAGS) Lab1.o -o Lab1

run:
	./Lab1

ps:
	ps -fu $$USER

clean:
	/bin/rm -f *~
	/bin/rm -f *.o
