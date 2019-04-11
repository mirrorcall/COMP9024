#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// This template is only a guide
// You need to include additional fields, data structures and functions

#define OP_SUCCESS	1
#define OP_FAILURE	0

#define TRUE	1
#define FALSE	0

#define ONE(BK)		(BK->order == 0 ? 1 : 0)
#define KEY(BK)		(BK->key)
#define ORDER(BK)	(BK->order)

typedef char bool;

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    // release time and deadline of one task
    int    key;       	// key of this task
    int    TaskName;  	// task name
    int    Etime; 			// execution time of this task
    int    Rtime; 			// release time of this task
    int    Dline; 			// deadline of this task
    // add additional fields here
    struct HeapNode
           *child;      // each node has multiple children
    struct HeapNode
           *parent;     // parent node of each child node 
    struct HeapNode
           *prev;		    // linked-list-like list of nodes
    struct HeapNode
           *next;		    // linked-list-like list of nodes
                        // store the left most
    int    order;       // heap order
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap{
	// this is heap header
    int	      size;      // count of items in the heap
    // add additional fields here
    HeapNode  *roots;	 // a set of roots
} BinomialHeap;

// Forward References
HeapNode * newHeapNode(int, int, int , int, int);
BinomialHeap * newHeap();
void Insert(BinomialHeap *, int, int, int, int, int);
HeapNode * RemoveMin(BinomialHeap *);
int Min(BinomialHeap *);
int TaskScheduler(char *, char *, int);

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{
		// k:key, n:task name, c: execution time, r:release time, d:deadline
  	// ... you need to define other parameters yourself)
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    // initialise other fields
    new->child = NULL;
    new->prev  = NULL;
    new->next  = NULL;
    new->order = 0;
    return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
    BinomialHeap *T;
    T = malloc(sizeof(BinomialHeap));
    assert (T != NULL);
    // initialise all the fields here
    T->roots = NULL;
    return T;
}

static HeapNode *
CloneHeapNode(HeapNode *node)
{
    assert(node != NULL);
    HeapNode *cpy = newHeapNode(node->key, node->TaskName,
                                node->Etime, node->Rtime, node->Dline);
    return cpy;
}

static bool
isEnd(HeapNode *root)
{
    if (!root)
        return TRUE;
    return FALSE;
}

// helper function - merge two heaps H1 and H2 into a new Heap
// result will be mapped to H1
static HeapNode *
MergeHeap(HeapNode *H1, HeapNode *H2)
{
    if (KEY(H1) > KEY(H2))
    {
        int tmp = KEY(H2);
        H2->key = KEY(H1);
        H1->key = tmp;
    }

    H2->next   = H1->child;
    /* H1->child->prev = H2; */
    H1->child  = H2;
    H2->parent = H1;
    H1->order += 1;

    return H1;
}

// helper function - merge two Bheaps T1 and T2 into a new Heap
// result will be mapped to T1
// !!bottom-up mechanism would be a lot more helpful when recognise the
// !!input at first (i.e., talking about one-by-one insertion, normal
// !!down-heap is much more efficient).
static BinomialHeap *
Merge(BinomialHeap *T1, BinomialHeap *T2)
{
    HeapNode *curr1 = T1->roots;	// start with the B_0 in both Ts
    HeapNode *curr2 = T2->roots;
    
    // directly return - one node results in zero order
    if (isEnd(curr1) || isEnd(curr2))
        return( isEnd(curr1) ? T2 : T1 );

    while (!isEnd(curr1) && !isEnd(curr2))
    {
        printf("B_%d and B_%d are now merging!\n", ORDER(curr1), ORDER(curr2));
        // there is only one B_k to be added to new BinomialHeap
        if (ORDER(curr1) == ORDER(curr2))
        {
            curr1 = MergeHeap(curr1, curr2);
            curr2 = curr2->next;
        }
        else if (ORDER(curr1) < ORDER(curr2))
            curr1 = curr1->next;
        else
        {   // ORDER(curr1) > ORDER(curr2) - insert curr2 in the front
            HeapNode *curr2_cpy = CloneHeapNode(curr2);
            if (!curr1->prev)
            {
                curr2_cpy->next = curr1;
                curr1->prev = curr2_cpy;
                T1->roots = curr2_cpy;
            }
            else
            {
                HeapNode *prev = curr1->prev;
                prev->next = curr2_cpy;
                curr2_cpy->prev = prev;
                curr2_cpy->next = curr1;
                curr1->prev = curr2_cpy;
            }
            curr2 = curr2->next;
        }
    }

    // all left-overs in the Heap 1 will remain
    while (!isEnd(curr2))
    {
        HeapNode *curr2_cpy = CloneHeapNode(curr2);
        curr1->next = curr2_cpy;
        curr2_cpy->prev = curr1;
        curr2 = curr2->next;
    }

    return T1;
}

