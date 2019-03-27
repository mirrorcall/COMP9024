#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

#define OP_SUCCESS 		1
#define OP_FAILURE		0

#define MAX(a, b)	((a > b) ? a : b)

// macros comparing key and/or value (a) and an AVLTreeNode (n)
#define EQ(k, v, n) (((k == n->key) &&          \
                      (v == n->value)) ? 1 : 0)
#define EQK(k, n)	(( k == n->key) ? 1 : 0)
#define LTK(k, n)	(( k <  n->key) ? 1 : 0)
#define GTK(k, n)	(( k >  n->key) ? 1 : 0)
#define LTV(k, n)	(( k <  n->value) ? 1 : 0)
#define GTV(k, n)	(( k >  n->value) ? 1 : 0)
// unguard macros - require exactly b of type AVLTreeNode

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

// put your time complexity analysis of CreateAVLTree() here
AVLTree *CreateAVLTree(const char *filename)
{
    // put your code here
    AVLTree *T = newAVLTree();
    FILE *fp = fopen(filename, "r");
    int key, value;

    // put extra whitespace to cosume the adjacent whitespace
    while ((fscanf(fp, "(%d, %d) ", &key, &value)) == 2)
    {
        if ((InsertNode(T, key, value)) == OP_FAILURE)
        {
#ifdef DEBUG
            printf("Found one - skipping.\n");
#endif
            continue;
        }
    }
    fclose(fp);

  	return T;
}

// helper function - recursively clone all nodes of a AVLTree into another
void CloneAVLTreeNodes(AVLTreeNode **dest, AVLTreeNode *src)
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
 
// put your time complexity for ALVTreesUNion() here
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
    // put your code here
    AVLTree *root = newAVLTree();

    return root;
}
 
// put your time complexity for ALVTreesIntersection() here
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
    // put your code here
    AVLTree *root = newAVLTree();

    return root;

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

int HeightDiffer(AVLTreeNode *node)
{
    return( Height(node->left) - Height(node->right) );
}

AVLTreeNode *RotateRight(AVLTreeNode *z)
{
    AVLTreeNode *y = z->left;
    AVLTreeNode *s = y->right;    // left child of y must be of height of 0 

    // redirecting
    y->right = z;
    z->left  = s;

    // updating height
    // TODO: check updated heights 
    y->height = Height(y) - 1;
    z->height = Height(z) - 1;

    return y;     // now y becomes the new root;
}

AVLTreeNode *RotateLeft(AVLTreeNode *z)
{
    AVLTreeNode *y = z->right;
    AVLTreeNode *s = y->left;        // left child of y must be of height of 0 

    // redirecting
    y->left  = z;
    z->right = s;

    // TODO: check updated heights 
    y->height = Height(y) - 1;
    z->height = Height(z) - 1;

    return y;     // now y becomes the new root;
}

// helper fucntion - insert each node recursively
void RecurInsert(AVLTreeNode **node, int k, int v)
{
    if (*node == NULL)
    {
        *node = newAVLTreeNode(k, v);
        return;
    }
    else
    {
        if (EQK(k, (*node)))
        {	// using value as minor key
            if (LTV(v, (*node)))
            {
                RecurInsert(&(*node)->left, k, v);
                (*node)->left->parent = (*node);
                (*node)->height = Height(*node) - 1;
            }
            else if (GTV(v, (*node)))
            {
                RecurInsert(&(*node)->right, k, v);
                (*node)->right->parent = (*node);
                (*node)->height = Height(*node) - 1;
            }
            else exit(EXIT_FAILURE); // THIS SHOULD NOT HAPPEN - EQV
        }
        else if (LTK(k, (*node)))
        {
            RecurInsert(&(*node)->left, k, v);
            (*node)->left->parent = (*node);
            (*node)->height = Height(*node) - 1;
        }
        else if (GTK(k, (*node)))	// splited for logical resaon
        {
            RecurInsert(&(*node)->right, k, v);
            (*node)->right->parent = (*node);
            (*node)->height = Height(*node) - 1;
        }
        else exit(EXIT_FAILURE);	// THIS SHOULD NOT HAPPEN
    }
    /*
     * rebalance the tree
     * HeightDiffer is the difference between left and right
     */
    { 
        (*node) = RotateRight((*node));
    }
    else if (HeightDiffer((*node)) < -1 && GTK(k, (*node)->right))
    {
        (*node) = RotateLeft((*node));
    }
    else if (HeightDiffer(*node) < -1 && LTK(k, (*node)->right))
    {
        (*node)->right = RotateRight((*node)->right);
        (*node) = RotateLeft((*node));
    }
    else if (HeightDiffer(*node) > 1 && GTK(k, (*node)->left))
    {
        (*node)->left = RotateLeft((*node)->left);
        (*node) = RotateRight((*node));
    }
}

