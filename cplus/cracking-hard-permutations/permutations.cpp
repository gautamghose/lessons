#include <assert.h>
#include <vector>
#include <iostream>


/// One permutation is captured in a vector
typedef std::vector<int> permutation_t;

/// A quick factorial() implementation.
size_t factorial(size_t n)
{
    if (n==1) return 1;
    return n*factorial(n-1);
}

/**
 * this helper routine inserts an element (chosen) into a vector and shifts the remaining elements accordingly.
 * If
 *   insert_at = 1
 *   chosen = 0
 *   partial = [23]
 * Then the output
 *   result_vec = [203] i.e. '0' is inserted at index 1, and everything to the right of the index 1 are shifted out.
 */
void insert_into_vector(size_t insert_at, int chosen, const permutation_t& partial, permutation_t& result_vec)
{
    assert(insert_at <= partial.size());
    result_vec.clear();
    auto it = partial.begin();
    size_t runner = 0;
    while (it != partial.end()) {
        if (runner == insert_at) {
            result_vec.push_back(chosen);
        }
        result_vec.push_back((*it));
        runner++;
        it++;
    }
    if (runner == insert_at) result_vec.push_back(chosen);
    assert(result_vec.size() == partial.size()+1);
}

/*
 * It takes one permutation vector, then places the 'chosen' value at all the places
 * inside the vector, and also at the start and end of the permutation vector.
 * If 
 *   chosen = 0
 *   partial = [23]
 * Then output
 *   permutations = [ [023], [203], [230] ]
 */
void add_new_permutations(int chosen, permutation_t& partial, std::vector<permutation_t>& permutations)
{
    for (size_t insert_at=0; insert_at<=partial.size(); insert_at++) {
        permutation_t local;
        insert_into_vector(insert_at, chosen, partial, local);
        permutations.push_back(local);
    }
}

/** 
 * if N=4, this routine computes the permutations of the numbers [0,1,2,3]
 * and returns all the permutations in the vector of permutations.
 * e.g. if N=3, the output vector will have the following six (N!) permutations:
 * If
 *   starting = [012]
 * Then output
 *   permutations = [ [012], [021], [102], [120], [201], [210] ]
 */
void generate_permutations(const std::vector<int>& starting, std::vector<permutation_t>& permutations)
{
    if (starting.size() == 1) {
        permutation_t only_one;
        only_one.push_back(starting[0]);
        permutations.push_back(only_one);
        return;
    }
    assert(starting.size() > 1);

    std::vector<int> shorter_set;
    int chosen = starting[0];
    for (size_t i=1; i<starting.size(); i++) {
        shorter_set.push_back(starting[i]);
    }
    assert(shorter_set.size() == starting.size()-1);
    std::vector<permutation_t> local_permutations;
    generate_permutations(shorter_set, local_permutations);
    assert(factorial(shorter_set.size()) == local_permutations.size());

    //Now assemble the new permutations
    auto it = local_permutations.begin();
    while (it != local_permutations.end()) {
        permutation_t& local = (*it);
        add_new_permutations(chosen, local, permutations);
        it++;
    }
}

/// Prints one permutation
void print_permutation(const permutation_t& one)
{
    for (auto it=one.begin(); it!=one.end(); it++) {
        std::cout << (*it);
    }
    std::cout << std::endl;
}

/// Top level routine to call. It formats the input suitable for the underlying recursive call.
void permute(int N, std::vector<permutation_t>& permutations)
{
    permutations.clear();

    std::vector<int> starting_set;
    for (int i=0; i<N; i++) {
        starting_set.push_back(i);
    }
    
    generate_permutations(starting_set, permutations);
}

int main(int argc, char** argv)
{
    std::vector<permutation_t> permutations;
    int N = 3;
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    permute(N, permutations);
    assert(factorial(N) == permutations.size());
    auto it = permutations.begin();
    while (it != permutations.end()) {

#ifdef LAMBDA_NO_VARIABLE_CAPTURE
//without variable capture
#pragma message "using lambda-function without variable capture"
        [] (const permutation_t& one) {
            for (auto i=one.begin(); i!=one.end(); i++) {
                std::cout << (*i) << " ";
            }
            std::cout << std::endl;
            return;
        } (*it);
#elif LAMBDA_VARIABLE_CAPTURE
//with variable capture
#pragma message "using lambda-function with variable capture"
        [&] () {
            for (auto i=(*it).begin(); i!=(*it).end(); i++) {
                std::cout << (*i) << ",";
            }
            std::cout << std::endl;
            return;
        } ();
#else
#pragma message "using no lambda-function"
        print_permutation((*it));
#endif

        it ++;
    }
    return 0;
}


