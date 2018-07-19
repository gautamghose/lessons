#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef enum {
    right = 0,
    down = 1,
    left = 2,
    up = 3,
    max_dir = 4,
} direction_e;

int **malloc_2D_array(int nr_rows, int nr_columns)
{
    int ** arr = (int **)malloc(sizeof(int*)*nr_rows);
    for (int i=0; i<nr_rows; ++i) {
        arr[i] = (int*)malloc(sizeof(int)*nr_columns);
    }
    return arr;
}

void print_2D_array(int **arr, int nr_rows, int nr_columns)
{
    int max_nr = nr_rows*nr_columns;
    int nr_digits = 1;
    if (max_nr > 999) nr_digits = 4;
    else if (max_nr > 99) nr_digits = 3;
    else if (max_nr > 9) nr_digits = 2;
    for (int i=0; i<nr_rows; ++i) {
        for (int j=0; j<nr_columns; ++j) {
            char buf[64];
            if (nr_digits == 1) sprintf(buf,"%d",arr[j][i]);
            else if (nr_digits == 2) sprintf(buf, "%2d", arr[j][i]);
            else if (nr_digits == 3) sprintf(buf, "%3d", arr[j][i]);
            else sprintf(buf, "%5d", arr[j][i]);
            std::cout << buf << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool get_next_cell(int **sq_matrix, int n, int& x, int& y, direction_e& dir)
{
    int next_x = x, next_y = y;
    int **arr = sq_matrix;

    switch(dir) {
        case direction_e::right:
            next_x = x+1;
            if (next_x >= n || arr[next_x][y]!=0) {
                dir = (direction_e) ((dir+1) % direction_e::max_dir);
                goto try_again;
            }
            break;
        case direction_e::left:
            next_x = x-1;
            if (next_x < 0 || arr[next_x][y]!=0) {
                dir = (direction_e) ((dir+1) % direction_e::max_dir);
                goto try_again;
            }
            break;
        case direction_e::down:
            next_y = y+1;
            if (next_y >= n || arr[x][next_y]!=0) {
                dir = (direction_e) ((dir+1) % direction_e::max_dir);
                goto try_again;
            }
            break;
        case direction_e::up:
            next_y = y-1;
            if (next_y < 0 || arr[x][next_y]!=0) {
                dir = (direction_e) ((dir+1) % direction_e::max_dir);
                goto try_again;
            }
            break;
        default:
            assert(false);
    }
    x = next_x;
    y = next_y;
    return true;

try_again:
    next_x = x; next_y = y;
    switch(dir) {
        case direction_e::right:
            next_x = x+1;
            if (next_x >= n || arr[next_x][y]!=0) {
                return false;
            }
            break;
        case direction_e::left:
            next_x = x-1;
            if (next_x < 0 || arr[next_x][y]!=0) {
                return false;
            }
            break;
        case direction_e::down:
            next_y = y+1;
            if (next_y >= n || arr[x][next_y]!=0) {
                return false;
            }
            break;
        case direction_e::up:
            next_y = y-1;
            if (next_y < 0 || arr[x][next_y]!=0) {
                return false;
            }
            break;
        default:
            assert(false);
    }
    x = next_x;
    y = next_y;
    return true;
}

void spiral(int n)
{
    int ** arr = malloc_2D_array(n,n);
    for (int i=0; i<n; ++i)
        for (int j=0; j<n; ++j)
            arr[i][j] = 0;
    print_2D_array(arr,n,n);//return;

    direction_e direction = direction_e::right;
    int curr_x = 0, curr_y = 0;
    int curr_cnt = 1;

    arr[curr_x][curr_y] = curr_cnt ++;
    std::cout << "(" << curr_x << "," << curr_y << ")" << std::endl;
    while (get_next_cell(arr,n,curr_x,curr_y,direction)) {
        std::cout << "(" << curr_x << "," << curr_y << ")" << std::endl;
        arr[curr_x][curr_y] = curr_cnt ++;
    }
    assert(curr_cnt == (n*n)+1);
    print_2D_array(arr,n,n);
    return;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        cout << "Usage: <prog> <square-matrix-order>\n";
        cout << "Example: " << argv[0] << "4" << std::endl;
        return 0;
    }
    spiral(atoi(argv[1]));
    return 0;
}

