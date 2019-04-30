# makefile
MODE=dev
CC=gcc
CFLAGS=-Werror -Wall

ifeq ($(MODE), dev)
	CFLAGS += -Wno-unused-function -g -D DEBUG
endif

%.o:%.c
		$(CC) $(CFLAGS) -c $<
MyGraph: MyGraph.o
		$(CC) $(CFLAGS) -o MyGraph MyGraph.o

clean:
		rm *.o
		rm MyGraph