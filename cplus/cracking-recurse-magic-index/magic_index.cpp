#include <stdio.h>
#include <assert.h>
#include <vector>

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


