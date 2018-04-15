#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <map>
#include <memory> //for unique_ptr


static int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
static int arr_length = sizeof(arr)/sizeof(arr[0]);
static int curr_best_answer = -1000000;
static int curr_best_ending_at = -1000000;

static inline int _max_(int a, int b)
{
    return (a>b ? a:b);
}

//msp = maximum subarray problem
/**
 Let A be the array, with A[0]..A[n-1] being the n elements
 Let B[i-1] be the solution to the msp at element-ending-at index (i-1), i.e. ending at A[i-1].
 Then, B[i] = max(A[i], B[i-1]+A[i]).
 That's it!! :)
 And once you find the 'i' and B[i], you can start summing all the numbers A[i]+A[i-1]+... one
 at a time till you reach a j, such that, A[i]+A[i-1]+...+A[j] = B[i]. Thereby, you have the
 maximum sub-array at indices [j,i] located.
**/
int recurse_find_msp_ending_at(int idx)
{
    assert(idx >=0 && idx < arr_length);
    if (idx == 0) {
        if (arr[0] > curr_best_answer) {
            curr_best_answer = arr[0];
            curr_best_ending_at = 0;
        }
        return arr[0];
    }
    int Bi_minus_1 = recurse_find_msp_ending_at(idx-1);
    //std::cout << "find_msp_ending_at " << (idx-1) << ": " << Bi_minus_1 << std::endl;
    int Bi = _max_(arr[idx], Bi_minus_1 + arr[idx]);
    //std::cout << "find_msp_ending_at " << (idx) << ": " << Bi << std::endl;
    if (Bi > curr_best_answer) {
        curr_best_answer = Bi;
        curr_best_ending_at = idx;
    }
    return Bi;
}

int iter_find_msp_ending_at(int idx)
{
    assert(idx >= 0);
    int Bi_minus_1 = curr_best_answer = arr[0];
    int Bi = Bi_minus_1;
    curr_best_ending_at = 0;
    for (int i=1; i<=idx; i++) {
        Bi = _max_(arr[i], Bi_minus_1 + arr[i]);
    std::cout << "find_msp_ending_at " << (i) << ": " << Bi << std::endl;
        if (Bi > curr_best_answer) {
            curr_best_answer = Bi;
            curr_best_ending_at = i;
        } 
        Bi_minus_1 = Bi;
    } 
    return Bi;
}

int main(int argc, char** argv)
{
    std::cout << "recurse_find_msp_ending_at " << (arr_length-1) << ": " << recurse_find_msp_ending_at(arr_length-1) << std::endl;
    std::cout << "Best_answer ending at " << curr_best_ending_at << ": " << curr_best_answer << std::endl;
    std::cout << "iter_find_msp_ending_at " << (arr_length-1) << ": " << iter_find_msp_ending_at(arr_length-1) << std::endl;
    std::cout << "Best_answer ending at " << curr_best_ending_at << ": " << curr_best_answer << std::endl;
    return 0;
}


