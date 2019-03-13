// modified stack data structure that takes integer in

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

#define MAXITEMS	10

struct
{
	// char item[MAXITEMS];
	int  item[MAXITEMS];
	int	 top;
} stackObject;

void stackInit()
{
	stackObject.top = -1;
}

int isEmpty()
{
	return( stackObject.top < 0 );
}

void push(int val)
{
	assert(stackObject.top < MAXITEMS-1);
	stackObject.top++;
	int i = stackObject.top;
	// can be replaced by
	// int i = ++stackObject.top;
	stackObject.item[i] = val;
}

int pop()
{
	assert(stackObject.top > -1);
	int i = stackObject.top;
	int val = stackObject.item[i];
	stackObject.top--;
	return val;
}
