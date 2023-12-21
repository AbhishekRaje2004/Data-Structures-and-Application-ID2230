#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int height;
};

/************************************
 * Return the maximum of two numbers
 ************************************/
int max(int num1, int num2)
{
    return (num1 > num2) ? num1 : num2;
}

/************************************
 * Return the height of a node
 ************************************/
int getHeight(struct Node *Root)
{
    if (Root == NULL)
    {
        return 0;
    }
    return max(getHeight(Root->left), getHeight(Root->right)) + 1;
}

/************************************
 * Returns root after right rotate
 ************************************/
struct Node *RightRotate(struct Node *y)
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
    return x;
}

/************************************
 * Returns root after left rotate
 ************************************/
struct Node *LeftRotate(struct Node *x)
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
    return y;
}

/************************************
 * AVL Tree Creation
 ************************************/
struct Node *AVLTree(struct Node *Root, int data)
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
        Root->left = AVLTree(Root->left, data);
        Root->left->parent = Root;
    }
    else if (Root->data < data)
    {
        // Insert at the right subtree
        Root->right = AVLTree(Root->right, data);
        Root->right->parent = Root;
    }
    else
    {
        return Root;
    }
    // Calculate the balance factor
    int balance = getHeight(Root->left) - getHeight(Root->right);
    if (balance > 1 && data < Root->left->data)
    {
        // Left-Left case
        return RightRotate(Root);
    }
    if (balance < -1 && data > Root->right->data)
    {
        // Right-Right case
        return LeftRotate(Root);
    }
    if (balance > 1 && data > Root->left->data)
    {
        // Left-Right case
        Root->left = LeftRotate(Root->left);
        return RightRotate(Root);
    }
    if (balance < -1 && data < Root->right->data)
    {
        // Right-Left case
        Root->right = RightRotate(Root->right);
        return LeftRotate(Root);
    }
    return Root;
}

/************************************
 * Function to Insert in AVL Tree
 ************************************/
void Insert(struct Node **Root, int data)
{
    *Root = AVLTree(*Root, data);
}

/************************************
 * Inorder Traversal of AVL Tree
 ************************************/
void inorder(struct Node *Root, FILE *file)
{
    if (Root == NULL)
    {
        return;
    }
    inorder(Root->left, file);
    fprintf(file, "%d(h=%d) ", Root->data, getHeight(Root));
    inorder(Root->right, file);
}

/**************************************************
 * Finding the Minimum number in the subtree of a node
 ***************************************************/
struct Node *findMin(struct Node *node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

/************************************
 * Function to Delete Node
 ************************************/
struct Node *deleteNode(struct Node *root, int key)
{
    if (root == NULL)
    {
        return root;
    }
    if (key < root->data)
    {
        root->left = deleteNode(root->left, key);
        if (root->left != NULL)
        {
            root->left->parent = root;
        }
    }
    else if (key > root->data)
    {
        root->right = deleteNode(root->right, key);
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

            return temp;
        }
        else if (root->right == NULL)
        {
            struct Node *temp = root->left;
            free(root);

            return temp;
        }
        struct Node *temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
        if (root->right != NULL)
        {
            root->right->parent = root;
        }
    }
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getHeight(root->left) - getHeight(root->right);

    // Perform rotations if necessary to maintain AVL balance
    if (balance > 1 && getHeight(root->left->left) >= getHeight(root->left->right))
    {
        // Left-Left case
        return RightRotate(root);
    }
    if (balance > 1 && getHeight(root->left->left) < getHeight(root->left->right))
    {
        // Left-Right case
        root->left = LeftRotate(root->left);
        return RightRotate(root);
    }
    if (balance < -1 && getHeight(root->right->right) >= getHeight(root->right->left))
    {
        // Right-Right case
        return LeftRotate(root);
    }
    if (balance < -1 && getHeight(root->right->right) < getHeight(root->right->left))
    {
        // Right-Left case
        root->right = RightRotate(root->right);
        return LeftRotate(root);
    }
    return root;
}
int searchAVL(struct Node *Root, int key, int *probes)
{
    if (Root == NULL)
    {
        (*probes)++;
        return 0;
    }
    if (Root->data == key)
    {
        (*probes)++;
        return 1;
    }
    else if (key < Root->data)
    {
        (*probes)++;
        return searchAVL(Root->left, key, probes);
    }
    else
    {
        (*probes)++;
        return searchAVL(Root->right, key, probes);
    }
}

/************************************
 * Delete All Nodes of a Tree
 ************************************/
