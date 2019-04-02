#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

#define OP_SUCCESS 		1
#define OP_FAILURE		0
#define INTER_MODE    73    // ASCII 'I'
#define UNION_MODE    85    // ASCII 'U'
#define BUFFER_SIZE   1024  // 1 kilo-byte

#define MAX(a, b)	((a > b) ? a : b)
#define MIN(a, b) ((a > b) ? b : a)

// macros comparing key and/or value (a) and an AVLTreeNode (n)
#define EQ(k, v, n) (((k == n->key) &&          \
                      (v == n->value)) ? 1 : 0)
#define EQK(k, n)	(( k == n->key) ? 1 : 0)
#define LTK(k, n)	(( k <  n->key) ? 1 : 0)
#define GTK(k, n)	(( k >  n->key) ? 1 : 0)
#define EQV(v, n) (( v == n->value) ? 1 : 0)
#define LTV(v, n)	(( v <  n->value) ? 1 : 0)
#define GTV(v, n)	(( v >  n->value) ? 1 : 0)
                  // unguard macros - require exactly n of type AVLTreeNode

// data type for avl tree nodes
typedef struct AVLTreeNode
{
    int 	key; 		// key of this item
    int  	value;  	// value (int) of this item
    int 	height; 	// height of the subtree rooted at this node
    struct AVLTreeNode
          *parent; 	// pointer to parent
    struct AVLTreeNode
          *left; 		// pointer to left child
    struct AVLTreeNode
          *right;		// pointer to right child
} AVLTreeNode;

// data type for AVL trees
typedef struct AVLTree
{
    int 		size;	// count of items in avl tree
    AVLTreeNode
            *root; 	// root
} AVLTree;

// data type for ASCII Tree Printer
typedef struct ATPnode
{
    int 		vaule;
    struct ATPnode
            *left;
    struct ATPnode
            *right;
} ATPnode;

typedef int * TreeList;

/************** Forward References **************/
AVLTreeNode *newAVLTreeNode(int k, int v );
AVLTree *newAVLTree();
AVLTree *CreateAVLTree(const char *filename);
AVLTree *CloneAVLTree(AVLTree *T);;
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2);
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2);
int InsertNode(AVLTree *T, int k, int v);
int DeleteNode(AVLTree *T, int k, int v);
AVLTreeNode *Search(AVLTree *T, int k, int v);
void FreeAVLTree(AVLTree *T);
void PrintAVLTree(AVLTree *T);

/************** Function Defnition **************/
// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}

static void validation(char *key, char *val) {
    if (!key || !val) {
        fprintf(stderr, "Input with wrong format, expect \"(D, D)\"");
        exit(EXIT_FAILURE);
    }
    else if (key[0] == 0 || val[0] == 0) {
        fprintf(stderr, "Input with wrong format, expect \"(D, D)\"");
        exit(EXIT_FAILURE);
    }
}

static void parser(AVLTree **T, const char *buf) {
    int i = 0, encounter = 0;
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
            int k = (int)strtol(key, NULL, 10);
            int v = (int)strtol(val, NULL, 10);
            InsertNode(*T, k, v);
            encounter = 0;
            i = 0;
        }
        buf++;
    }
    validation(key, val);
    free(key);
    free(val);
}

// put your time complexity analysis of CreateAVLTree() here
AVLTree *CreateAVLTree(const char *filename)
{
    // put your code here
    AVLTree *T = newAVLTree();
    FILE *fp;
    char *buf = malloc(sizeof(char) * BUFFER_SIZE);

    if (strcmp(filename, "stdin") == 0)
        fp = stdin;
    else
        fp = fopen(filename, "r");

    // put extra whitespace to cosume the adjacent whitespace
    while (fgets(buf, BUFFER_SIZE, fp) != NULL)
    {
        if (buf[0] == '\n') break;
        parser(&T, buf);
    }
    free(buf);
    fclose(fp);

  	return T;
}

