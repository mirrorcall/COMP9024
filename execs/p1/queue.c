#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

#define MAXITEMS	10

struct
{
	int  item[MAXITEMS];
	int	 size;
} queueObject;

void queueInit()
{
	queueObject.size = 0;
}

int qisEmpty()
{
	return( queueObject.size > 0 );
}

void enqueue(int val)
{
	assert(queueObject.size < MAXITEMS);
	int i = queueObject.size++;
	queueObject.item[i] = val;
}

int dequeue()
{
	assert(queueObject.size > 0);
	int i, ret = queueObject.item[0];
	for (i = 1; i <= queueObject.size; ++i)
		queueObject.item[i-1] = queueObject.item[i];
	queueObject.size--;
	return ret;
}

void show()
{
	int i;
	for (i = 0; i < queueObject.size; ++i)
		printf("%d ", queueObject.item[i]);
}