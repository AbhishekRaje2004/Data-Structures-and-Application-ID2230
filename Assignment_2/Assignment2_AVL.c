/********************************************
 * Name: Abhishek Raje
 * Roll No: bm22btech11002
 * Assignment 2
 * AVL Tree
 *********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the number of keys, number of trees, and number of keys to delete
#define ARRAY_SIZE 1000
#define NUM_ARRAYS 100
#define KEYS_TO_DELETE 100

/************************************
 * Define the properties of Node
 ************************************/
struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int height;
};

/************************************
 * Define a Structure WorkDone
 ************************************/
struct WorkDone
{
    int comparisons;
    int iterations;
};

/************************************
 * Return the maximum of two numbers
 ************************************/
int max(int num1, int num2, struct WorkDone *work)
{
    work->iterations++;
    return (num1 > num2) ? num1 : num2;
}

/************************************
 * Return the height of a node
 ************************************/
int getHeight(struct Node *Root, struct WorkDone *work)
{
    if (Root == NULL)
    {
        return 0;
    }
    work->iterations++;
    return max(getHeight(Root->left, work), getHeight(Root->right, work), work) + 1;
}

/************************************
 * Returns root after right rotate
 ************************************/
struct Node *RightRotate(struct Node *y, struct WorkDone *work)
{
    // Define x as the child of y
    struct Node *x = y->left;
    struct Node *Tree = x->right;

    // Update parent pointers
    x->parent = y->parent;
    y->parent = x;
    if (Tree != NULL)
        Tree->parent = y;

    // Perform the rotation operation
    x->right = y;
    y->left = Tree;
    work->iterations++;
    return x;
}

/************************************
 * Returns root after left rotate
 ************************************/
struct Node *LeftRotate(struct Node *x, struct WorkDone *work)
{
    // Define y as the child of x
    struct Node *y = x->right;
    struct Node *Tree = y->left;

    // Update the parent pointer
    y->parent = x->parent;
    x->parent = y;
    if (Tree != NULL)
        Tree->parent = x;

    // Perform the rotation operation
    y->left = x;
    x->right = Tree;
    work->iterations++;
    return y;
}

/************************************
 * AVL Tree Creation
 ************************************/
struct Node *AVLTree(struct Node *Root, int data, struct WorkDone *work)
{
    if (Root == NULL)
    {
        // Define properties of a leaf node
        Root = (struct Node *)malloc(sizeof(struct Node));
        Root->data = data;
        Root->left = NULL;
        Root->right = NULL;
        Root->height = 1;
        return Root;
    }
    if (Root->data > data)
    {
        // Insert at the left subtree
        work->comparisons++;
        work->iterations++;
        Root->left = AVLTree(Root->left, data, work);
        Root->left->parent = Root;
    }
    else if (Root->data < data)
    {
        // Insert at the right subtree
        work->comparisons++;
        work->iterations++;
        Root->right = AVLTree(Root->right, data, work);
        Root->right->parent = Root;
    }
    else
    {
        return Root;
    }
    // Calculate the balance factor
    int balance = getHeight(Root->left, work) - getHeight(Root->right, work);
    work->iterations++;
    if (balance > 1 && data < Root->left->data)
    {
        // Left-Left case
        work->iterations++;
        return RightRotate(Root, work);
    }
    if (balance < -1 && data > Root->right->data)
    {
        // Right-Right case
        work->iterations++;
        return LeftRotate(Root, work);
    }
    if (balance > 1 && data > Root->left->data)
    {
        // Left-Right case
        work->iterations++;
        Root->left = LeftRotate(Root->left, work);
        return RightRotate(Root, work);
    }
    if (balance < -1 && data < Root->right->data)
    {
        // Right-Left case
        work->iterations++;
        Root->right = RightRotate(Root->right, work);
        return LeftRotate(Root, work);
    }
    return Root;
}

/************************************
 * Function to Insert in AVL Tree
 ************************************/
void Insert(struct Node **Root, int data, struct WorkDone *work)
{
    *Root = AVLTree(*Root, data, work);
}

/************************************
 * Inorder Traversal of AVL Tree
 ************************************/
void inorder(struct Node *Root, FILE *file, struct WorkDone *work)
{
    if (Root == NULL)
    {
        return;
    }
    inorder(Root->left, file, work);
    fprintf(file, "%d(h=%d) ", Root->data, getHeight(Root, work));
    inorder(Root->right, file, work);
    work->iterations++;
}

/**************************************************
 * Finding the Minimum number in the subtree of a node
 ***************************************************/
struct Node *findMin(struct Node *node, struct WorkDone *work)
{
    while (node->left != NULL)
    {
        node = node->left;
        work->iterations++;
    }
    return node;
}