// helper function - recursively clone all nodes of a AVLTree into another
static void CloneAVLTreeNodes(AVLTreeNode **dest, AVLTreeNode *src)
{
    if (src == NULL) return;

    if (src->left != NULL)
    {
        (*dest)->left = newAVLTreeNode(src->left->key, src->left->value);
        CloneAVLTreeNodes(&(*dest)->left, src->left);
    }
    if (src->right != NULL)
    {
        (*dest)->right = newAVLTreeNode(src->right->key, src->right->value);
        CloneAVLTreeNodes(&(*dest)->right, src->right);
    }
}

// put your time complexity analysis for CloneAVLTree() here
/* Time Complexity: O(n) - calling <code>void CloneAVLTreeNodes()</code>
 * only once, which travese n nodes in the tree one time.
 */
AVLTree *CloneAVLTree(AVLTree *T)
{
    // put your code here
    assert(T != NULL);
  	AVLTree *dest = newAVLTree();
  	dest->size = T->size;
  	dest->root = newAVLTreeNode(T->root->key, T->root->value);
  	CloneAVLTreeNodes(&dest->root, T->root);

  	return dest;
}

// !!idx always starts with 0.
// !!t stores both of the key and the value of the node
// For future reference, InorderTraverse() is of time complexity
// O(n) - n is the numeber of element in tree.
// InorderTraverse() traverses tree node in inorder once.
static void InorderTraverse(TreeList t, AVLTreeNode *root, int *idx)
{
    if (!root) return;
    InorderTraverse(t, root->left,  idx);
    t[(*idx)++] = root->key;
    t[(*idx)++] = root->value;
    InorderTraverse(t, root->right, idx);
}

// helper function - merge two tree lists containing (key, value) pair
// For future reference, MergeTreeLists() is of time complexity
// O(m+n) - m and n are the number of element in different trees.
// MergeTreeLists() traverses both of two TreeLists only once.
static int MergeTreeLists(TreeList dest, char mode,
                          TreeList t1, int n1, TreeList t2, int n2)
{
    int i = 0, j = 0, k = 0;
    if (mode == 'U')
    {
        while (i < (n1*2) && j < (n2*2))
        {
            if (t1[i] == t2[j])
            {
                if (t1[i+1] == t2[j+1])   // minor key comparison
                {
                    dest[k++] = t1[i++];
                    dest[k++] = t1[i++];
                    j += 2;
                }
                else if (t1[i+1] < t2[j+1])
                {
                    dest[k++] = t1[i++];
                    dest[k++] = t1[i++];
                }
                else
                {
                    dest[k++] = t2[j++];
                    dest[k++] = t2[j++];
                }
            }
            else if (t1[i] < t2[j])
            {
                dest[k++] = t1[i++];
                dest[k++] = t1[i++];
            }
            else
            {
                dest[k++] = t2[j++];
                dest[k++] = t2[j++];
            }
        }
        // left-over's
        while (i < n1)
        {
            dest[k++] = t1[i++];
            dest[k++] = t1[i++];
        }
        while (j < n2)
        {
            dest[k++] = t2[j++];
            dest[k++] = t2[j++];
        }
    }
    else if (mode == 'I')
    {
        while (i < (n1*2) && j < (n2*2))
        {
            if (t1[i] == t2[j])   // AVLTree supports minor key - value
            {                     // needs another comparison
                if (t1[i+1] == t2[j+1])
                {
                    dest[k++] = t1[i++];
                    dest[k++] = t1[i++];
                    j += 2;
                }
                else if (t1[i+1] < t2[j+1])
                {
                    i += 2;
                }
                else
                {
                    j += 2;
                }
            }
            else if (t1[i] < t2[j])
                i += 2;
            else
                j += 2;
        }
    }
    else printf("panic: wrong mode (neither (U)nion nor (I)ntersection)\n");

    return k;
}

