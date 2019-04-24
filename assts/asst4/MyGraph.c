#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <math.h>

#define MAX_VERTICES    256
#define MAX_QUEUE_SIZE  256
#define MAX_ALIST_SIZE  255     // 256-1: no self-cycle involved

#define OP_FAILURE      0
#define OP_SUCCESS      1

#define TRUE            1
#define FALSE           0

#define PINNED          1
#define UNPINNED        0

#define x(VERTEXNODE)   (VERTEXNODE).v->x
#define y(VERTEXNODE)   (VERTEXNODE).v->y
// macros implement comparable vertex and edge
// v1 must be the structure rather than its pointer
#define veq(v1, v2)     ((v1->x==v2->x) && (v1->y==v2->y))
#define eeq(e1, e2)     (veq((e1).p1, (e2)->p1) && veq((e1).p2, (e2)->p2))
// e1 must be structure rather than its pointer

// A vertex is a 2D point
typedef struct Vertex { 
    int     x; // x-coordinate
    int     y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
    Vertex  *p1; // first end point
    Vertex  *p2; // second end point
} Edge;
// linked-list as adjacency list structure
// the reason adjacency matrix is not used here is to make space-efficient
typedef struct AdjNode {
    Vertex  *dest;
    int     weight;
    struct AdjNode 
                *next;
} AdjNode;

// as vertex is a 2D point
typedef struct VertexNode {
    Vertex  *v;
    AdjNode *head_a;
    int      nA;
} VertexNode;
typedef struct GraphRep *Graph;
typedef struct GraphRep {	// graph header
    VertexNode *vertices;	// an array of vertices or a linked list of vertices  
                            // edges will be implied by vertex's adjacency list
    int     nV; // #vertices
    int     nE; // #edges
} GraphRep;
// A vertex node stores a vertex and other information, and you need to expand this type

typedef char Bool;
typedef struct QueueNode
{
    VertexNode  vn;
    struct QueueNode 
                    *next;
} QueueNode;
typedef struct QueueRep
{
    QueueNode   *front;
    QueueNode   *back;
    int         size;
} QueueRep;
typedef QueueRep *Queue;

// The above types serve as a starting point only. You need to expand them and add more types. 
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment    

// malloc failure caused errors
static const char *error_msg[6] =
{
    "vnode creation failure",
    "graph creation failure",
    "edge creation failure",
    "qnode creation failure",
    "queue creation failure",
    "adjnode creation failure"
};

// private forward references - queue impl
static Queue newQueue();
static QueueNode * newQueueNode(VertexNode vn);
static Bool isEmpty(Queue q);
static void push(Queue q, VertexNode vn);
static QueueNode * pop();
static void dropQueue(Queue q);
static void showQueue(Queue q);

// detect memory allocation error for <assert.h>
static void
MallocError(void *ptr, const char *msg)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "%s\n", msg);
        exit(EXIT_FAILURE);
    }
}

static void
debug_info(Graph g, int i)
{
    printf("===Debugging %dth vertex===\n", i);
    if (i > g->nV-1)
        printf("exceeding\n");
    else
        printf("(%d, %d)\n", g->vertices[i].v->x, g->vertices[i].v->y);
    printf("=========================\n");
}

// Add the time complexity analysis of CreateEmptyGraph() here
Graph CreateEmptyGraph()
{
    // cast mallocs for the unity in main function
    Graph g = (Graph) malloc(sizeof(GraphRep));
    MallocError(g, error_msg[1]);
    g->vertices = (VertexNode *) malloc(sizeof(VertexNode) * MAX_VERTICES);
    MallocError(g->vertices, error_msg[0]);
    g->nV = 0;
    g->nE = 0;
    
    return g;
}

static int
isContain(Graph g, Vertex *vin)
{
    int retval = -1;
    int i;
    for (i = 0; i < g->nV; i++)
    {
        if (veq(g->vertices[i].v, vin))
            retval = i;
    }
    return retval;
}

static Bool
isConnect(Vertex *v, VertexNode vnode)
{
    AdjNode *it = vnode.head_a;
    do
    {
        if (veq(v, it->dest))
            return TRUE;
    } while ((it=it->next) != NULL);

    return FALSE;
}

static int
costFunction(Vertex *v1, Vertex *v2)
{
    return (sqrt(pow((v1->x-v2->x),2) + pow((v1->y-v2->y),2)));
}

static AdjNode *
newAdjNode(Vertex *dest, Vertex *src)
{
    AdjNode *adj = (AdjNode *) malloc(sizeof(AdjNode));
    MallocError(adj, error_msg[5]);
    adj->dest = dest;
    adj->weight = costFunction(dest, src);
    adj->next = NULL;
    return adj;
}

