#include <stdio.h>
#include<math.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_X_RANGE 40
#define MAX_Y_RANGE 40
using namespace std;
struct Node
{
    int dest;
    struct Node* next;
};

struct Graph
{
    int vertices;
    struct Node** array;
};

struct location
{
    int x_cord;
    int y_cord;
};

struct queue
{
    int size;
    int front;
    int back;
    struct location position[MAX_X_RANGE * MAX_Y_RANGE];
};

bool is_full(struct queue *q)
{
    return q->back == q->size - 1;
}

bool is_empty(struct queue *q)
{
    return q->front == q->back;
}

void enque(struct queue *q, struct location cord)
{
    if (!is_full(q))
    {
        q->position[q->back] = cord;
        q->back++;
    }
}

void dequeue(struct queue *q)
{
    if (!is_empty(q))
    {
        q->front++;
    }
}

bool in_bounds(struct location cord_curr, int x_range, int y_range)
{
    return (cord_curr.x_cord >= 0 && cord_curr.y_cord >= 0 && cord_curr.x_cord < x_range && cord_curr.y_cord < y_range);
}

bool check_wall(char maze[MAX_Y_RANGE][MAX_X_RANGE], struct location cord_curr)
{
    return maze[cord_curr.y_cord][cord_curr.x_cord] == '#';
}

bool maze_solved(struct location cord_curr, struct location cord_end)
{
    return (cord_curr.x_cord == cord_end.x_cord && cord_curr.y_cord == cord_end.y_cord);
}

struct Graph* buildGraph(char maze[MAX_Y_RANGE][MAX_X_RANGE], int x_range, int y_range)
{
    int vertices = x_range * y_range;
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->vertices = vertices;
    graph->array = (struct Node**)malloc(vertices * sizeof(struct Node*));

    for (int i = 0; i < vertices; ++i)
        graph->array[i] = NULL;

    for (int i = 0; i < y_range; ++i)
    {
        for (int j = 0; j < x_range; ++j)
        {
            if (maze[i][j] != '#')
            {
                int node_index = i * x_range + j;

                // Check neighboring cells
                if (i > 0 && maze[i - 1][j] != '#')
                {
                    int neighbor_index = (i - 1) * x_range + j;
                    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
                    newNode->dest = neighbor_index;
                    newNode->next = graph->array[node_index];
                    graph->array[node_index] = newNode;
                }

                if (j > 0 && maze[i][j - 1] != '#')
                {
                    int neighbor_index = i * x_range + (j - 1);
                    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
                    newNode->dest = neighbor_index;
                    newNode->next = graph->array[node_index];
                    graph->array[node_index] = newNode;
                }
            }
        }
    }

    return graph;
}

void BFS(struct Graph* graph, int start, int end)
{
    struct queue q;
    q.size = MAX_X_RANGE * MAX_Y_RANGE;
    q.front = q.back = 0;

    bool visited[MAX_X_RANGE * MAX_Y_RANGE] = {false};
    enque(&q, (struct location){start % MAX_X_RANGE, start / MAX_X_RANGE});
    visited[start] = true;

    while (!is_empty(&q))
    {
        struct location current = q.position[q.front];
        int node_index = current.y_cord * MAX_X_RANGE + current.x_cord;
        dequeue(&q);

        struct Node* neighbor = graph->array[node_index];
        while (neighbor != NULL)
        {
            if (!visited[neighbor->dest])
            {
                visited[neighbor->dest] = true;
                int x = neighbor->dest % MAX_X_RANGE;
                int y = neighbor->dest / MAX_X_RANGE;
                enque(&q, (struct location){x, y});
            }

            neighbor = neighbor->next;
        }
    }
}

int main()
{
    FILE* fptr;
    char ch;
    struct location cord_start;
    struct location cord_end;
    fptr = fopen("maze1.txt", "r");
    char maze[MAX_Y_RANGE][MAX_X_RANGE];
    memset(maze, 0, MAX_X_RANGE * MAX_Y_RANGE);
    int x_range = 0;
    int y_range = 0;
    int max_range = 0;

    while ((ch = fgetc(fptr)) != EOF)
    {
        maze[y_range][x_range] = ch;
        if (ch == '\n')
        {
            y_range++;
            if (x_range > max_range)
                max_range = x_range;
            x_range = 0;
        }
        else
        {
            x_range++;
        }
    }

    x_range = max_range;
    fseek(fptr, 0, SEEK_SET);

    struct Graph* graph = buildGraph(maze, x_range, y_range);
    int start = cord_start.y_cord * x_range + cord_start.x_cord;
    int end = cord_end.y_cord * x_range + cord_end.x_cord;

    BFS(graph, start, end);

    fclose(fptr);
    return 0;
}