// For future reference, CreateList() is of time complexity O(m+n),
// as CreateList() only calls InorderTraverse() for T1 of size m and T2 of
// size of n in linear time (i.e., O(n)), as well as MergeTreeLists() for
// the sum of length (m+n) in linear time (i.e., O(m+n)).
static int CreateList(TreeList dest, char mode,
                           AVLTree *T1, AVLTree *T2)
{
    int retval = 0;
    TreeList t1 = malloc(sizeof(TreeList) * T1->size * 2);
    TreeList t2 = malloc(sizeof(TreeList) * T2->size * 2);
    int zero = 0;
    InorderTraverse(t1, T1->root, &zero);
    zero = 0;
    InorderTraverse(t2, T2->root, &zero);
    if (mode == 'U')
        retval = MergeTreeLists(dest, mode, t1, T1->size, t2, T2->size);
    else if (mode == 'I')
        retval = MergeTreeLists(dest, mode, t1, T1->size, t2, T2->size);

    free(t1);
    free(t2);
    return retval;
}

// put your time complexity for ALVTreesUNion() here
/* Time Complexity: O(m+n) - m and n are the size of two AVLTrees.
 * AVLTreeUnion() calls CreateList of complexity O(m+n) as well as
 * InsertNode of complexity O(log(m+n)). And since linear dominates
 * logarithm, the final complexity is O(m+n).
 */
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
    // put your code here
    AVLTree *result = newAVLTree();
    TreeList u = malloc(sizeof(TreeList) * (T1->size + T2->size) * 2);
    int j, n = 0;
    n = CreateList(u, 'U', T1, T2);
    for (j = 0; j < n; j+=2)
        InsertNode(result, u[j], u[j+1]);
    free(u);
    return result;
}

// put your time complexity for ALVTreesIntersection() here
/* Time Complexity: O(m+n) - m and n are the size of two AVLTrees.
 * Similar to the AVLTreesUnion(), as calling the same callee.
 */
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
    // put your code here
    AVLTree *result = newAVLTree();
    TreeList i = malloc(sizeof(TreeList) * MIN(T1->size, T2->size) * 2);
    int j, n = 0;
    n = CreateList(i, 'I', T1, T2);
    for (j = 0; j < n; j+=2)
        InsertNode(result, i[j], i[j+1]);
    free(i);
    return result;
}

// helper function - calculating height of the current node
// !! Addtioanlly, the real height would be
// <strong>Height() - 1</strong>
// !! as Height() fucntion starts counting from 1. Initial
// !! height of "0" behaviour comes from calling new node.
// For future reference, the complexity of this function is
// O(n) for the worst case (i.e. full of leaves).
int Height(AVLTreeNode *node)
{
    if (!node) return 0;
    int lheight = Height(node->left);
    int rheight = Height(node->right);

    return( MAX(lheight, rheight) + 1 );
}

// !!Returning height difference between the left child of the node and the
// !!right child of it.
// !!return positive integer if left child has deeper depth
// !!return negative integer if right child has deeper depth
int HeightDiffer(AVLTreeNode *node)
{
    if (!node) return 0;
    return( Height(node->left) - Height(node->right) );
}

static AVLTreeNode *RotateRight(AVLTreeNode *z)
{
#ifdef DEBUG
    printf("Perfroming Rotation Right with k,v : %d,%d\n",
           z->key, z->value);
#endif
    AVLTreeNode *y = z->left;
    AVLTreeNode *s = y->right;    // left child of y must be of height of 0

    // redirecting
    if (z->parent)
        y->parent = z->parent;
    y->right  = z;
    z->left   = s;
    z->parent = y;
    if (s)
        s->parent = z;

    // TODO: check updated heights
    y->height = Height(y) - 1;
    z->height = Height(z) - 1;

    return y;     // now y becomes the new root;
}