static void
InsertAdjNode(Vertex *dest, VertexNode *src)
{
    AdjNode *it  = src->head_a;
    AdjNode *new = newAdjNode(dest, src->v);

    if (src->nA == 0)
    {
        src->head_a = new;
        src->nA++;
    }
    else
    {
        while (it->next != NULL)
            it = it->next;
        it->next = new;
    }

    src->nA++;
}

VertexNode
newVertexNode(Vertex *vin)
{
    VertexNode vnode = { .v = vin, .head_a = NULL, .nA = 0 };
    return vnode;
}

// Add the time complexity analysis of InsertEdge() here
int InsertEdge(Graph g, Edge *e)
{
    assert(g != NULL);
    int tmp_idx1;
    Vertex *tmp_vtx1, *tmp_vtx2;
    int i, j;
    i = isContain(g, e->p1);
    j = isContain(g, e->p2);
    if (i == -1 && j == -1)         // p1 & p2 not in the graph
    {
        g->vertices[g->nV] = newVertexNode(e->p1);
        InsertAdjNode(e->p2, &g->vertices[g->nV]);
        g->nV++;
        g->vertices[g->nV] = newVertexNode(e->p2);
        InsertAdjNode(e->p1, &g->vertices[g->nV]);
        g->nV++;
    }
    else if (i == -1 || j == -1)    // p1 | p2 not in the graph
    {
        tmp_idx1 = i == -1 ? j : i;         // tmp_idx1 holds idx in the graph
        tmp_vtx1 = i == -1 ? e->p2 : e->p1;
        tmp_vtx2 = i == -1 ? e->p1 : e->p2; // tmp_vtx2 holds vtx not in the graph
        InsertAdjNode(tmp_vtx2, &g->vertices[tmp_idx1]);
        g->vertices[g->nV] = newVertexNode(tmp_vtx2);
        InsertAdjNode(tmp_vtx1, &g->vertices[g->nV++]);
    }
    else                            // p1 & p2 in the graph
    {
        if (isConnect(e->p1, g->vertices[j]))   // edge existed
            return OP_FAILURE;
        InsertAdjNode(e->p2, &g->vertices[i]);
        InsertAdjNode(e->p1, &g->vertices[j]);
    }
    g->nE++;
    return OP_SUCCESS;
}

// Add the time complexity analysis of DeleteEdge() here
void DeleteEdge(Graph g, Edge *e)
{
    assert(g != NULL);
}

// Add the time complexity analysis of ReachableVertices() here
void ReachableVertices(Graph g, Vertex *v)
{
    int idx;
    if ((idx=isContain(g, v)) != -1)
        return;
}

// Add the time complexity analysis of ShortestPath() here
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
    
}

// Add the time complexity analysis of FreeGraph() here
void FreeGraph(Graph g)
{
    int i;
    AdjNode *j, *tmp;
    for (i = 0; i < MAX_VERTICES; i++)
    {
        j=g->vertices[i].head_a;
        while (j != NULL)
        {
            tmp = j;
            j = j->next;
            free(tmp);
        }
    }
    free(g->vertices);
    free(g);
}

static void
BFS(Graph g)
{
    int i, idx;
    Bool visited[MAX_VERTICES] = { FALSE };
    AdjNode *it;
    VertexNode vnode;
    Queue q = newQueue();
    QueueNode *qnode = NULL;

    for (i = 0; i < g->nV; i++)
    {
        // start from first vertex by default
        visited[i] = TRUE;
        push(q, g->vertices[i]);
        while (!isEmpty(q))
        {
            qnode = pop(q);
            if (qnode == NULL) break;
            for (it = qnode->vn.head_a; it != NULL; it = it->next)
            {
                idx = isContain(g, it->dest);
                vnode = g->vertices[idx];
                if (!visited[idx])
                {
                    printf("(%d, %d) - ", x(qnode->vn), y(qnode->vn));
                    printf("(%d, %d)\n", x(vnode), y(vnode));
                    push(q, vnode);
                    visited[idx] = TRUE;
                }
            }
        }
    }
    dropQueue(q);
}

// Add the time complexity analysis of ShowGraph() here
void ShowGraph(Graph g)
{
    assert(g != NULL);
    if (g->nV == 0)
    {
        printf("Empty Graph\n");
        return;
    }
    BFS(g);
}

/************** Queue Implementation **************/
static Queue
newQueue()
{
    Queue q = (Queue) malloc(sizeof(QueueRep));
    MallocError(q, error_msg[4]);
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    return q;
}

static QueueNode *
newQueueNode(VertexNode vn)
{
    QueueNode *qnode = (QueueNode *) malloc(sizeof(QueueNode) * MAX_QUEUE_SIZE);
    MallocError(qnode, error_msg[3]);
    qnode->vn = vn;
    qnode->next = NULL;
    return qnode;
}

