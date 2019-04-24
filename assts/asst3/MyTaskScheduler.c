#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// This template is only a guide
// You need to include additional fields, data structures and functions

#define MAX_TASKS		1024

#define OP_SUCCESS	1
#define OP_FAILURE	0

#define DEADLINE		68
#define RELEASE	 		82

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
typedef struct BinomialHeap {
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

// debug purpose
void PrintBinomialHeap(HeapNode *, bool);

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
        if (H2->child)
            H2->child->prev = H1;
        H1->parent = H2;
        H1->next   = H2->child;
        H2->child  = H1;
        HeapNode *prev = H1->prev;
        H2->prev = prev;
        H1->prev = NULL;
        H2->order += 1;
        return H2;
    }

    if (H1->child)
        H1->child->prev = H2;
    H2->parent = H1;
    H2->next   = H1->child;
    H1->child  = H2;
    HeapNode *next = H1->next;
    H1->next = next;
    H2->prev = NULL;
    H1->order += 1;

    return H1;
}

void MergeSelf(HeapNode **H)
{
    HeapNode *curr = (*H);
    while (!isEnd(curr->next))
    {
        if (ORDER(curr) == ORDER(curr->next))
        {
            HeapNode *prev = curr->prev;
            HeapNode *next = curr->next->next;
            curr = MergeHeap(curr, curr->next);
            if (prev)
            {
                curr->prev = prev;
                prev->next = curr;
            }
            else
                (*H) = curr;
            curr->next = next;
            if (next)
                next->prev = curr;
            continue;
        }
        curr = curr->next;
    }
}

// helper function - merge two Bheaps T1 and T2 into a new Heap
// result will be mapped to T1
// !!bottom-up mechanism would be a lot more helpful when recognise the
// !!input at first (i.e., talking about one-by-one insertion, normal
// !!down-heap is much more efficient).
// O(logn) - Merge() happens between a root and another root, following
// children relations will not be taken operations.
static BinomialHeap *
Merge(BinomialHeap *T1, BinomialHeap *T2)
{
    HeapNode *curr1 = T1->roots;	// start with the B_0 in both Ts
    HeapNode *curr2 = T2->roots;
    HeapNode *_heap = NULL;
    HeapNode **it   = &_heap;
    BinomialHeap *B = newHeap();
    HeapNode *prev  = NULL;	// previous relation builds up by callback

    // directly return - one node results in zero order
    if (isEnd(curr1) || isEnd(curr2))
        return( isEnd(curr1) ? T2 : T1 );

    while (1)
    {
        B->size += 1;
        if (!curr1 && !curr2) break;
        else if (!curr1)
        {
            *it = curr2;
            (*it)->prev = prev;
            if (curr2->next)
                curr2 = curr2->next;
            else
                break;
        }
        else if (!curr2)
        {
            *it = curr1;
            (*it)->prev = prev;
            if (curr1->next)
                curr1 = curr1->next;
            else
                break;
        }
        else if (ORDER(curr1) == ORDER(curr2))
        {
            HeapNode *curr1_next = curr1->next;
            HeapNode *curr2_next = curr2->next;
            *it = MergeHeap(curr1, curr2);
            (*it)->prev = prev;
            curr1 = curr1_next;
            curr2 = curr2_next;
        }
        else if (ORDER(curr1) < ORDER(curr2))
        {
            *it = curr1;
            curr1 = curr1->next;
            (*it)->prev = prev;
        }
        else
        {
            *it = curr2;
            curr2 = curr2->next;
            (*it)->prev = prev;
        }
        prev = (*it);
        it = &((*it)->next);
    }

    MergeSelf(&_heap);
    B->roots = _heap;
    return B;
}

// put the time complexity analysis for Insert() here
// O(logn) - As bottom-up construction somehow costs much more than
// downheap, normal merge algorithm is applied. Insertion() mostly
// calls Merge() function which takes log(n) operations.
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{
    // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this task already exists in T
    //put your code here
    /* printf("INSERT WITH PRIORITY KEY: %d\n", k); */
    HeapNode *newN = newHeapNode(k, n, c, r, d);
    BinomialHeap *newT = newHeap();
    newT->roots = newN;
    newT->size  = 1;
    *T = *Merge(T, newT);	// assign new BinomialHeap
}

// helper function - reverse the way how roots are linked together
static void
reverse(HeapNode **root)
{
    HeapNode *prev = NULL;
    HeapNode *curr = (*root);
    HeapNode *next = NULL;

    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
        if (prev)
            prev->prev = curr;
    }
    (*root) = prev;
}

// put your time complexity for RemoveMin() here
// O(log n) - As RemoveMin only cares about the roots node
// which is of size log n.
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
            // rearrange the remaining
            HeapNode *next = it->next;
            if (it->prev)
                it->prev->next = next;
            else
                T->roots = it->next;
            // generating new forest
            BinomialHeap *new_forest = newHeap();
            reverse(&ret->child);
            new_forest->roots = ret->child;
            /* printf("new_forest looks like\n"); */
            /* PrintBinomialHeap(new_forest->roots, FALSE); */
            /* printf("new T looks like\n"); */
            /* PrintBinomialHeap(T->roots, FALSE); */
            /* printf("EOD\n"); */
            if (next)
                next->prev = it->prev;
            *T = *Merge(T, new_forest);
            T->size -= 1;
            break;
        }
        it = it->next;
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