static AVLTreeNode *RotateLeft(AVLTreeNode *z)
{
#ifdef DEBUG
    printf("Performing Rotation Left with k,v : %d,%d\n", z->key, z->value);
#endif
    AVLTreeNode *y = z->right;
    AVLTreeNode *s = y->left;        // left child of y must be of height of 0

    // redirecting
    if (z->parent)
        y->parent = z->parent;
    y->left   = z;
    z->right  = s;
    z->parent = y;
    if (s)
        s->parent = z;

    // TODO: check updated heights
    y->height = Height(y) - 1;
    z->height = Height(z) - 1;

    return y;     // now y becomes the new root;
}

// Insulate the function for the convenience of InsertNode as well as DeleteNode
static void Rebalance(AVLTreeNode **node, int k, int v)
{
    if (HeightDiffer(*node) > 1)
    {
        if (LTK(k, (*node)->left))
            (*node) = RotateRight((*node));
        else if (GTK(k, (*node)->left))
        {
            (*node)->left = RotateLeft((*node)->left);
            (*node) = RotateRight((*node));
        }
        else if (EQK(k, (*node)->left))    // dealing with the minor key - value
        {
            if (LTV(v, (*node)->left))
            {
                (*node) = RotateRight((*node));
            }
            else if (GTV(v, (*node)->left))
            {
                (*node)->left = RotateLeft((*node)->left);
                (*node) = RotateRight((*node));
            }
        }
    }
    else if (HeightDiffer(*node) < -1)
    {
        if (GTK(k, (*node)->right))
            (*node) = RotateLeft((*node));
        else if (LTK(k, (*node)->right))
        {
            (*node)->right = RotateRight((*node)->right);
            (*node) = RotateLeft((*node));
        }
        else if (EQK(k, (*node)->right))
        {
            if (GTV(v, (*node)->right))
                (*node) = RotateLeft((*node));
            else if (LTV(v, (*node)->right))
            {
                (*node)->right = RotateRight((*node)->right);
                (*node) = RotateLeft((*node));
            }
        }
    }
}

// helper fucntion - insert each node recursively
static AVLTreeNode *RecurInsert(AVLTreeNode **node, int k, int v)
{
    if (!*node)
    {
        *node = newAVLTreeNode(k, v);
        return( *node );
    }
    else
    {
        if (EQK(k, (*node)))
        {	// using value as minor key
            if (LTV(v, (*node)))
            {
                AVLTreeNode *child = RecurInsert(&(*node)->left, k, v);
                (*node)->left = child;
                child->parent = (*node);
                (*node)->height = Height(*node) - 1;
            }
            else if (GTV(v, (*node)))
            {
                AVLTreeNode *child = RecurInsert(&(*node)->right, k, v);
                (*node)->right = child;
                child->parent = (*node);
                (*node)->height = Height(*node) - 1;
            }
            else exit(EXIT_FAILURE); // THIS SHOULD NOT HAPPEN - EQV
        }
        else if (LTK(k, (*node)))
        {
            AVLTreeNode *child = RecurInsert(&(*node)->left, k, v);
            (*node)->left = child;
            child->parent = (*node);
            (*node)->height = Height(*node) - 1;
        }
        else if (GTK(k, (*node)))	// splited for logical resaon
        {
            AVLTreeNode *child = RecurInsert(&(*node)->right, k, v);
            (*node)->right = child;
            child->parent = (*node);
            (*node)->height = Height(*node) - 1;
        }
        else exit(EXIT_FAILURE);	// THIS SHOULD NOT HAPPEN
    }

    /*
     * rebalancing the tree - taking minor key (value) into consideration
     * instead of using key indicate its location,
     * rotation using comparison to find the specific child
     */
    Rebalance(&(*node), k, v);

    return( *node );
}

// put the time complexity analysis for InsertNode() here
/* Time Complexity: O(log n) - To insert a node in an AVL Tree, the node
 * will only have to be compared with one node in each level. Say there are
 * n nodes in the tree, so there are maximal (log n) levels in total based
 * on that AVL tree is a balanced tree. Then, it is obvious that the
 * insertion takes time of (log n) plus (log n) of performing search at the
 * beginning,which is clearly O(log n) (i.e., (2 * log n) dominated by
 * (log n) by Master Theorem).
 */
