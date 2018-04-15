#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <sstream>

int** my2Dalloc(int m, int n)
{
    int** columns = (int**)malloc((1+m)*sizeof(int*));
    columns[0] = (int*)malloc(sizeof(int));
    *(columns[0]) = m;
    for (int i=1; i<=m; i++) {
        columns[i] = (int*)malloc(n*sizeof(int)); 
    }
    return &columns[1];
}

void my2Dfree(int** arr)
{
    int m = *(arr[-1]);
std::cout << "Free-ing " << m << " arrays\n";
    free(arr[-1]);
    for (int i=0; i<m; i++) {
        free(arr[i]);
    }
    free(&arr[-1]);
}

#define M 4
#define N 5
enum {
    e_unexplored = 7,
    e_exploring = 8,
    e_explored = 9
};

static int ** arr = NULL;
static int ** reference = NULL;

void explore_pond(int cell_m, int cell_n, std::vector<std::pair<int,int>>& pond_cells)
{
    if (cell_m < 0 || cell_m >= M) return; //illegal location
    if (cell_n < 0 || cell_n >= N) return; //illegal location
    if (reference[cell_m][cell_n] == e_unexplored) {
        if (arr[cell_m][cell_n] == 0) {
            arr[cell_m][cell_n] = e_exploring;
            //8 neighbors are there
            explore_pond(cell_m-1,cell_n-1, pond_cells);
            explore_pond(cell_m-1,cell_n-0, pond_cells);
            explore_pond(cell_m-1,cell_n+1, pond_cells);
            explore_pond(cell_m-0,cell_n-1, pond_cells);
            //explore_pond(cell_m-0,cell_n-0, pond_cells);
            explore_pond(cell_m-0,cell_n+1, pond_cells);
            explore_pond(cell_m+1,cell_n-1, pond_cells);
            explore_pond(cell_m+1,cell_n-0, pond_cells);
            explore_pond(cell_m+1,cell_n+1, pond_cells);

            pond_cells.push_back(std::pair<int,int>(cell_m,cell_n));
            reference[cell_m][cell_n] = e_explored;
        } else {
            reference[cell_m][cell_n] = e_explored;
        }
    }
    return;
}

void explore(int cell_m, int cell_n)
{
    if (reference[cell_m][cell_n] == e_unexplored) {
        if (arr[cell_m][cell_n] == 0) {
            std::vector<std::pair<int,int>> pond_cells;
            explore_pond(cell_m, cell_n, pond_cells);
            assert(pond_cells.size() > 0);
            std::cout << "Pond with size " << pond_cells.size() << " found." << std::endl;
            auto it = pond_cells.begin();
            while (it != pond_cells.end()) {
                std::cout << "<" << (*it).first << "," << (*it).second << ">" << " ";
                it ++;
            }
            std::cout << std::endl;
        }
        reference[cell_m][cell_n] = e_explored;
    } else
        {}//dont do anything
}

int main(int argc, char** argv)
{
    //int ** arr = (int**)malloc(40);//40 bytes
    //int M=4, N=5;

    arr = my2Dalloc(M,N);
    reference = my2Dalloc(M,N);

    const char* cinput[N] = {"0|2|0|1", "1|2|0|1", "1|0|0|1", "2|1|1|0", "0|0|1|1"};
    std::istringstream input[N];
    std::vector<std::array<char, 2>> v;
    for (int i=0; i<N; i++)
        input[i].str(cinput[i]);

    // note: the following loop terminates when std::ios_base::operator bool()
    // on the stream returned from getline() returns false
    for (int i=0; i<N; i++) {
    int m=0;
    for (std::array<char, 2> a; input[i].getline(&a[0], 2, '|'); ) {
        arr[m][i] = std::stoi(&a[0]);
        reference[m][i] = e_unexplored; 
        m++;
        v.push_back(a);
    }
    std::cout << "input: " << i << std::endl;
    for (auto& a : v) {
        std::cout << &a[0] << '\n';
    }
    v.clear();
    }
 
    std::cout << "arr is " << arr << std::endl;
    for (int j=0; j<N; j++) {
      std::cout << std::endl;
      for (int i=0; i<M; i++) {
        std::cout << arr[i][j] << " ";
      }
    }

    std::cout << std::endl;

    std::cout << "reference is " << arr << std::endl;
    for (int j=0; j<N; j++) {
      std::cout << std::endl;
      for (int i=0; i<M; i++) {
        std::cout << reference[i][j] << " ";
      }
    }
    std::cout << std::endl;

    std::cout << "exploring now: " << std::endl; 
    for (int j=0; j<N; j++) {
      for (int i=0; i<M; i++) {
        explore(i,j);
      }
    }

    std::cout << "final reference is " << arr << std::endl;
    for (int j=0; j<N; j++) {
      std::cout << std::endl;
      for (int i=0; i<M; i++) {
        std::cout << reference[i][j] << " ";
      }
    }
    std::cout << std::endl;

/*
    printf("arr %p &arr[0] %p &arr[0][2] %p &arr[2][0] %p &arr[2][1] %p &arr[3][1] %p &arr[1] %p &arr[2] %p\n",
            arr, &arr[0], &arr[0][2], &arr[2][0], &arr[2][1], &arr[3][1], &arr[1], &arr[2]);
    
    for (int i=0; i<M; i++)
      for (int j=0; j<N; j++) {
        arr[i][j] = ((1+i)*10+j);
      }
*/

    
    my2Dfree(arr);
    my2Dfree(reference);
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <valid expression>" << std::endl;
        return 1;
    }
    return 0;
}


