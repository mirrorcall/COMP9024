# Makefile

CC=gcc
CFLAGS=-Werror -Wall -g -Wunneeded-internal-declaration

%.o:%.c
				$(CC) $(CFLAGS) -c $<

Asst3: MyTaskScheduler.o
				$(CC) $(CFLAGS) -o MyTaskScheduler MyTaskScheduler.o
