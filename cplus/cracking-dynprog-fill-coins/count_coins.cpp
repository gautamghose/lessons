#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>

#if 0
//Just testing logical versus arithmetic bit shifting
int main()
{
    int val = -643;
    int val_logical_shift = (unsigned)val >> 1;
    int val_arith_shift = val >> 1;
    printf ("Orig %d, val_logical_shift %d, val_arith_shift %d\n", val, val_logical_shift, val_arith_shift);
    return 0;
}
#endif

int num_ways_to_fill_with_1s(int goal, std::vector<std::string>& combos)
{
    combos.clear(); 
    if (goal > 0) {
        combos.push_back(std::string()); //empty string
        while (goal > 0) {
            combos[0].append("1,");
            goal --;
        }
    }
    return combos.size();
}

int num_ways_to_fill_with_5_1s(int goal, std::vector<std::string>& combos)
{
    int nr = 0;
    std::string prefix_str;
    combos.clear(); 
    while (goal > 0) {
        std::vector<std::string> inner_combos;
        nr += num_ways_to_fill_with_1s(goal, inner_combos);
        std::vector<std::string>::const_iterator it = inner_combos.begin();
        while (it != inner_combos.end()) {
            combos.push_back(prefix_str);
            combos.back().append(*it);
            it++;
        }
        goal -= 5;
        prefix_str.append("5,");
    }
    if (goal == 0) {
        combos.push_back(prefix_str);
        nr += 1; //1 way to reach with all 5's
    }
    assert((int)combos.size() == nr);
    return nr;
}

int num_ways_to_fill_with_10_5_1s(int goal, std::vector<std::string>& combos)
{
    int nr = 0;
    std::string prefix_str;
    combos.clear(); 
    while (goal > 0) {
        std::vector<std::string> inner_combos;
        nr += num_ways_to_fill_with_5_1s(goal, inner_combos);
        std::vector<std::string>::const_iterator it = inner_combos.begin();
        while (it != inner_combos.end()) {
            combos.push_back(prefix_str);
            combos.back().append(*it);
            it++;
        }
        goal -= 10;
        prefix_str.append("10,");
    }
    if (goal == 0) {
        combos.push_back(prefix_str);
        nr += 1; //1 way to reach with all 10's
    }
    assert((int)combos.size() == nr);
    return nr;
}

int num_ways_to_fill_with_25_10_5_1s(int goal, std::vector<std::string>& combos)
{
    int nr = 0;
    std::string prefix_str;
    combos.clear(); 
    while (goal > 0) {
        std::vector<std::string> inner_combos;
        nr += num_ways_to_fill_with_10_5_1s(goal, inner_combos);
        std::vector<std::string>::const_iterator it = inner_combos.begin();
        while (it != inner_combos.end()) {
            combos.push_back(prefix_str);
            combos.back().append(*it);
            it++;
        }
        goal -= 25;
        prefix_str.append("25,");
    }
    if (goal == 0) {
        combos.push_back(prefix_str);
        nr += 1; //1 way to reach with all 25's
    }
    assert((int)combos.size() == nr);
    return nr;
}

#if 0
int num_ways_to_fill_with_10_5_1s(int goal)
{
    int nr = 0;
    while (goal > 0) {
        nr += num_ways_to_fill_with_5_1s(goal);
        goal -= 10;
    }
    if (goal == 0) nr += 1; //1 way to reach with all 10's
    return nr;
}

int num_ways_to_fill_with_25_10_5_1s(int goal)
{
    int nr = 0;
    while (goal > 0) {
        nr += num_ways_to_fill_with_10_5_1s(goal);
        goal -= 25;
    }
    if (goal == 0) nr += 1; //1 way to reach with all 25's
    return nr;
}
#endif

int compute_num_ways_to_fill(int goal)
{
    std::vector<std::string> all_combos;
    int nr = num_ways_to_fill_with_25_10_5_1s(goal, all_combos);
    //return num_ways_to_fill_with_25_10_5_1s(goal);
    while (all_combos.size() > 0) {
        std::cout << all_combos.back() << std::endl;
        all_combos.pop_back();
    }
    return nr;
}

int main(int argc, char** argv)
{
    int input = 0;
    if (argc < 2) {
        printf("Give a number between [1,100]. Will assume that is the total value to build using unlimited 1,5,10 and 25cent coins: ");
        assert(scanf("%d", &input) == 1);
    } else {
        assert(sscanf(argv[1],"%d", &input) == 1);
    }
    printf("Number of ways to reach goal %d is %d\n",
                input, compute_num_ways_to_fill(input));
    return 0;    
}


