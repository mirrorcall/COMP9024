#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stack.h"
#include "queue.h"

static void panic(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
	if (argc == 1)
	{
		panic("Usage: make && ./problemb [question number] [any-parameters] or ./problemb -h");
	}

	if (strcmp(argv[1], "-h") == 0)
	{
		panic("./problemb 1 -------------- Problem 1\n"
 			  "./problemb 2 -------------- Problem 2\n"
			  "./problemb 3 [N1] [N2] [N3] Problem 3\n"
			  "./problemb 4 [N] ---------- Problem 4\n"
			  "./problemb 5 -------------- Problem 5\n"
			  "N.B. [N]s are potential parameters");
	}

	switch(atoi(argv[1]))
	{
		case 1:
			/* q1 modified inside the original code */
			system("cat stack.c");
			break;
		case 2:
		{
			/* q2 starts here */
			stackInit();		// initialise the stack

			printf("Enter a positive number:");
			int i, n = 0;
			while (n <= 0)		// prompt input & check if n > 0
			{
				scanf("%d", &n);
				if (n <= 0)
					printf("Please enter a postive number\n");
				else
					break;
			}

			for (i = 0; i < n; ++i)
			{
				int e = 0;
				printf("Enter a number:");
				scanf("%d", &e);
				push(e);		// push onto stack
			}

			while (!isEmpty())	// finally display the elements in the stack
				printf("%d\n", pop());
			break;
		}
		case 3:
		{
			/* q3 starts here */
			stackInit();
		
			int i;
			// skip the first argument - question number
			// in this case 3 (i.e. question 3)
			for (i = 2; i < argc; ++i)
				push(atoi(argv[i]));

			while (!isEmpty())
				printf("%d\n", pop());
			break;
		}
		case 4:
		{
			stackInit();
			int k = 2, n = atoi(argv[2]);
			while (n > 0)
			{
				push(n % k);
				n = n / k;
			}
			printf("Result: ");
			while (!isEmpty())
				printf("%d", pop());
			printf("\n");
			break;
		}
		case 5:
			system("cat queue.c");
			break;
		case 6:
		{
			int data[12] = { 5, 3, 6, 2, 7, 4, 9, 1, 8 };
			printf("data = %p\n", &data[0]);
			printf("data + 4 = %p\n", &data[0] + 4);
			printf("*data + 4 = %d\n", *data + 4);
			printf("*(data + 4) = %d\n", *(data + 4));
			printf("data[4] = %d\n", data[4]);
			printf("*(data + *(data + 3)) = %d\n", *(data + *(data + 3)));
			printf("data[data[2]] = %d\n", data[data[2]]);
			break;
		}
		case 8:
		{
			int length = 0;
			while (argv[2][length] != '\0')
				length++;
			int i, j = 0;
			for (i = length; i > 0; --i)
			{
				j = 0;
				while (argv[2] != '\0')
				{
					if (j == i) break;
					printf("%c", argv[2][j]);
					j++;
				}
				printf("\n");
			}

			// sample answer
			// char *end = argv[2];
			// while (*end != '\0') { // find address of terminating '\0'
			// 	 end++;
			//  }
			//  while (start != end) {
			// 	 printf("%s\n", start); // print string from start to '\0'
			// 	 end--; // move end pointer up
			// 	 *end = '\0'; // overwrite last char by '\0'
			//  }
			break;
		}
		default:
			panic("Given 8 problems in problem-set 1b.");
			break;
	}

	return 0;
}