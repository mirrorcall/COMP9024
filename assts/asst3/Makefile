# Makefile

CC=gcc
CFLAGS=-Werror -Wall

%.o:%.c
				$(CC) $(CFLAGS) -c $<

Asst3: MyTaskScheduler.o
				$(CC) $(CFLAGS) -o MyTaskScheduler MyTaskScheduler.o
