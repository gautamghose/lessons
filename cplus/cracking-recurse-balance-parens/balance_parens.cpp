#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>

#if 0
//This routine finds any one magic index in the array 'arr', and this scans
//only the portion of the array from 'start' to 'end' both inclusive.
ssize_t find_a_magic_index(const std::vector<int>& arr, size_t start, size_t end)
{
    assert(start>=0);
    assert(end<arr.size());
    if (start > end) {
        return (-1);
    } else if (start == end) {
        if (arr[start]==start) return start;
        else return (-1);
    }
    assert(start < end);
    size_t midpoint = (start+end)/2;
    if (arr[midpoint] > midpoint) {
        //we cannot have a magic at higher than midpoint
        return find_a_magic_index(arr, start, midpoint);
    } else if (arr[midpoint] < midpoint) {
        //we cannot have a magic at lower than midpoint
        return find_a_magic_index(arr, midpoint, end);
    } else {
        return midpoint;
    }
}
#endif

static int N = 0;

void create_parens_sequence(const std::string& sequence_so_far, int remaining_opens, int remaining_closes, int curr_balance)
{
    if ((remaining_opens == 0) && (remaining_closes == 0)) {
        //At the bottom of the recursion tree
        if (curr_balance == N) {
            //Found a valid sequence
            std::cout << sequence_so_far << std::endl;
        }
        return;
    }

    if (remaining_opens > 0 && curr_balance>0) {
        std::string new_sequence(sequence_so_far);
        new_sequence.append("(");
        create_parens_sequence(new_sequence, remaining_opens-1, remaining_closes, curr_balance-1);
    }
        
    if (remaining_closes > 0 && curr_balance<N) {
        std::string new_sequence(sequence_so_far);
        new_sequence.append(")");
        create_parens_sequence(new_sequence, remaining_opens, remaining_closes-1, curr_balance+1);
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Give a number between [1,10] be be used as the number of open-close parentheses: ");
        assert(scanf("%d", &N) == 1);
    } else {
        assert(sscanf(argv[1],"%d", &N) == 1);
    }
    printf("Number of valid sequences:\n");
    create_parens_sequence(std::string(), N, N, N);
    return 0;    
}


