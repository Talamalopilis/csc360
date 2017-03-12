#
# "makefile" for the CPU scheduler simulation.
#

CC=gcc
CFLAGS=-c -Wall -g

all: randomtasks cpusim

randomtasks.o: randomtasks.c
	$(CC) $(CFLAGS) randomtasks.c

cpusim.o: cpusim.c
	$(CC) $(CFLAGS) cpusim.c

randomtasks: randomtasks.o
	$(CC) randomtasks.o -o randomtasks

cpusim: cpusim.o
	$(CC) cpusim.o -o cpusim

clean:
	rm -rf *.o randomtasks cpusim