// put the time complexity analysis for InsertNode() here
/* Time Complexity: O(log n) - To insert a node in an AVL Tree, the node will
 * only have to be compared with one node in each level. Say there are n 
 * nodes in the tree, so there are maximal (log n) levels in total based on
 * that AVL tree is a balanced tree. Then, it is obvious that the insertion
 * takes time of (log n) plus (log n) of performing search at the beginning,
 * which is clearly O(log n) (i.e., (2 * log n) dominated by (log n) by Master
 * Theorem).
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

void NodeCpy(AVLTreeNode **dest, AVLTreeNode *src)
{
    (*dest)->key   = src->key;
    (*dest)->value = src->value;
    return;
}

AVLTreeNode *MinValueNode(AVLTreeNode *node)
{
    if (!node->left) return node;
    else 
        return MinValueNode(node->left);
}

AVLTreeNode *Succssor(AVLTreeNode *node)
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

// put your time complexity for DeleteNode() here
int DeleteNode(AVLTree *T, int k, int v)
{
    // put your code here
    assert(T != NULL);

    AVLTreeNode *n = Search(T, k, v);
    if (n == NULL) return OP_FAILURE;

    if (!n->right && !n->left)
    {	// delete leaf node with no children
        AVLTreeNode *parent = n->parent;
        if (parent->left && EQ(k, v, parent->left))
            parent->left = NULL;
        else if (parent->right && EQ(k, v, parent->right))
            parent->right = NULL;
        free(n);
    }
    else if (n->left && !n->right)
    {	// delete node with only one child
        NodeCpy(&n, n->left);
        AVLTreeNode *freed = n->left;
        n->left = NULL;
        free(freed);
    }
    else if (n->right && !n->left)
    {	// delete node with only one child
        NodeCpy(&n, n->right);
        AVLTreeNode *freed = n->right;
        n->right = NULL;
        free(freed);
    }
    else
    {	// delete node with two children
        AVLTreeNode *succ = Succssor(n);
        AVLTreeNode *p = succ->parent;
        NodeCpy(&n, succ);
        if (p->left && EQ(succ->key, succ->value, p->left))
            p->left = NULL;
        else if (p->right && EQ(succ->key, succ->value, p->right))
            p->right = NULL;
        free(succ);
    }

    T->size -= 1;

    return OP_SUCCESS;
}

AVLTreeNode *RecurSearch(AVLTreeNode *node, int k, int v)
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
void FreeAVLTreeNode(AVLTreeNode *node)
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
void PrintAVLTreeNodes(AVLTreeNode *node)
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
    tree1=CreateAVLTree("File1.txt");
    PrintAVLTree(tree1);
#ifdef PRINT
    ASCIITreePrinter(tree1);
#endif
    printf("%d %d\n", MinValueNode(tree1->root)->key, MinValueNode(tree1->root)->value);
    printf("tree 1 of size: %d\n", tree1->size);
    DeleteNode(tree1, 50, 2);
    DeleteNode(tree1, -17, 34);
    DeleteNode(tree1, -19, 92);
    DeleteNode(tree1, -2, 29);
    DeleteNode(tree1, 2, 1);
    DeleteNode(tree1, 2, 50);
    printf("%d %d\n", MinValueNode(tree1->root)->key, MinValueNode(tree1->root)->value);
    tree2=CloneAVLTree(tree1);

#ifdef PRINT
    ASCIITreePrinter(tree2);
#endif
    printf("tree 2 of size: %d\n", tree2->size);
    FreeAVLTree(tree1);
    FreeAVLTree(tree2);
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
	 
    // tree4=newAVLTree();
    // j=InsertNode(tree4, 10, 10);
    // for (i=0; i<15; i++)
    // {
    // 	j=InsertNode(tree4, i, i);
    // 	if (j==0) printf("(%d, %d) already exists\n", i, i);
    // }
    // PrintAVLTree(tree4);
    // node1=Search(tree4,20,20);
    // if (node1!=NULL)
    // 	printf("key= %d value= %d\n",node1->key,node1->value);
    // else 
    //     printf("Key 20 does not exist\n");
	  
    // for (i=17; i>0; i--)
    // {
    // 	j=DeleteNode(tree4, i, i);
    // 	if (j==0) 
    // 	  	printf("Key %d does not exist\n",i);  
    //     PrintAVLTree(tree4);
    // }
    // FreeAVLTree(tree4);
    return 0; 
}
