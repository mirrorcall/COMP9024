#include <stdio.h>
#include <stdlib.h>

#define TRUE	1
#define FALSE	-1

#define MAX_Q1_BAKCWARDS	99999
#define MAX_Q1_FORWARDS		24999
#define MIN_Q1_FORWARDS		10000

#define MIN_Q6_ITEM		1
#define MAX_Q6_ITEM		10

#define MAX_Q8_SIZE		10

#define CUSTOM_MAX(a,b)		((a >= b) ? a : b)

typedef int 	PNumber;
typedef char	Date;
typedef char 	Mode;
typedef float	Dollar;

// typedef struct _date
// {
// 	int day, month, year;
// } Date;

// typedef struct _time
// {
// 	int hour, minute;
// } Time;

/**************************************************************
 * Encounter with large records, the structure can be improved.
 * 1. Long strings can be encoded to small bits.
 * 2. Any potential status information can be encoded to
 *    single bit.
 **************************************************************/
typedef struct _opal_serv
{
	int 		t_number;
	Date		date[20];
	Mode		mode;
	char		from[32], to[32];
	int 		j_numebr;	
	Dollar 		fare;
	Dollar		discount;
	Dollar		amount;
} OpalServ;

static void panic(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

void q1();
int q2();
int q3();
int q4a(int);
int q4b(int);
int q5();
int q6();
int q7(int, int, int);
int q8();

int main(int argc, char const *argv[])
{
	if (argc < 2)
		panic("Usage: make && ./problema [question number]");

	int qnumber = atoi(argv[1]);
	switch(qnumber)
	{
		case 1:
			q1();
			break;
		case 2:
			q2();
			break;
		case 3:
			q3();
			break;
		case 4:
		{
			printf("Enter a positive integer to be processed:\n");
			PNumber n = 0;
			if (scanf("%d", &n) != 0)
			{
				printf("Recursive version entered.\n");
				q4a(n);
				printf("For loop version entered.\n");
				q4b(n);
			}
			break;
		}
		case 5:
			q5();
			break;
		case 6:
			q6();
			break;
		case 7:
		{
			int max = 0;
			int a = 0, b = 0, c = 0;
			printf("Enter three numbers to be compared(space separated)\n");
			scanf("%d %d %d", &a, &b, &c);
			max = q7(a, b, c);
			printf("The largest number is: %d\n", max);
			break;
		}
		case 8:
			q8();
			break;
		default:
			panic("Given 8 problems in problem-set 1a.");
			break;
	}

	return 0;
}

void q1()
{
	int i = 0;
	int ret = 0;

	for (i = MIN_Q1_FORWARDS; i < MAX_Q1_FORWARDS; i++)
	{
		int e = i / 10000;
		int d = (i - (10000 * e)) / 1000;
		int a = i % 10;	
		int b = (i % 100 - a) / 10;
		int c = (i % 1000 - (10 * b + a)) / 100;

		
		int f = 10000*a + 1000*b + 100*c + 10*d + e;
		if (i * 4 == f)
		{
			printf("Found numebr: %d\n", i);
			ret++;
		}
		/*
		Another way of calculating the number in backwards
		is to convert it to a string and then inverse it
		and finally convert it into integer again
		*/	
	}

	printf("Found %d result(s) in total.\n", ret);
}

int q2()
{
	return 0;
}

int q3()
{
	// 6 * 5 * 4 * 3 * 2 * 1 = 720
	char letters[6] = {'c', 'a', 't', 'd', 'o', 'g'};
	int a, b, c, d, e;

	int result = 0;
	for (a = 0; a < sizeof(letters); ++a)
		for (b = 0; b < sizeof(letters); ++b)
		{
			if (b == a) continue;
			for (c = 0; c < sizeof(letters); ++c)
			{
				if (c == a || c == b) continue;
				for (d = 0; d < sizeof(letters); ++d)
				{
					if (d == a || d == b || d == c) continue;
					for (e = 0; e < sizeof(letters); ++e)
					{
						if (e == a || e == b || e ==c || e ==d) continue;
						result++;
					}
				}
			}
		}

	printf("%d\n", result);
	return 0;
}

// recursive version
int q4a(int n)
{
	if (n == 1)
		printf("1 is reached\n");
	else if (n % 2 == 0)
	{
		printf("%d\n", n / 2);
		q4a(n / 2);
	}
	else
	{
		printf("%d\n", 3 * n + 1);
		q4a(3 * n + 1);
	}

	return 0;
}

// for loop version
int q4b(int n)
{
	int number = n;

	for (; ;)
	{
		if (number == 1)
		{
			printf("1 is reached\n");
			break;
		}
		else if (number % 2 == 0)
			number = number / 2;
		else
			number = number * 3 + 1;
		printf("%d\n", number);
	}

	return 0;
}

int q5()
{
	OpalServ os;
	size_t sizeof_os = sizeof(os.t_number) + 
			sizeof(os.date) + sizeof(os.mode) + 
			sizeof(os.from) + sizeof(os.to) + 
			sizeof(os.j_numebr) + sizeof(os.fare) +
			sizeof(os.discount) + sizeof(os.amount);
	printf("The data strcuture needs memory of %lu byte(s)\n", sizeof_os);
	printf("Sizeof function gives that sturcture is %lu byte(s)\n", sizeof(os));
	// C gives memory a padding mechanism that compiler insert several unused bit
	// for the alignment of next integer caused by different memeory usage from
	// integer (4 bytes) and char (1 byte)
	// struct Test
	// {
	//    char AA;
	//    int BB;
	//    char CC;
	// }; GIVES memory cell as following
	// |   1   |   2   |   3   |   4   |  
	// | AA(1) | pad.................. |
	// | BB(1) | BB(2) | BB(3) | BB(4) | 
	// | CC(1) | pad.................. |
	// Solution:
	// 1. ask compiler to pack strcuture members with particular alignment
	// #pragma pack(1)
	// struct
	// #pragma pack()
	// 2. reordering members from largest to smallest
	return 0;
}

static int fib(int n)
{
	if (n <= 0)
		panic("Fibonacci sequence starts from 1!!");
	if (n == 1)
		return 1;
	else if (n == 2)
		return 1;
	else
		return fib(n - 1) + fib(n - 2);
}

int q6(int n)
{
	int i;

	for (i = MIN_Q6_ITEM; i < MAX_Q6_ITEM; ++i)
	{
		printf("Fib[%d] = %d\n", i, fib(i));
		q4a(fib(i));
	}

	return 0;
}

int q7(int a, int b, int c)
{
	int anb = a * (a >= b) + b * (b > a);
	
	return( anb * (anb >= c) + c * (c > anb));
	// using macro calculating maximum number among threes
	// return CUSTOM_MAX(CUSTOM_MAX(a, b), CUSTOM_MAX(a, c));
}

int q8()
{
	int arr[MAX_Q8_SIZE] = { 0 };
	int i, j;
	int n = 0;
	int counter = 0;
	int tmp;

	for (i = 0; scanf("%d", &n) != 0; ++i)
		arr[i] = n;
	counter = i;

	// simple buble sort implementation
	for (i = 0; i < counter; ++i)
	{
		for (j = i+1; j < counter; ++j)
		{
			if (arr[i] > arr[j])
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}

	for (i = 0; i < counter; ++i)
		printf("%d\n", arr[i]);

	return 0;
}
