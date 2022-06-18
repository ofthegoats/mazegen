#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void add_connection(int w, int h, int** matrix, int from_x, int from_y, int to_x, int to_y);
void prepare_grid(int w, int h, int** matrix);
void build_maze_dfs(int w, int h, int** matrix);
void output_maze(int w, int h, int** matrix);

int main(int argc, char* argv[])
{
    size_t w, h;
    if (argc >= 3) {
        w = atoi(argv[1]);
        h = atoi(argv[2]);
    } else {
        scanf("%zd %zd", &w, &h);
    }
    int** g;  // id by w * y + x
    g = (int**)calloc(w * h, sizeof(*g));
    for (size_t i = 0; i < w * h; i++)
        g[i] = calloc(w * h, sizeof(*g[i]));
    prepare_grid(w, h, g);
    build_maze_dfs(w, h, g);
    output_maze(w, h, g);
    return 0;
}

void add_connection(int w, int h, int** matrix, int from_x, int from_y, int to_x, int to_y)
{
    if (from_x >= 0 && from_x < w && from_y >= 0 && from_y < h && to_x >= 0 && to_x < w && to_y >= 0
        && to_y < h)
        matrix[w * from_y + from_x][w * to_y + to_x] = 1;
}

void prepare_grid(int w, int h, int** matrix)
{
    // 0 out the matrix
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            matrix[x][y] = 0;
    // add grid connections
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            add_connection(w, h, matrix, x, y, x, y + 1);
            add_connection(w, h, matrix, x, y, x + 1, y);
            add_connection(w, h, matrix, x, y, x, y - 1);
            add_connection(w, h, matrix, x, y, x - 1, y);
        }
    }
}

// https://en.wikipedia.org/wiki/Maze_generation_algorithm#Randomized_depth-first_search
void build_maze_dfs(int w, int h, int** matrix)
{
    srand(time(NULL));
    int* stack = (int*)calloc(w * h, sizeof(int));
    int* visited = (int*)calloc(w * h, sizeof(int));
    for (int i = 0; i < w * h; i++)
        visited[i] = 0;
    stack[0] = 0;  // 0,0
    visited[0] = 1;
    int top = 0;
    int current;
    while (top >= 0) {
        current = stack[top];
        top--;
        int no_connected = 0;
        for (int i = 0; i < w * h; i++)
            if (matrix[current][i] && !visited[i])
                no_connected++;
        if (no_connected != 0) {
            top++;
            stack[top] = current;
            int r = rand() % no_connected;
            int next = 0;
            for (int i = 0; i < w * h; i++) {
                if (matrix[current][i] && !visited[i] && r == 0) {
                    next = i;
                    break;
                } else if (matrix[current][i] && !visited[i]) {
                    r--;
                }
            }
            assert(next >= 0 && next < w * h && matrix[current][next] && !visited[next]);
            matrix[current][next] = 0;
            matrix[next][current] = 0;
            visited[next] = 1;
            top++;
            stack[top] = next;
        }
    }
}

void output_maze(int w, int h, int** matrix)
{
    /* For every row, print two rows: one containing connections to the right,
     * the second containing connections below
     */

    // top border
    printf("#");
    for (int x = 0; x < w; x++)
        printf("##");
    printf("\n");
    // print the top row
    printf("#");
    for (int x = 0; x < w - 1; x++) {
        printf(" ");
        if (matrix[w * (h - 1) + x][w * (h - 1) + (x + 1)])
            printf("#");
        else
            printf(" ");
    }
    printf("  ");  // exit hole
    printf("\n#");
    for (int x = 0; x < w; x++) {
        if (matrix[w * (h - 1) + x][w * (h - 2) + x])
            printf("#");
        else
            printf(" ");
        printf("#");
    }
    printf("\n");
    // print the middle rows
    for (int y = h - 2; y > 0; y--) {
        printf("#");
        for (int x = 0; x < w - 1; x++) {
            printf(" ");
            if (matrix[w * y + x][w * y + (x + 1)])
                printf("#");
            else
                printf(" ");
        }
        printf(" #");
        printf("\n#");
        for (int x = 0; x < w; x++) {
            if (matrix[w * y + x][w * (y - 1) + x])
                printf("#");
            else
                printf(" ");
            printf("#");
        }
        printf("\n");
    }
    // print the bottom row
    printf(" ");  // entry hole
    for (int x = 0; x < w - 1; x++) {
        printf(" ");
        if (matrix[x][x + 1])
            printf("#");
        else
            printf(" ");
    }
    printf(" #");
    printf("\n#");
    for (int x = 0; x < w; x++)
        printf("##");
    printf("\n");
}