int InsertNode(AVLTree *T, int k, int v)
{
    // put your code here
#ifdef DEBUG
    printf("Now inserting pair (%d,%d)\n", k, v);
#endif
    if (Search(T, k, v) != NULL) return OP_FAILURE;	 // item(k, v) exists
    RecurInsert(&T->root, k, v);
    T->size += 1;

    return OP_SUCCESS;	// mute compiler
}

static void NodeCpy(AVLTreeNode **dest, AVLTreeNode *src)
{
    (*dest)->key   = src->key;
    (*dest)->value = src->value;
    return;
}

static AVLTreeNode *MinValueNode(AVLTreeNode *node)
{
    if (!node->left) return node;
    else
        return MinValueNode(node->left);
}

static AVLTreeNode *Succssor(AVLTreeNode *node)
{
    if (node->right)
        return MinValueNode(node->right);

    AVLTreeNode *p = node->parent;
    while (p && EQ(node->key, node->value, p->right))
    {
        node = p;
        p = p->parent;
    }
    return p;
}

static void RecurDelete(AVLTreeNode **node, int k, int v)
{
    if (!(*node)) return;
    else if (EQK(k, (*node)))
    {
        if (EQV(v, (*node)))
        {
            if (!(*node)->left && !(*node)->right)
            {
                // delete node with no child
                AVLTreeNode *p = (*node)->parent;
                if (p->left && EQ(k, v, p->left))
                    p->left = NULL;
                else if (p->right && EQ(k, v, p->right))
                    p->right = NULL;
                free((*node));
            }
            else if ((*node)->left && (*node)->right)
            {
                // delete node with two children
                AVLTreeNode *succ = Succssor((*node));
                NodeCpy(&(*node), succ);
                // delete succ from RHS
                RecurDelete(&(*node)->right, succ->key, succ->value);
            }
            else
            {
                // delete node with one child
                AVLTreeNode *child = ((*node)->left == NULL ?
                                      (*node)->right : (*node)->left);
                AVLTreeNode *parent = (*node)->parent;
                if ((*node)->left)
                {
                    *(*node) = *child;
                    (*node)->parent = parent;
                    (*node)->left = NULL;
                }
                else
                {
                    *(*node) = *child;
                    (*node)->parent = parent;
                    (*node)->right = NULL;
                }
                free(child);
            }
        }
        else if (LTV(v, (*node)))
            RecurDelete(&(*node)->left, k, v);
        else
            RecurDelete(&(*node)->right, k, v);
    }
    else if (LTK(k, (*node)))
        RecurDelete(&(*node)->left, k, v);
    else
        RecurDelete(&(*node)->right, k, v);

    /*
     * Rebalance the tree using similar but not the same mechanic
     */
    if (HeightDiffer((*node)) > 1 && HeightDiffer((*node)->left) >= 0)
        (*node) = RotateRight((*node));
    else if (HeightDiffer((*node)) > 1 && HeightDiffer((*node)->left) < 0)
    {
        (*node)->left = RotateLeft((*node)->left);
        (*node) = RotateRight((*node));
    }
    else if (HeightDiffer((*node)) < -1 && HeightDiffer((*node)->right) <= 0)
        (*node) = RotateLeft((*node));
    else if (HeightDiffer((*node)) < -1 && HeightDiffer((*node)->right) > 0)
    {
        (*node)->right = RotateRight((*node)->right);
        (*node) = RotateLeft((*node));
    }
}

