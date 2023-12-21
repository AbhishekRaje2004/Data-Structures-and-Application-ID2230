/***********************************************
 * Assignment 1
 * Name:    Abhishek Raje
 * Course:  Data Structures and Applications
 * Questionc:2
 * Submitted:    3/9/2023
 ***********************************************/
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_X_RANGE 40
#define MAX_Y_RANGE 40

struct location
{
    int x_cord;
    int y_cord;
};

struct deque
{
    int size;
    int front;
    int back;
    struct location position[MAX_X_RANGE * MAX_Y_RANGE];
};

bool is_full(struct deque *dq)
{
    if ((dq->front == 0 && dq->back == dq->size - 1) || dq->back + 1 == dq->front)
    {
        return true;
    }
    return false;
}

bool is_empty(struct deque *dq)
{
    if (dq->front == dq->back)
    {
        return true;
    }
    return false;
}

void addfirst(struct deque *dq, struct location cord)
{
    if (is_full(dq))
    {
        return;
    }
    if (dq->front == 0)
    {
        dq->front = dq->size - 1;
    }
    else
    {
        dq->front--;
    }
    dq->position[dq->front] = cord;
}

void addlast(struct deque *dq, struct location cord)
{
    if (is_full(dq))
    {
        return;
    }
    dq->position[dq->back] = cord;
    if (dq->back == dq->size - 1)
    {
        dq->back = 0;
    }
    else
    {
        dq->back++;
    }
}

void removefirst(struct deque *dq)
{
    if (is_empty(dq))
    {
        return;
    }

    if (dq->front == dq->size - 1)
    {
        dq->front = 0;
    }
    else
    {
        dq->front++;
    }
}

void removelast(struct deque *dq)
{
    if (is_empty(dq))
    {
        return;
    }

    if (dq->back == 0)
    {
        dq->back = dq->size - 1;
    }
    else
    {
        dq->back--;
    }
}

bool in_bounds(struct location cord_curr, int x_range, int y_range)
{
    if (cord_curr.x_cord >= 0 && cord_curr.y_cord >= 0 && cord_curr.x_cord < x_range && cord_curr.y_cord < y_range)
    {
        return true;
    }
    return false;
}

bool check_wall(char maze[MAX_Y_RANGE][MAX_X_RANGE], struct location cord_curr)
{
    if (maze[cord_curr.y_cord][cord_curr.x_cord] == '#')
    {
        return true;
    }
    return false;
}

bool maze_solved(struct location cord_curr, struct location cord_end)
{
    if (cord_curr.x_cord == cord_end.x_cord && cord_curr.y_cord == cord_end.y_cord)
    {
        return true;
    }
    return false;
}

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

int distance(struct location point1, struct location point2)
{
    int delta_x = abs(point2.x_cord - point1.x_cord);
    int delta_y = abs(point2.y_cord - point1.y_cord);
    return delta_x + delta_y;
}

void initialize(struct deque *dq)
{
    dq->size = MAX_X_RANGE * MAX_Y_RANGE;
    dq->front = dq->back = 0;
}

void mark_path(int y_range,int x_range,char maze[MAX_Y_RANGE][MAX_X_RANGE], struct location path[], int path_length, struct location cord_start, struct location cord_end)
{
    for (int i = 0; i < path_length; i++)
    {
        if ((path[i].x_cord != cord_start.x_cord || path[i].y_cord != cord_start.y_cord) &&
            (path[i].x_cord != cord_end.x_cord || path[i].y_cord != cord_end.y_cord))
        {
            maze[path[i].y_cord][path[i].x_cord] = '.';
        }
    }

    for (int i = 0; i < y_range; i++)
    {
        for (int j = 0; j < x_range+1; j++)
        {
            printf("%c", maze[i][j]);
        }
    }
}

void maze_solver( int y_range,int x_range, char maze[y_range][x_range+1], struct location *cord_start, struct location *cord_end)
{   
    struct deque dq;
    initialize(&dq);
    struct location current;
    struct location neighbour;
    struct location path[MAX_Y_RANGE * MAX_X_RANGE];
    int path_length = 0;
    find_cords( x_range, y_range,cord_start, cord_end, maze);
    current.x_cord = cord_start->x_cord;
    current.y_cord = cord_start->y_cord;
    addfirst(&dq, current);
    bool visited[y_range][x_range+1];
    
    for (int i = 0; i < y_range; i++)
    {
        for (int j = 0; j < x_range+1; j++)
        {
          visited[i][j]=false;
        }
    }
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    while (dq.front != dq.back)
    {
        current = dq.position[dq.front];
        removefirst(&dq);
        path[path_length].x_cord = current.x_cord;
        path[path_length].y_cord = current.y_cord;
        path_length++;

        if (maze_solved(current, *cord_end))
        {
            printf("Maze Solved\n");
            mark_path(y_range,x_range,maze, path, path_length, *cord_start, *cord_end);
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            neighbour.x_cord = current.x_cord + directions[i][0];
            neighbour.y_cord = current.y_cord + directions[i][1];

            if (in_bounds(neighbour, x_range, y_range) && !visited[neighbour.y_cord][neighbour.x_cord] && !check_wall(maze, neighbour))
            {
                visited[neighbour.y_cord][neighbour.x_cord] = true;
                
                if (distance(current, *cord_end) >= distance(neighbour, *cord_end))
                {
                    addfirst(&dq, neighbour);
                }
                else
                {
                    addlast(&dq, neighbour);
                }
            }
        }
    }

    printf("No Path Found\n");
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
    printf("%d\n%d\n",x_range,y_range);
    fseek(fptr,0,SEEK_SET);

    for (int i = 0; i < y_range; i++)
    {
        for (int j = 0; j < x_range+1; j++)
        {
            printf("%c", maze[i][j]);
        }
        //printf("\n");
    }

    maze_solver(y_range, x_range,maze, &cord_start, &cord_end );
    
    fclose(fptr);
    return 0;
}
