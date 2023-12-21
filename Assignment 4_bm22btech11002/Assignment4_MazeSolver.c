/***********************************************
 * Assignment 4
 * Name:    Abhishek Raje
 * Course:  Data Structures and Applications
 * Questionc:1
 * Submitted:    20/11/2023
 ***********************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_X_RANGE 40
#define MAX_Y_RANGE 40
/***********************************************
 *Defining a structure location for storing coordinates 
 ***********************************************/
struct location
{
    int x_cord;
    int y_cord;
};
/***********************************************
 *Creating a queue
 ***********************************************/
struct queue
{
    int size;
    int front;
    int back;
    struct location position[MAX_X_RANGE * MAX_Y_RANGE];//queue stores array of locations
};
/***********************************************
 * Function to check if queue is full
 ***********************************************/
bool is_full(struct queue *q)
{
    if ((q->back == q->size - 1))//condition for queue being full
    {
        return true;
    }
    return false;
}

/***********************************************
 * Function to check if queue is empty
 ***********************************************/
bool is_empty(struct queue *q)
{
    if (q->front == q->back)//condition to check if queue is full
    {
        return true;
    }
    return false;
}

/***********************************************
*Enqueing coordinates in the queue
 ***********************************************/
void enque(struct queue *q, struct location cord)
{
    if (is_full(q))
    {
        return;
    }
    q->position[q->back] = cord;//inserting cord in the queue
    q->back++;//updating back pointer
    
}
/***********************************************
 * Function to deuqe cordinate from the queue
 ***********************************************/
void dequeue(struct queue *q)
{
    if (is_empty(q))
    {
        return;
    }
        q->front++;
    
}

/***********************************************
 * Function to check if coordinates are in bounds
 ***********************************************/
bool in_bounds(struct location cord_curr, int x_range, int y_range)
{
    if (cord_curr.x_cord >= 0 && cord_curr.y_cord >= 0 && cord_curr.x_cord < x_range && cord_curr.y_cord < y_range)
    {
        return true;
    }
    return false;
}

/***********************************************
 * Function to check if coordinates of the maze correspond to a wall
 ***********************************************/
bool check_wall(char maze[MAX_Y_RANGE][MAX_X_RANGE], struct location cord_curr)
{
    if (maze[cord_curr.y_cord][cord_curr.x_cord] == '#')
    {
        return true;
    }
    return false;
}
/***********************************************
 * Function to check if current coordinates correspond to the end
 ***********************************************/
bool maze_solved(struct location cord_curr, struct location cord_end)
{
    if (cord_curr.x_cord == cord_end.x_cord && cord_curr.y_cord == cord_end.y_cord)
    {
        return true;
    }
    return false;
}
/***********************************************
 * Function to find the start and end coordinates
 ***********************************************/
void find_cords(int x_range, int y_range,struct location *cord_start, struct location *cord_end, char maze[MAX_Y_RANGE][MAX_X_RANGE])
{
    for (int i = 0; i < y_range; i++)
    {
        for (int j = 0; j < x_range+1; j++)
        {
            char cur = maze[i][j];
            if (maze[i][j] == 'S')
            {
                cord_start->x_cord = j;
                cord_start->y_cord = i;
            }
            if (maze[i][j] == 'E')
            {
                cord_end->x_cord = j;
                cord_end->y_cord = i;
            }
        }
    }
}
/***********************************************
 * initializing the queue 
 ***********************************************/
void initialize(struct queue *q)
{
    q->size = MAX_X_RANGE * MAX_Y_RANGE;
    q->front = q->back = 0;
}
/***********************************************
 * Markling the path where the current cord has visited
 ***********************************************/
void mark_path(int y_range,int x_range,char maze[MAX_Y_RANGE][MAX_X_RANGE], struct location path[], int path_length, struct location cord_start, struct location cord_end)
{
    for (int i = 0; i < path_length; i++)
    {
        if ((path[i].x_cord != cord_start.x_cord || path[i].y_cord != cord_start.y_cord) &&
            (path[i].x_cord != cord_end.x_cord || path[i].y_cord != cord_end.y_cord))
        {
            maze[path[i].y_cord][path[i].x_cord] = '.';// if path correspond to maze then mark it as '.'
        }
    }
    //print the maze
    for (int i = 0; i < y_range; i++)
    {
        for (int j = 0; j < x_range+1; j++)
        {
            printf("%c", maze[i][j]);
        }
    }
}
/***********************************************
 * Solving the maze using bfs
 ***********************************************/
void maze_solver( int y_range,int x_range, char maze[MAX_Y_RANGE][MAX_X_RANGE], struct location *cord_start, struct location *cord_end)
{   
    struct queue q;
    initialize(&q);
    struct location current;
    struct location neighbour;
    struct location matrix;
    struct location path[MAX_Y_RANGE * MAX_X_RANGE];//creating array of structure of location
    int path_length = 0;
    find_cords( x_range, y_range,cord_start, cord_end, maze);//finding the start and end coordinates
    current.x_cord = cord_start->x_cord;//storing the current coordinates as start
    current.y_cord = cord_start->y_cord;
    enque(&q, current);//enqueing current in the queue
    bool visited[y_range][x_range];
    int adjMatrix[y_range*x_range][x_range*y_range];
    for(int i=0;i<y_range*x_range;i++)
    {
        for(int j=0;j<x_range*y_range;j++)
        {
            adjMatrix[i][j]=0;
        }
    }
    for (int i = 0; i < y_range; i++)
    {
        for (int j = 0; j < x_range+1; j++)
        {
          visited[i][j]=false;
        }
    }
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    while (current.x_cord!=cord_end->x_cord && current.y_cord!=cord_end->y_cord)
    {   
        //making the current coordinates as the front of the que
        current = q.position[q.front];
        for (int i = 0; i < 4; i++)
        {
            neighbour.x_cord = current.x_cord + directions[i][0];
            neighbour.y_cord = current.y_cord + directions[i][1];

            if (in_bounds(neighbour, x_range, y_range) && !visited[neighbour.y_cord][neighbour.x_cord] && !check_wall(maze, neighbour))
            {   
                adjMatrix[current.x_cord][neighbour.x_cord*x_range+current.y_cord]=1;
            }
        }
    }
    if(current.y_cord=y_range-1)
    {
        
    }
    for (int i = 0; i <y_range*y_range; i++)
    {
        for(int j=0;j<x_range*y_range;j++)
        {
            printf("%d ",adjMatrix[i][j]);
        }
    }
    
}

int main()
{
    FILE *fptr;
    char ch;
    struct location cord_start;
    struct location cord_end;
    fptr = fopen("maze1.txt", "r");
    char maze[MAX_Y_RANGE][MAX_X_RANGE];
    memset(maze, 0, MAX_X_RANGE*MAX_Y_RANGE);
    int x_range = 0;
    int y_range = 0;    
    int max_range = 0;
    while((ch=fgetc(fptr))!=EOF)
    {
        maze[y_range][x_range] = ch;
        if(ch=='\n')
        {
            y_range++;
            if (x_range>max_range)
                max_range = x_range;
            x_range = 0;
        }
        else
        {
           
            x_range++;
        }
    }
    x_range = max_range;
    fseek(fptr,0,SEEK_SET);
    maze_solver(y_range, x_range,maze, &cord_start, &cord_end );
    
    fclose(fptr);
    return 0;
}
