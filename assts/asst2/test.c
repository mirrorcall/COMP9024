#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE	1024 // 1023 char + 1 \n

#define OP_SUCCESS  1
#define OP_FAILURE  0

void validation(char *key, char *val) {
	if (!key || !val) {
		fprintf(stderr, "Input with wrong format, expect \"(D, D)\"");
		exit(EXIT_FAILURE);
	}
	else if (key[0] == 0 || val[0] == 0) {
		fprintf(stderr, "Input with wrong format, expect \"(D, D)\"");
		exit(EXIT_FAILURE);
	}
}

void parser(const char *buf) {
	int encounter = 0;
	int i;
	char *key = malloc(sizeof(char) * BUFFER_SIZE);
	char *val = malloc(sizeof(char) * BUFFER_SIZE);
	while (*buf != '\0') {
		if (*buf == '(') {
			memset(key, 0, BUFFER_SIZE);
			memset(val, 0, BUFFER_SIZE);
			encounter = 1;
		}
		else if ((isdigit(*buf) && encounter == 1) || *buf == '-') {
			key[i++] = *buf;
		}
		else if (*buf == ',') {
			encounter = 2;
			i = 0;
		}
		else if ((isdigit(*buf) && encounter == 2) || *buf == '-') {
			val[i++] = *buf;
		}
		else if (*buf == ')') {
			validation(key, val);
			printf("(%d,%d)\n", (int)strtol(key, NULL, 10), (int)strtol(val, NULL, 10));
			encounter = 0;
			i = 0;
		}
		buf++;
	}
	validation(key, val);
	free(key);
	free(val);
}

int main(int argc, char *argv[]) {
	
	char *buf = malloc(sizeof(char) * BUFFER_SIZE);
	
//	FILE *fp = fopen("File1.txt", "r");
	FILE *fp = stdin;
//	while (fgets(buf, BUFFER_SIZE, fp) != NULL)
//	{
//		if (buf[0] == '\n') break;
//		parser(buf);
//	}
//	
	char *b = malloc(100);
	printf("%lu\n", sizeof(b));

	free(buf);
	fclose(fp);
	return 0;
}