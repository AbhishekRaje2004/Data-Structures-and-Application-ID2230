/********************************************
 * Name: Abhishek Raje
 * Roll No: bm22btech11002
 * Assignment 2
 * Binary Tree
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
    return Root->height; // Pointer associated with each node
}

/************************************
 * Create a Binary Tree
 ************************************/
struct Node *BinarySearchTree(struct Node *Root, int data, struct WorkDone *work)
{
    if (Root == NULL)
    { // Define properties of a leaf node
        Root = (struct Node *)malloc(sizeof(struct Node));
        Root->data = data;
        Root->left = NULL;
        Root->right = NULL;
        Root->height = 1;
        return Root;
    }
    if (Root->data > data)
    { // Insert at the left subtree
        work->comparisons++;
        work->iterations++;

        Root->left = BinarySearchTree(Root->left, data, work);
        Root->left->parent = Root;
    }
    else
    {
        // Insert at the right subtree
        work->comparisons++;
        work->iterations++;

        Root->right = BinarySearchTree(Root->right, data, work);
        Root->right->parent = Root;
    }
    // Update height pointer
    Root->height = 1 + max(getHeight(Root->left, work), getHeight(Root->right, work), work);
    work->iterations++;
    return Root;
}

/************************************
 * Insert Function which calls BST
 ************************************/
void Insert(struct Node **Root, int data, struct WorkDone *work)
{
    *Root = BinarySearchTree(*Root, data, work);
}

/************************************
 * Inorder Traversal of Binary Tree
 ************************************/
void inorder(struct Node *Root, FILE *file, struct WorkDone *work)
{
    if (Root == NULL)
    {
        return;
    }
    inorder(Root->left, file, work);
    fprintf(file, "%d(h=%d) ", Root->data, Root->height);
    inorder(Root->right, file, work);
    work->iterations++;
}

/**************************************************
 * Find the minimum number in the subtree of a node
 ***************************************************/
struct Node *findMin(struct Node *node, struct WorkDone *work)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    work->iterations++;
    return node;
}

/************************************
 * Function to Delete Node
 ************************************/
struct Node* deleteNode(struct Node *root, int key, struct WorkDone *work)
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

        // Key to delete is on the left of root
        root->left = deleteNode(root->left, key, work);
        // Update the parent pointer of root->left
        if (root->left != NULL)
        {
            root->left->parent = root;
        }
    }
    else if (key > root->data)
    {
        // Key to delete is on the right of root
        work->comparisons++;
        work->iterations++;

        root->right = deleteNode(root->right, key, work);
        // Update the parent pointer of root->right
        if (root->right != NULL)
        {
            root->right->parent = root;
        }
    }
    else
    { // Root has no left child
        if (root->left == NULL)
        {
            struct Node *temp = root->right;
            free(root);
            work->iterations++;
            return temp;
        }
        // Root has no right child
        else if (root->right == NULL)
        {
            struct Node *temp = root->left;
            free(root);
            work->iterations++;
            return temp;
        }
        // Root has both left and right children
        // Find the successor of the node
        struct Node *temp = findMin(root->right, work);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data, work);
        // Update the parent of root->right
        if (root->right != NULL)
        {
            root->right->parent = root;
        }
    }
    // Update the value of height
    root->height = 1 + max(getHeight(root->left, work), getHeight(root->right, work), work);
    work->iterations++;
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
void shuffleArray(int array[], int size, struct WorkDone work)
{
    for (int i = size - 1; i > 0; i--)
    {
        // j < i+1
        int j = rand() % (i + 1);
        work.iterations++;
        // Swap array[i] and array[j]
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
    FILE *outputFile = fopen("output_BST.txt", "w");

    // Add variables to keep track of comparisons and iterations
    int totalComparisons = 0;
    int totalIterations = 0;

    int treeHeight[NUM_ARRAYS];
    int TotalWork[NUM_ARRAYS];
    for (int i = 0; i < NUM_ARRAYS; i++)
    {
        work.comparisons = work.iterations = 0;

        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            listkey[i][j] = j + 1; // Create a matrix with each row having numbers from 1 to 1000
        }

        shuffleArray(listkey[i], ARRAY_SIZE, work); // Shuffle each row

        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            Insert(&Root, listkey[i][j], &work); // Insert the elements in the tree
        }

        fprintf(outputFile, "Tree %d\n", i + 1);
        fprintf(outputFile, "Height of Tree: %d\n", Root->height);
        treeHeight[i] = Root->height;
        fprintf(outputFile, "Inorder Traversal of Tree:\n");
        inorder(Root, outputFile, &work);
        fprintf(outputFile, "\n");
        fprintf(outputFile, "\n");

        for (int j = 0; j < KEYS_TO_DELETE; j++)
        {
            // Generate a random key less than ARRAY_SIZE
            key = rand() % ARRAY_SIZE;
            Root = deleteNode(Root, key, &work);
        }

        fprintf(outputFile, "Tree %d\n", i + 1);
        fprintf(outputFile, "Height of Tree After Deletion: %d\n", Root->height);
        fprintf(outputFile, "Inorder Traversal of Tree:\n");
        inorder(Root, outputFile, &work);
        fprintf(outputFile, "\n");
        fprintf(outputFile, "\n");
        fprintf(outputFile, "---------------------------------\n");

        // Print comparisons and iterations for each tree
        fprintf(outputFile, "Comparisons for Tree %d: %d\n", i + 1, work.comparisons);
        fprintf(outputFile, "Iterations for Tree %d: %d\n", i + 1, work.iterations);
        fprintf(outputFile, "---------------------------------\n");
        TotalWork[i]=work.comparisons+work.iterations;
        // Update the total comparisons and iterations
        totalComparisons += work.comparisons;
        totalIterations += work.iterations;

        // Clear the tree for the next tree
        freeTree(Root, &work);
        Root = NULL;
    }

    // Print the total comparisons and iterations
    fprintf(outputFile, "Total Comparisons: %d\n", totalComparisons);
    fprintf(outputFile, "Total Iterations: %d\n", totalIterations);
    fclose(outputFile);
    printf("File Created\n");
    return 0;
}
