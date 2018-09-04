#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <sstream>
#include <unordered_map>

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

std::unordered_map<std::string, int> lookup;
void preprocess()
{
    lookup["11"] = 1;
    lookup["12"] = 1;
    lookup["13"] = 1;
    lookup["14"] = 2;
    lookup["15"] = 2;
    lookup["21"] = 2;
    lookup["22"] = 3;
    lookup["23"] = 3;
    lookup["24"] = 3;
    lookup["25"] = 4;
    lookup["31"] = 4;
    lookup["32"] = 4;
    lookup["33"] = 5;
    lookup["34"] = 5;
    lookup["35"] = 5;
    lookup["41"] = 6;
    lookup["42"] = 6;
    lookup["43"] = 6;
    lookup["44"] = 7;
    lookup["45"] = 7;
    lookup["51"] = 7;

    lookup["52"] = 0;
    lookup["53"] = 0;
    lookup["54"] = 0;
    lookup["55"] = 0;
}

int rand5()
{
    long int r = random() % 5;
    return (r+1);
}

int rand7()
{
    std::stringstream ss;
    while (true) {
        ss << rand5() << rand5();
        int rc = lookup[ss.str()];
        ss.str("");
        if (rc == 0)  {
            //std::cout << "generated 0\n";
            continue;
        }
        return rc;
    }
    assert(false);
    return 0;
}

        

int main(int argc, char** argv)
{
std::cout << sizeof(long long) << std::endl;
    preprocess();
    int n = atoi(argv[1]);
    while (n>0) {
        std::cout << rand7() << std::endl;
        --n;
    }
    return 0;
}