static Bool
isEmpty(Queue q)
{
    return (q->size == 0);
}

static void
push(Queue q, VertexNode vn)
{
    QueueNode *new = newQueueNode(vn);
    if (q->front == NULL)
        q->front = q->back = new;
    else
    {
        q->back->next = new;
        q->back = new;
    }
    q->size++;
}

static QueueNode *
pop(Queue q)
{
    if (q->front == NULL) return NULL;
    QueueNode *front = q->front;
    q->front = front->next;
    q->size--;
    return front;
}

static void
dropQueue(Queue q)
{
    QueueNode *tmp, *it = q->front;
    while (it != NULL)
    {
        tmp = it;
        it = it->next;
        free(tmp);
    }
    free(q);
}

static void
showQueue(Queue q)
{
    printf("Showing queue\n");
    QueueNode *it = q->front;
    for (it = q->front; it != NULL; it = it->next) {
        printf("(%d, %d)\n", it->vn.v->x, it->vn.v->y);
    }
    printf("size:[%d]\n", q->size);
    printf("------------------------\n");
}
/************** End of Queue **************/

int main() //sample main for testing 
{ 
    Graph g1;
    Edge *e_ptr; 
    Vertex *v1, *v2;

    // Create an empty graph g1;
    g1=CreateEmptyGraph();

    // Create first connected component 
    // Insert edge (0,0)-(0,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=0;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // Insert edge (0,0)-(5,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=5;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    free(e_ptr);
    free(v1);
    free(v2);

    // // Insert edge (0, 10)-(10, 10)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=0;
    // v1->y=10;
    // v2->x=10;
    // v2->y=10;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (0,10)-(5,6)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=0;
    // v1->y=10;
    // v2->x=5;
    // v2->y=6;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (0,0)-(5,4)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=0;
    // v1->y=0;
    // v2->x=5;
    // v2->y=4;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (5, 4)-(10, 4)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=5;
    // v1->y=4;
    // v2->x=10;
    // v2->y=4;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (5,6)-(10,6)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=5;
    // v1->y=6;
    // v2->x=10;
    // v2->y=6;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (10,10)-(10,6)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=10;
    // v1->y=10;
    // v2->x=10;
    // v2->y=6;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (10, 6)-(10, 4)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=10;
    // v1->y=6;
    // v2->x=10;
    // v2->y=4;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Create second connected component
    // // Insert edge (20,4)-(20,10)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=20;
    // v1->y=4;
    // v2->x=20;
    // v2->y=10;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (20,10)-(30,10)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=20;
    // v1->y=10;
    // v2->x=30;
    // v2->y=10;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

    // // Insert edge (25,5)-(30,10) 	
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=25;
    // v1->y=5;
    // v2->x=30;
    // v2->y=10;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2;
    // if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n"); 

    //Display graph g1
    ShowGraph(g1);

    // // Find the shortest path between (0,0) and (10,6) 
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=0;
    // v1->y=0;
    // v2->x=10;
    // v2->y=6;
    // ShortestPath(g1, v1, v2);
    // free(v1);
    // free(v2);
        
    // // Delete edge (0,0)-(5, 6)
    // e_ptr = (Edge*) malloc(sizeof(Edge));
    // assert(e_ptr != NULL);
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=0;
    // v1->y=0;
    // v2->x=5;
    // v2->y=6;
    // e_ptr->p1=v1;
    // e_ptr->p2=v2; 	 
    // DeleteEdge(g1, e_ptr);
    // free(e_ptr);
    // free(v1);
    // free(v2);
        
    // // Display graph g1
    // ShowGraph(g1);

    // // Find the shortest path between (0,0) and (10,6) 
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=0;
    // v1->y=0;
    // v2->x=10;
    // v2->y=6; 
    // ShortestPath(g1, v1, v2);
    // free(v1);
    // free(v2);

    // // Find the shortest path between (0,0) and (25,5)
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v2=(Vertex *) malloc(sizeof(Vertex));
    // assert(v2 != NULL);
    // v1->x=0;
    // v1->y=0;
    // v2->x=25;
    // v2->y=5;
    // ShortestPath(g1, v1, v2);
    // free(v1);
    // free(v2);	

    // // Find reachable vertices of (0,0)
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v1->x=0;
    // v1->y=0;
    // ReachableVertices(g1, v1);
    // free(v1);

    // // Find reachable vertices of (20,4)
    // v1=(Vertex*) malloc(sizeof(Vertex));
    // assert(v1 != NULL);
    // v1->x=20;
    // v1->y=4;
    // ReachableVertices(g1, v1);
    // free(v1);

    // Free graph g1
    FreeGraph(g1);

    return 0; 
}
