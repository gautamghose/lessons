#include <stdio.h>
#include <assert.h>
#include <vector>

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

int RECURSIVE_compute_num_ways_to_complete(int goal, int starting_at)
{
    if (starting_at >= goal) {
        return 0;
    }
    int n = 0;
    switch (goal - starting_at) {
    case 1: return 1;
    case 2: return 2; //(1,1) and (2)
    case 3: return 4; //(1,1,1) (1,2) (2,1) (3)
    default:
        //take '1' as the first step; then compute the number of ways the rest of journey
        int n1 = RECURSIVE_compute_num_ways_to_complete(goal, starting_at+1);
        //take '2' as the first step; then compute the number of ways the rest of journey
        int n2 = RECURSIVE_compute_num_ways_to_complete(goal, starting_at+2);
        //take '3' as the first step; then compute the number of ways the rest of journey
        int n3 = RECURSIVE_compute_num_ways_to_complete(goal, starting_at+3);
        n = n1+n2+n3;
    }

    return n;
}

int RECURSIVE_MEMOIZE_compute_num_ways_to_complete(int goal, int starting_at, std::vector<int>& memos)
{
    if (starting_at >= goal) {
        return 0;
    }
    assert((int)memos.size() > goal);
    if (memos[starting_at] != (-1)) {
        //already computed
        return memos[starting_at];
    }
    int n = 0;
    switch (goal - starting_at) {
    case 1: return 1;
    case 2: return 2; //(1,1) and (2)
    case 3: return 4; //(1,1,1) (1,2) (2,1) (3)
    default:
        //take '1' as the first step; then compute the number of ways the rest of journey
        int n1 = RECURSIVE_MEMOIZE_compute_num_ways_to_complete(goal, starting_at+1, memos);
        //take '2' as the first step; then compute the number of ways the rest of journey
        int n2 = RECURSIVE_MEMOIZE_compute_num_ways_to_complete(goal, starting_at+2, memos);
        //take '3' as the first step; then compute the number of ways the rest of journey
        int n3 = RECURSIVE_MEMOIZE_compute_num_ways_to_complete(goal, starting_at+3, memos);
        n = n1+n2+n3;
        memos[starting_at] = n;
    }
    return memos[starting_at];
}

int compute_num_ways_to_complete(int goal, int starting_at)
{
#if 0
    std::vector<int> memos;
    int cnt = 0;
    while (cnt <= goal) {
        memos.push_back(-1);
        cnt++;
    }
    return RECURSIVE_MEMOIZE_compute_num_ways_to_complete(goal, starting_at, memos);
#else
    return RECURSIVE_compute_num_ways_to_complete(goal, starting_at);
#endif
}

int main(int argc, char** argv)
{
    int input = 0.0;
    if (argc < 2) {
        printf("Give a number between [1,10]: ");
        assert(scanf("%d", &input) == 1);
    } else {
        assert(sscanf(argv[1],"%d", &input) == 1);
    }
    printf("Number of ways to reach goal %d is %d\n",
                input, compute_num_ways_to_complete(input, 0));
    return 0;    
}