void freeTree(struct Node *root)
{
    if (root == NULL)
    {
        return;

        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
/****************************************
 * Create Array from charaters in a file
 *****************************************/
void CreateArray(const char *filename, int *array, int count)
{
    FILE *file = fopen(filename, "r");

    int numRead = 0;
    for (int i = 0; i < count; i++)
    {
        if (fscanf(file, "%d", &array[i]) == 1)
        {
            numRead++;
        }
        else
        {
            break;
        }
    }

    fclose(file);
}
/****************************************
 *  Hash Function
 *****************************************/
int HashFunction(int key)
{
    return key;
}
/***********************************************************************************
 * Create Sub Array from orignal  Array containing random elements of that number
 ***********************************************************************************/
void SubArray(int *originalArray, int *shrunkenArray, int originalSize, int newSize)
{

    srand(time(NULL));

    for (int i = 0; i < newSize; i++)
    {
        int randomIndex = rand() % originalSize;
        shrunkenArray[i] = originalArray[randomIndex];
    }
}
/***********************************************************
 * Remove Common Elements of one Array from another
 ***********************************************************/
void RemoveCommonElements(int *arr1, int *arr2, int *size1, int *size2)
{
    int newSize = 0;

    for (int i = 0; i < *size1; i++)
    {
        int found = 0;

        for (int j = 0; j < *size2; j++)
        {
            if (arr1[i] == arr2[j])
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            arr1[newSize] = arr1[i];
            newSize++;
        }
    }

    *size1 = newSize;
}
int main()
{
    const char *filename = "RandomNumbers.txt";
    int count = 1000;
    int searchcount = 100;
    int size = 50;
    int probes = 0;
    float sum_probes = 0;
    //Initializing Random Array with 1000 elements
    int RandomArray[count];
        //Initializing Random Array with 100 elements
    int SearchArray[searchcount];

    CreateArray(filename, RandomArray, count);
    //Create Search Array havign 100 random elements of RandomArray
    SubArray(RandomArray, SearchArray, count, searchcount);
    struct Node *HashTable[size];
    //Initializing the Roots of All the trees to NULL
    for (int i = 0; i < size; i++)
    {
        HashTable[i] = NULL;
    }
    struct Node *Root = NULL;
    //Inserting all the 1000 elements of the Array in the HashTable
    for (int i = 0; i < count; i++)
    {
        int index = HashFunction(RandomArray[i]) % size;
        Insert(&HashTable[index], RandomArray[i]);
    }

    FILE *outputFile = fopen("output.txt", "w");

    fprintf(outputFile, "1000 numbers in Random Array:\n");
    for (int i = 0; i < count; i++)
    {
        fprintf(outputFile, "%d ", RandomArray[i]);
    }
    fprintf(outputFile, "\n");

    fprintf(outputFile, "100 elements for search:\n");
    for (int i = 0; i < searchcount; i++)
    {
        fprintf(outputFile, "%d ", SearchArray[i]);
    }
    fprintf(outputFile, "\n");

    // a) 100 successful searches
    fprintf(outputFile, "a) 100 successful searches:\n");
    for (int i = 0; i < searchcount; i++)
    {
        int index = HashFunction(SearchArray[i]) % size;
        probes = 0;
        int x = searchAVL(HashTable[index], SearchArray[i], &probes);
        // x is 0 if the search returns false
        if (x == 0)
        {
            fprintf(outputFile, "Element %d not found. Probes: %d\n", SearchArray[i], probes);
        }
        else
        {
            fprintf(outputFile, "Element %d found. Probes: %d\n", SearchArray[i], probes);
        }
        sum_probes += probes;
    }
    //mainting sum of probes for averaging
    fprintf(outputFile, "Average Probes for successful searches: %f\n", sum_probes / searchcount);

    // b) Delete the keys from the AVL tree
    for (int i = 0; i < searchcount; i++)
    {   //Deleting the element from the specific Index of the AVL Tree
        int index = HashFunction(SearchArray[i]) % size;
        HashTable[index] = deleteNode(HashTable[index], SearchArray[i]);
    }

    // c) Remove common elements between SearchArray and RandomArray
    fprintf(outputFile, "c) Removing common elements from RandomArray:\n");
    int newSize = 0;
    for (int i = 0; i < count; i++)
    {
        int found = 0;
        for (int j = 0; j < searchcount; j++)
        {
            if (RandomArray[i] == SearchArray[j])
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            RandomArray[newSize] = RandomArray[i];
            newSize++;
        }
    }
    count = newSize;
    //Creating a New SubArray for successful searches
    SubArray(RandomArray, SearchArray, count, searchcount);

    // d) 100 successful searches with the modified SearchArray
    //performing search operation again
    fprintf(outputFile, "d) 100 successful searches with the modified SearchArray:\n");
    sum_probes = 0;
    for (int i = 0; i < searchcount; i++)
    {
        int index = HashFunction(SearchArray[i]) % size;
        probes = 0;
        int x = searchAVL(HashTable[index], SearchArray[i], &probes);
        if (x == 0)
        {
            fprintf(outputFile, "Element %d not found. Probes: %d\n", SearchArray[i], probes);
        }
        else
        {
            fprintf(outputFile, "Element %d found. Probes: %d\n", SearchArray[i], probes);
        }
        //The Number of Probes this time will be lesser as 100 elements have been deleted
        sum_probes += probes;
    }
    fprintf(outputFile, "Average Probes for successful searches (modified SearchArray): %f\n", sum_probes / searchcount);

    // e) 100 unsuccessful searches
    fprintf(outputFile, "e) 100 unsuccessful searches:\n");
    sum_probes = 0;
    //doing the operation for 100 unsucesfull searches
    for (int i = 0; i < searchcount; i++)
    {
        int randomKey = rand() % 10000; 
        int index = HashFunction(randomKey) % size;
        probes = 0;
        int x = searchAVL(HashTable[index], randomKey, &probes);
        if (x == 0)
        {
            fprintf(outputFile, "Element %d not found. Probes: %d\n", randomKey, probes);
        }
        sum_probes += probes;
    }
    fprintf(outputFile, "Average Probes for unsuccessful searches: %f\n", sum_probes / searchcount);

    fclose(outputFile);

    // Free memory
    for (int i = 0; i < size; i++)
    {
        freeTree(HashTable[i]);
    }
    printf("Files Generated");
    return 0;
}
