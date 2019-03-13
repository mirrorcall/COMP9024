#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>

static void panic(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

int * makeArrayOfInts(void);
void  func(int**);
unsigned long long * fibonacci(int);

int main(int argc, char const *argv[])
{
	if (argc < 2)
		panic("Usage: make && ./problem [question-number]");

	switch(atoi(argv[1]))
	{
		case 1:
		{
			int i;
			int *p;
			p = makeArrayOfInts();
			for (i = 0; i < 10; ++i)
				printf("%d\n", *(p + i));
			break;
		}
		case 2:
		{
			int *p;
			func(&p);
			*p = 6;
			/************************************************************
			 * Tested on the CSE machine - undefined behaviour
			 * malloc fucntion does not necessarily alloacted the exact
			 * number of memory as you asked. OS assigns pages of memory
			 * instead of simply exact amount of memory. So, os-dependent.
			 *************************************************************/
			// printf("Memory allocated: %zubyte(s)\n", malloc_usable_size(p));
			printf("Memory allocated: %zubyte(s)\n", malloc_size(p));
			printf("%d\n", *p);
			free(p);
			break;
		}
		case 3:
		{
			unsigned long long *fib;
			fib = fibonacci(atoi(argv[2]));
			printf("The %dth fibonacci item = %lld\n", atoi(argv[2]), fib[(atoi(argv[2])-1)]);
			free(fib);
			break;
		}
		default:
			break;
	}
	return 0;
}

int * makeArrayOfInts()
{
	// int arr[10] = { 0 };
	// The array is declared within function scope
	// so as to stack, it will be released as long
	// as current block goes out the scope
	static int arr[10] = { 0 };
	// sample answer
	// int *arr = malloc(sizeof(int) * 10);
	int i;
	for (i = 0; i < 10; ++i)
	{
		arr[i] = i;
	}

	return arr;
}

// void func(int *a)
// indirectly return a pointer that is to be changed
// in the function requires passing a pointer of pointer
void func(int **a)
{
	*a = malloc(sizeof(int));
}

unsigned long long * fibonacci(int n)
{
	unsigned long long *fibs = malloc(sizeof(unsigned long long) * n);
	int i;
	fibs[0] = 1;
	fibs[1] = 1;
	for (i = 2; i < n; ++i)
	{
		fibs[i] = fibs[i-2] + fibs[i-1];
		// printf("N%3d = %lld\n", i, fibs[i]);
	}

	return fibs;
}












