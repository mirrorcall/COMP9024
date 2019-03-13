# Makefile

CC = gcc
CFLAGS = -Werror -Wall
SRCA = problema.c
SRCB = problemb.c stack.c queue.c
PBOBJ = $(SRCB:.c=.o)
HSRC = stack.h queue.h

%o:%c $(HSRC)
		$(CC) $(CFLAGS) -c $<

all: problema problemb

problema:
		$(CC) $(CFLAGS) -o problema $(SRCA)

problemb: $(PBOBJ)
		$(CC) $(CFLAGS) -o problemb $(PBOBJ)

clean:
		rm -f problema problemb *.o