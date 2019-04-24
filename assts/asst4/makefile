# makefile
CC=gcc
CFLAGS=-Werror -Wall -Wno-unused-function -g

%.o:%.c
		$(CC) $(CFLAGS) -c $<
MyGraph: MyGraph.o
		$(CC) $(CFLAGS) -o MyGraph MyGraph.o

clean:
		rm *.o
		rm MyGraph