// put the time complexity analysis for Insert() here
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{
    // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this task already exists in T
    //put your code here
    HeapNode *newN = newHeapNode(k, n, c, r, d);
    BinomialHeap *newT = newHeap();
    newT->roots = newN;
    newT->size  = 1;
    *T = *Merge(T, newT);	// assign new BinomialHeap
}

// put your time complexity for RemoveMin() here
HeapNode *RemoveMin(BinomialHeap *T)
{
    // put your code here
    HeapNode *ret = NULL;
    HeapNode *it = T->roots;
    int min = Min(T);
    while (!isEnd(it))
    {
        if (KEY(it) == min)
        {
            // store the entry
            ret = it;
            // generating new forest
            BinomialHeap *new_forest = newHeap();
            new_forest->roots = ret->child;
            // rearrange the remaining
            HeapNode *next = it->next;
            it->prev->next = next;
            next->prev = it->prev;
            *T = *Merge(T, new_forest);
            break;
        }
    }
    return ret;
}

int Min(BinomialHeap *T)
{
    // put your code here
    HeapNode *it = T->roots;
    assert(it != NULL);	// empty BinomialHeap does not have a min here

    int min = KEY(it);
    while (!isEnd(it))
    {
        if (KEY(it) < min)
            min = KEY(it);
        it = it->next;
    }
    return min;
}

static void
PrintBinomialHeap(HeapNode *H)
{
    if (!H) return;
    int i = 0;
    HeapNode *curr = H;
    while (!isEnd(curr))
    {
        printf("%d\n", KEY(curr));
        PrintBinomialHeap(curr->child);
        if (!curr->child) printf("#######B_%d End#######\n", i);
        i++;
        curr = curr->next;
    }
}

// put your time complexity analysis for MyTaskScheduler here
int TaskScheduler(char *f1, char *f2, int m)
{
    // put your code here
    FILE *in  = fopen(f1, "r");	// exception 2:
    FILE *out = fopen(f2, "w");	// "w" destory contents create new

    int v = 0;	// priority key
    int c = 0;	// execution time
    int r = 0;	// release time
    int d = 0;	// deadline

    // exception 1:
    if (!in)
    {
        fprintf(stderr, "%s\n", "file1 does not exsit");
        exit(EXIT_FAILURE);
    }

    while ((fscanf(in, "%d %d %d %d", &v, &c, &r, &d)) == 4)
    {
        printf("%d, %d, %d, %d\n", v, c, r, d);
    }

    fclose(in);
    fclose(out);
    return OP_SUCCESS;
}

int main() //sample main for testing
{
    BinomialHeap *b = newHeap();
    printf("-----------------------------------------\n");
    Insert(b, 1, 1, 1, 1, 1);
    PrintBinomialHeap(b->roots);
    printf("-----------------------------------------\n");
    Insert(b, 2, 2, 2, 2, 2);
    PrintBinomialHeap(b->roots);
    Insert(b, 3, 3, 3, 3, 3);
    printf("-----------------------------------------\n");
    PrintBinomialHeap(b->roots);
    Insert(b, 4, 4, 4, 4, 4);
    printf("-----------------------------------------\n");
    PrintBinomialHeap(b->roots);
    /* int i; */
    /* i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* /\* There is a feasible schedule on 4 cores *\/ */
    /* i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* /\* There is no feasible schedule on 3 cores *\/ */
    /* i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* /\* There is a feasible schedule on 5 cores *\/ */
    /* i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* /\* There is no feasible schedule on 4 cores *\/ */
    /* i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* /\* There is no feasible schedule on 2 cores *\/ */
    /* i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* /\* There is a feasible schedule on 2 cores *\/ */
    return 0;
}
