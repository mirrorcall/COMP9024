#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 
#define TRUE	1
#define FALSE	0


// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;     	   // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;

	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
// Time Complexity: O(n) - traverse file for n elements
DLList *CreateDLListFromFileDlist(const char *filename)
{
	// put your code here
	FILE *fp;
	DLList *L = newDLList();

	if (strcmp(filename, "stdin") == 0) {
		// assume the number would not be longer than 100 bits
		char *val = malloc(sizeof(char) * 100);
		fp = stdin;
		while (1) {
			if ((fscanf(fp, "%[^\n]s", val)) == 1) {
				DLListNode *new = newDLListNode(atoi(val));

				if (L->size == 0) {				// add new node to a empty list
					L->first = new;
					L->last  = new;
					L->size++;
				}
				else {							// add new node to a growing list
					DLListNode *curr = L->last;
					curr->next = new;
					new->prev = curr;
					L->last = new;
					L->size++;
				}
			}
			else {
				break;
			}
			getchar();
		}
		free(val);
	}
	else {
		int val;
		fp = fopen(filename, "rb");
		while ((fscanf(fp, "%d", &val)) == 1) {
			DLListNode *new = newDLListNode(val);

			if (L->size == 0) {				// add new node to a empty list
				L->first = new;
				L->last  = new;
				L->size++;
			}
			else {							// add new node to a growing list
				DLListNode *curr = L->last;
				curr->next = new;
				new->prev = curr;
				L->last = new;
				L->size++;
			}
		}
	}

	fclose(fp);							// since read file only once - close here
	return L;
}

// clone a DLList
// put your time complexity analysis for cloneList() here
// Time Complexity: O(n) - traverse copied list for n elements
DLList *cloneList(DLList *u)
{
	// put your code here
 	DLList *v = newDLList();
 	DLListNode *curr = u->first;

 	while (curr != NULL) {
 		DLListNode *new = newDLListNode(curr->value);

 		if (v->size == 0) {		
 			v->first = new;
 			v->last  = new;
 			v->size++;
 		}
 		else {
 			DLListNode *v_curr = v->last;
 			v_curr->next = new;
 			new->prev = v_curr;
 			v->last = new;
 			v->size++;
 		}

 		curr = curr->next;
 	}
	
	return v;
}

// check if set contains value
int isContain(int value, DLList *u)
{
	DLListNode *curr = u->first;

	while (curr != NULL) {
		if (curr->value == value)
			return TRUE;
		curr = curr->next;
	}

	return FALSE;
}

// compute the union of two DLLists u and v
// Time Complexity: O(mn) - traverse both m and n lists
DLList *setUnion(DLList *u, DLList *v)
{
	// put your code here
	// TODO: WILL ALL THE ELEMENTS IN THE SET BE UNIQUE?
	DLList *result = cloneList(u);
	DLListNode *v_curr = v->first;

	while (v_curr != NULL) {
		if (isContain(v_curr->value, u)) {
			v_curr = v_curr->next;
			continue;
		}
		else {
			DLListNode *new = newDLListNode(v_curr->value);
			DLListNode *last = result->last;

			last->next = new;
			new->prev = last;
			result->last = new;
			result->size++;
			v_curr = v_curr->next;
		}
	}

	return result;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
// Time Complexity: O(mn) - traverse both m and n lists
DLList *setIntersection(DLList *u, DLList *v)
{
	// put your code here
	DLList *result = newDLList();
	DLListNode *v_curr = v->first;

	while (v_curr != NULL) {
		if (!isContain(v_curr->value, u)) {
			v_curr = v_curr->next;
			continue;
		}
		else {
			DLListNode *new = newDLListNode(v_curr->value);

			if (result->size == 0) {
				result->first = new;
				result->last  = new;
				result->size++;
			}
			else {
				DLListNode *curr = result->last;
				curr->next = new;
				new->prev  = curr;
				result->last = new;
				result->size++;
			}
			v_curr = v_curr->next;
		}
	}

	return result;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here
// Time Complexity: O(n+1) - free all n elements in list and list itself
void freeDLList(DLList *L)
{
	// put your code here
	DLListNode *head = L->first;
	DLListNode *tmp  = NULL;

	// TODO: to be tested on valgrind - valgrind tested no leak
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}

	free(L);
}


// display items of a DLList
// put your time complexity analysis for printDDList() here
// Time Complexity: O(n) - traverse list for n elements
void printDLList(DLList *u)
{
	// put your code here

#ifdef PRINT
	DLListNode *first = u->first;
	DLListNode *last  = u->last; 
	// going to print list in both ways
	printf("Forward printing.\n");
	while (first != NULL) {
		printf("%d->", first->value);
		first = first->next;
	}
	printf("NULL\n");

	printf("Backward printing.\n");
	while (last != NULL) {
		printf("%d->", last->value);
		last = last->prev;
	}
	printf("NULL\n");
	printf("Off size: %d\n", u->size);
#endif

	DLListNode *curr = u->first;
	while (curr != NULL) {
		printf("%d\n", curr->value);
		curr = curr->next;
	}
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
	if (strcmp(argv[1], "union") == 0) {
		DLList *u, *v, *ret;
		u = CreateDLListFromFileDlist(argv[2]);
		v = CreateDLListFromFileDlist(argv[3]);
		ret = setUnion(u, v);
		printDLList(ret);

		freeDLList(ret);
		freeDLList(v);
		freeDLList(u);
	}
	else if (strcmp(argv[1], "inter") == 0) {
		DLList 	*u, *v, *ret;
		u = CreateDLListFromFileDlist(argv[2]);
		v = CreateDLListFromFileDlist(argv[3]);
		ret = setIntersection(u, v);
		printDLList(ret);

		freeDLList(ret);
		freeDLList(v);
		freeDLList(u);
	}
	else {
		DLList *list;
		list = CreateDLListFromFileDlist(argv[1]);
		printDLList(list);
		freeDLList(list);
	}
#endif
	DLList *list1, *list2, *list3, *list4;

	list1=CreateDLListFromFileDlist("File1.txt");
	printDLList(list1);

	list2=CreateDLListFromFileDlist("File2.txt");
	printDLList(list2);

	list3=setUnion(list1, list2);
	printDLList(list3);

	list4=setIntersection(list1, list2);
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	printf("please type all the integers of list1\n");
	list1=CreateDLListFromFileDlist("stdin");


	printf("please type all the integers of list2\n");
	list2=CreateDLListFromFileDlist("stdin");

	list3=cloneList(list1);
	printDLList(list3);
	list4=cloneList(list2);
	printDLList(list4);

	printDLList(list1);
	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	return 0; 
}