void
PrintBinomialHeap(HeapNode *H, bool isChild)
{
    if (!H) return;
    HeapNode *curr = H;
    while (!isEnd(curr))
    {
        if (!isChild)
            printf("===B_%d===\n", ORDER(curr));
        printf("%d[%d]\n", KEY(curr), curr->TaskName);
        /* if (curr->prev) */
        /*     printf("\t prev: %d\n", KEY(curr->prev)); */
        /* else */
        /*     printf("\n"); */
        PrintBinomialHeap(curr->child, TRUE);
        curr = curr->next;
    }
}

// put your time complexity analysis for MyTaskScheduler here
int TaskScheduler(char *f1, char *f2, int m)
{
    // put your code here
    FILE *in  = fopen(f1, "r");
    FILE *out = fopen(f2, "w");	// exception 2:
                                // "w" destory contents create new
    int i = 0;	// increment variable
    int size = 0;
    int v = 0;	// priority key
    int c = 0;	// execution time
    int r = 0;	// release time
    int d = 0;	// deadline

    BinomialHeap *r_heap = newHeap();	// release time as key
    BinomialHeap *d_heap = newHeap();	// deadline as key
    BinomialHeap *cores  = newHeap();	// core scheduling priority queue

    // exception 1:
    if (!in)
    {
        fprintf(stderr, "%s\n", "file1 does not exsit");
        exit(EXIT_FAILURE);
    }

    /* for (i = 1; i <= m; i++)	// initialise scheduling point */
    /*     Insert(cores, 0, 0, 0, 0, 0); */

    i = 0;
    while ((fscanf(in, "%d %d %d %d", &v, &c, &r, &d)) == 4)
    {
        Insert(r_heap, r, v, c, r, d);
        i++;
    }
    size = i;
    i = 0;
    int cpu_idx = 1;
    int curtime = 0;	// scheduling point
    while (1)
    {
        if (i == size) break;
        while (r_heap->roots && Min(r_heap) == curtime)	// task is ready
        {
            HeapNode *min = RemoveMin(r_heap);
            // store all tasks with the same release time
            // sort them by deadline as key in ascending order
            /* printf("extract %d\n", min->TaskName); */
            Insert(d_heap, min->Dline, min->TaskName,
                   min->Etime, min->Rtime, min->Dline);
            /* PrintBinomialHeap(d_heap->roots, FALSE); */
            if (!r_heap->roots) break;
            continue;
        }
        PrintBinomialHeap(d_heap->roots, FALSE);
        printf("==========time:%2d===========\n", curtime);
        while (cores->roots)	// empty the in-time free cpu
        {
            if (Min(cores) == curtime && curtime != 0)
            {
                RemoveMin(cores);
                cpu_idx--;
            }
            else break;
        }
        while (d_heap->roots)
        {
            if (cpu_idx > m)	// running out of cores
                break;
            HeapNode *task = RemoveMin(d_heap);
            int sparetime = task->Etime + curtime;
            if (sparetime > task->Dline) // infesaible condition
            {
                fprintf(out, "Infeasible\n");
                fclose(in);
                fclose(out);
                return OP_FAILURE;
            }
            Insert(cores, sparetime, cpu_idx, 0, 0, 0);
            fprintf(out, "%d Core%d %d ", task->TaskName, cpu_idx++, curtime);
            i++;
        }
        curtime++;
    }
    fprintf(out, "\n");

    fclose(in);
    fclose(out);
    return OP_SUCCESS;
}

int main( void ) //sample main for testing
{
    /* BinomialHeap *b = newHeap(); */
    /* for (int i= 10; i > 1; i--) */
    /* { */
    /*     printf("----------------------"); */
    /*     Insert(b, i, i, i, i, i); */
    /*     PrintBinomialHeap(b->roots, FALSE); */
    /* } */
    /* for (int i = 10; i > 1; i--) */
    /* { */
    /*     RemoveMin(b); */
    /*     PrintBinomialHeap(b->roots, FALSE); */
    /* } */

    int i;
    /* i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* else printf("Feasible schedule!\n"); */
    /* /\* There is a feasible schedule on 4 cores *\/ */
    i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
    if (i==0) printf("No feasible schedule!\n");
    else printf("Feasible schedule!\n");
    /* /\* There is no feasible schedule on 3 cores *\/ */
    /* i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* else printf("Feasible schedule!\n"); */
    /* /\* There is a feasible schedule on 5 cores *\/ */
    /* i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* else printf("Feasible schedule!\n"); */
    /* /\* There is no feasible schedule on 4 cores *\/ */
    /* i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* else printf("Feasible schedule!\n"); */
    /* /\* There is no feasible schedule on 2 cores *\/ */
    /* i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2); */
    /* if (i==0) printf("No feasible schedule!\n"); */
    /* else printf("Feasible schedule!\n"); */
    /* /\* There is a feasible schedule on 2 cores *\/ */
    return 0;
}