// put your time complexity for DeleteNode() here
int DeleteNode(AVLTree *T, int k, int v)
{
    // put your code here
    assert(T != NULL);

#ifdef DEBUG
    printf("Now deleting with %d,%d\n", k, v);
#endif
    AVLTreeNode *n = Search(T, k, v);
    if (n == NULL) return OP_FAILURE;
    RecurDelete(&T->root, k, v);

    T->size -= 1;

    return OP_SUCCESS;
}

static AVLTreeNode *RecurSearch(AVLTreeNode *node, int k, int v)
{
    if (node == NULL) return NULL;
    else if (EQ(k, v, node))
        return node;
    else if (EQK(k, node))
    {
        if (LTV(v, node))
            return RecurSearch(node->left, k, v);
        else if (GTV(v, node))
            return RecurSearch(node->right, k, v);
    }
    else if (LTK(k, node))
        return RecurSearch(node->left, k, v);
    else if (GTK(k, node))
        return RecurSearch(node->right, k, v);
    else exit(EXIT_FAILURE);	// THIS SHOULD NOT HAPPEN
    return NULL;				// mute compiler
}

// put your time complexity analysis for Search() here
/* Time Complexity: O(log n) - Similar to the insertion, the given to-be-searched
 * node will only have to be compared with one node each level, which makes
 * the number of comparison be (log n)
 */
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
  	// put your code here
  	assert(T != NULL);
  	return RecurSearch(T->root, k, v);
}

// helper function - free each node out of the tree
static void FreeAVLTreeNode(AVLTreeNode *node)
{
    if (node == NULL) return;
    FreeAVLTreeNode(node->left);
    FreeAVLTreeNode(node->right);
    free(node);
}

// put your time complexity analysis for freeAVLTree() here
/* Time Complexity: O(n) - call free O(1) n times (i.e., recursively free 
 * the whole tree of n nodes) and free O(1) a single time for the pointer
 * holding tree itslef.
 */
void FreeAVLTree(AVLTree *T)
{
    // put your code here
    // TODO: to be checked in valgrind - CHECKED
    assert(T != NULL);
    FreeAVLTreeNode(T->root);
    free(T);
}

// helper fucntion - print each node out of the tree
static void PrintAVLTreeNodes(AVLTreeNode *node)
{
    if (!node) return;
    else
    {
        PrintAVLTreeNodes(node->left);
        printf("(%d, %d) [%d]\n", node->key, node->value, node->height);
        PrintAVLTreeNodes(node->right);
    }
}

// put your time complexity analysis for PrintAVLTree() here
/* Time Complexity: O(n) - call printf O(1) n times (i.e., recursively print
 * n nodes out of the tree).
 */
void PrintAVLTree(AVLTree *T)
{
    // put your code here
    assert(T != NULL);
    PrintAVLTreeNodes(T->root);
}

// A little formatted AVL Tree printer
/*********** AVL Tree Printer ***********/
#ifdef PRINT
#define MAX_DEPATH	1024
static char ascii[MAX_DEPATH];		// simulated ascii character stack
static int  di = 0;					// ascii self increment
static void Push( char c )
{
    ascii[di++] = ' ';
    ascii[di++] = c;
    ascii[di++] = ' ';
    ascii[di++] = ' ';
    ascii[di]   = '\0';
}

static void Pop( )
{
    ascii[di -= 4] = '\0';
}

static void RecurPrint(AVLTreeNode *root)
{
    printf("(%d, %d)\n", root->key, root->value);

    if (root->left != NULL)
    {
        printf("%s `--", ascii);
        Push('|');	// all left come first
        RecurPrint(root->left);
        Pop();
    }
    if (root->right != NULL)
    {
        printf("%s `--", ascii);
        Push(' ');	// on the contrary, all right branches come right after
        RecurPrint(root->right);
        Pop();
    }
}

static void ASCIITreePrinter(AVLTree *T)
{
    assert(T != NULL);
    if (!T->root)
    {
        printf("(*)\n");
        return;
    }
    memset(ascii, 0, MAX_DEPATH);
    printf("------------------------------------------\n");
    printf("AVL Tree Printer:\n");
    RecurPrint(T->root);
    printf("------------------------------------------\n");
    return;
}
#endif