/************************************
 * Function to Delete Node
 ************************************/
struct Node *deleteNode(struct Node *root, int key, struct WorkDone *work)
{
    if (root == NULL)
    {
        work->iterations++;
        return root;
    }

    if (key < root->data)
    {
        work->comparisons++;
        work->iterations++;
        root->left = deleteNode(root->left, key, work);
        if (root->left != NULL)
        {
            root->left->parent = root;
        }
    }
    else if (key > root->data)
    {
        work->comparisons++;
        work->iterations++;
        root->right = deleteNode(root->right, key, work);
        if (root->right != NULL)
        {
            root->right->parent = root;
        }
    }
    else
    {
        if (root->left == NULL)
        {
            struct Node *temp = root->right;
            free(root);
            work->iterations++;
            return temp;
        }
        else if (root->right == NULL)
        {
            struct Node *temp = root->left;
            free(root);
            work->iterations++;
            return temp;
        }
        work->iterations++;
        struct Node *temp = findMin(root->right, work);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data, work);
        if (root->right != NULL)
        {
            root->right->parent = root;
        }
    }
    root->height = 1 + max(getHeight(root->left, work), getHeight(root->right, work), work);
    int balance = getHeight(root->left, work) - getHeight(root->right, work);

    // Perform rotations if necessary to maintain AVL balance
    if (balance > 1 && getHeight(root->left->left, work) >= getHeight(root->left->right, work))
    {
        // Left-Left case
        return RightRotate(root, work);
    }
    if (balance > 1 && getHeight(root->left->left, work) < getHeight(root->left->right, work))
    {
        // Left-Right case
        root->left = LeftRotate(root->left, work);
        return RightRotate(root, work);
    }
    if (balance < -1 && getHeight(root->right->right, work) >= getHeight(root->right->left, work))
    {
        // Right-Right case
        return LeftRotate(root, work);
    }
    if (balance < -1 && getHeight(root->right->right, work) < getHeight(root->right->left, work))
    {
        // Right-Left case
        root->right = RightRotate(root->right, work);
        return LeftRotate(root, work);
    }
    return root;
}

/************************************
 * Delete All Nodes of a Tree
 ************************************/
void freeTree(struct Node *root, struct WorkDone *work)
{
    if (root == NULL)
    {
        return;
    }
    work->iterations++;
    freeTree(root->left, work);
    freeTree(root->right, work);
    free(root);
}

/************************************
 * Randomly shuffle elements in an array
 ************************************/
void shuffleArray(int array[], int size, struct WorkDone *work)
{
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        work->iterations++;
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int main()
{
    srand(time(NULL));
    int ch, key;
    int listkey[NUM_ARRAYS][ARRAY_SIZE];
    struct Node *Root = NULL;
    struct WorkDone work;
    FILE *outputFile = fopen("output_AVL.txt", "w");
    int treeHeight[NUM_ARRAYS];
    int TotalWork[NUM_ARRAYS];
    for (int i = 0; i < NUM_ARRAYS; i++)
    {
        work.comparisons = work.iterations = 0;

        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            listkey[i][j] = j + 1;
        }

        shuffleArray(listkey[i], ARRAY_SIZE, &work);
        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            Insert(&Root, listkey[i][j], &work);
        }

        fprintf(outputFile, "Tree %d\n", i + 1);
        fprintf(outputFile, "Height of Tree : %d\n", getHeight(Root, &work));
        treeHeight[i] = getHeight(Root, &work);
        fprintf(outputFile, "Inorder Traversal of Tree : \n");
        inorder(Root, outputFile, &work);
        fprintf(outputFile, "\n");
        fprintf(outputFile, "\n");

        for (int j = 0; j < KEYS_TO_DELETE; j++)
        {
            key = rand() % ARRAY_SIZE + 1;

            Root = deleteNode(Root, key, &work);
        }

        fprintf(outputFile, "Tree %d After deleting 100 keys\n", i + 1);
        fprintf(outputFile, "Height of Tree : %d\n", getHeight(Root, &work));
        fprintf(outputFile, "Inorder Traversal of Tree : \n");
        inorder(Root, outputFile, &work);
        fprintf(outputFile, "\n");
        fprintf(outputFile, "-----------------------------------\n");
        fprintf(outputFile, "Comparisons for Tree: %d\n", work.comparisons);
        fprintf(outputFile, "Iterations for Tree: %d\n", work.iterations);
        fprintf(outputFile, "-----------------------------------\n");

        TotalWork[i] = work.comparisons + work.iterations;
        fprintf(outputFile, "\n");

        freeTree(Root, &work);
        Root = NULL;
    }

    fclose(outputFile);
    printf("File Created");
    return 0;
}
