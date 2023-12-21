/***********************************************
 * Assignment 1
 * Name:    Abhishek Raje
 * Course:     Data Structures and Applications
 * Questionc:1
 * Submitted:    3/9/2023
 ***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct deque
{
    int size;
    int front;
    int back;
    int *array;
};

/** checks if the deque is full
 *
 */
bool is_full(struct deque *dq)
{
    if ((dq->front == 0 && dq->back == dq->size - 1) || dq->back + 1 == dq->front)
    {
        return true;
    }
    return false;
}
/** checks if the deque is empty
 *
 */
bool is_empty(struct deque *dq)
{
    if ((dq->front == dq->back))
    {
        dq->front = dq->back = 0;
        return true;
    }
    return false;
}
/** inserts an element from the front postion
 *
 */
void addfirst(struct deque *dq, int element)
{
    if (is_full(dq))
    {
        printf("Queue is Full....Queue Overflow\n");
        return;
    }
    // handling the exceptional case of enqueing from front when front is at 0 index
    if (dq->front == 0)
    {
        dq->front = dq->size - 1;
    }
    else
    {
        dq->front--;
    }
    dq->array[dq->front] = element;
}
/** inserts an element from the back postion
 *
 */
void addlast(struct deque *dq, int element)
{
    if (is_full(dq))
    {
        printf("Queue is Full....Queue Overflow\n");
        return;
    }
    dq->array[dq->back] = element;
    // handling the exceptional case of enqueing from back when back is at size-1 index

    if (dq->back == dq->size - 1)
    {
        dq->back = 0;
    }
    else
    {
        dq->back++;
    }
}
/** removes an element from the first postion
 *
 */
int removefirst(struct deque *dq)
{
    if (is_empty(dq))
    {
        printf("Queue is Empty....Queue Underflow\n");
        return -1;
    }
    int removed_element = dq->array[dq->front];
    // impleneting circular buffer
    if (dq->front == dq->size - 1)
    {
        dq->front = 0;
    }
    else
    {
        dq->front++;
    }
    return removed_element;
}
/** removes an element from the back postion
 *
 */
int removelast(struct deque *dq)
{
    if (is_empty(dq))
    {
        printf("Queue is Empty....Queue Underflow\n");
        return -1;
    }

    if (dq->back == 0)
    {
        dq->back = dq->size - 1;
    }
    else
    {
        dq->back--;
    }
    int removed_element = dq->array[dq->back];
    return removed_element;
}
/** displays an array while implementing circular buffer
 *
 */
void displayDeque(struct deque *dq)
{
    int i = dq->front;
    if (dq->front < dq->back)
    {
        while (i != dq->back)
        {
            printf("%d\t", dq->array[i]);
            i++;
        }
    }
    else if (dq->front > dq->back)
    {
        int i = dq->front;
        while (i != dq->size)
        {
            printf("%d\t", dq->array[i]);
            i++;
        }
        i = 0;
        while (i != dq->back)
        {
            printf("%d\t", dq->array[i]);
            i++;
        }
    }
    printf("\n");
}
int main()
{
    int choice, element;
    char ch;
    // initializing a  dq with data type struct deque
    struct deque *dq = (struct deque *)malloc(sizeof(struct deque));
    dq->front = dq->back = 0;
    printf("Enter size of Deque:");
    scanf("%d", &(dq->size));
    dq->size++;
    // dynamically allocating memory in the heap
    dq->array = (int *)malloc(dq->size * sizeof(int));
    while (choice != 0)
    {
        printf("Which Operation do you wish to Perform \n1.add first(),\n2.removefirst(),\n3.add last()\n4.remove last()\n");
        printf("Enter 0 to exit:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter Element you wish to insert:");
            scanf("%d", &element);
            addfirst(dq, element);
            break;
        case 2:
            removefirst(dq);
            break;
        case 3:
            printf("Enter Element you wish to insert:");
            scanf("%d", &element);
            addlast(dq, element);
            break;
        case 4:
            removelast(dq);
            break;
        }
        printf("Deque Currently:\n");
        displayDeque(dq);
    }
    free(dq->array);
    free(dq);
}