int main() //sample main for testing
{
    AVLTree *tree1, *tree2;
    tree1=CreateAVLTree("File2.txt");
    PrintAVLTree(tree1);
#ifdef PRINT
    ASCIITreePrinter(tree1);
#endif
    printf("tree 1 of size: %d\n", tree1->size);
    /* if (DeleteNode(tree1, 2, 2) == OP_FAILURE) */
    /*     printf("Could not found.\n"); */
    /* ASCIITreePrinter(tree1); */
    DeleteNode(tree1, 2, 10);
#ifdef PRINT
    ASCIITreePrinter(tree1);
#endif
    /* DeleteNode(tree1, 50, 2); */
    /* ASCIITreePrinter(tree1); */
    /* DeleteNode(tree1, -17, 34); */
    /* ASCIITreePrinter(tree1); */
    /* DeleteNode(tree1, -19, 92); */
    /* ASCIITreePrinter(tree1); */
    /* DeleteNode(tree1, -2, 29); */
    /* ASCIITreePrinter(tree1); */
    /* DeleteNode(tree1, 2, 1); */
    /* ASCIITreePrinter(tree1); */
    /* DeleteNode(tree1, 2, 50); */
    tree2=CloneAVLTree(tree1);
    PrintAVLTree(tree1);
#ifdef PRINT
    ASCIITreePrinter(tree2);
#endif
    printf("tree 2 of size: %d\n", tree2->size);
    AVLTree *tree3 = CreateAVLTree("File1.txt");
#ifdef PRINT
    ASCIITreePrinter(tree3);
#endif
    printf("Intersection is\n");
    AVLTree *tree4 = AVLTreesIntersection(tree1, tree3);
    PrintAVLTree(tree4);
#ifdef PRINT
    ASCIITreePrinter(tree4);
#endif
    AVLTree *tree5 = AVLTreesUnion(tree1, tree3);
    PrintAVLTree(tree5);
#ifdef PRINT
    ASCIITreePrinter(tree5);
#endif
    FreeAVLTree(tree1);
    FreeAVLTree(tree2);
    FreeAVLTree(tree3);
    FreeAVLTree(tree4);
    FreeAVLTree(tree5);
    // int i,j;
    // AVLTree *tree1, *tree2, *tree3, *tree4;
    // AVLTreeNode *node1;

    // tree1=CreateAVLTree("stdin");
    // PrintAVLTree(tree1);
    // FreeAVLTree(tree1);
    // //you need to create the text file file1.txt
    // // to store a set of items without duplicate items
    // tree2=CreateAVLTree("file1.txt");
    // PrintAVLTree(tree2);
    // tree3=CloneAVLTree(tree2);
    // PrintAVLTree(tree3);
    // FreeAVLTree(tree2);
    // FreeAVLTree(tree3);
	 
    int i, j;
    AVLTree *tree6;
    AVLTreeNode *node1;
    tree6=newAVLTree();
    j=InsertNode(tree6, 10, 10);
    for (i=0; i<15; i++)
    {
    	j=InsertNode(tree6, i, i);
    	if (j==0) printf("(%d, %d) already exists\n", i, i);
    }
    PrintAVLTree(tree6);
#ifdef PRINT
    ASCIITreePrinter(tree6);
#endif
    node1=Search(tree6,20,20);
    if (node1!=NULL)
    	printf("key= %d value= %d\n",node1->key,node1->value);
    else
        printf("Key 20 does not exist\n");
	  
    for (i=17; i>0; i--)
    {
    	j=DeleteNode(tree6, i, i);
    	if (j==0)
    	  	printf("Key %d does not exist\n",i);
        PrintAVLTree(tree6);
#ifdef PRINT
        ASCIITreePrinter(tree6);
#endif
    }
    FreeAVLTree(tree6);
    return 0; 
}
