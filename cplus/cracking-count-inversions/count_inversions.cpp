#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>

       long int random(void);

using namespace std;

int add_no_arithm(int a, int b) 
{
    if (b == 0) return a;
    int sum = a ^ b; // add without carrying 
    int carry = (a & b) << 1; // carry, but don’t add 
    return add_no_arithm(sum, carry); // recurse 
}

size_t myrand(size_t i)
{
    return 0;
}

void shuffle(int* arr, size_t n)
{
    for (size_t i=n-1; i>0; --i) {
        size_t index = myrand(i+1);
        //swap arr[i] and arr[index]
        int tmp = arr[i];
        arr[i] = arr[index];
        arr[index] = tmp;
    }
}

vector<int> randomly_pick_m_out_of_n(vector<int> orig, size_t n, size_t m)
{
    assert(orig.size() == n);
    assert(m <= n);
    vector<int> clone = orig;
    vector<int> result(m);
    for (size_t i=n-1, j=0; (i>0 && j<m); --i,++j) {
        size_t index = myrand(i+1);
        int tmp = clone[i];
        clone[i] = clone[index];
        clone[index] = tmp;

        result[j] = clone[i];
    }
    return result;
}

int inversions = 0;
void merge(vector<int>& arr, size_t start1, size_t end1, size_t start2, size_t end2, vector<int>& temp_arr)
{
    size_t idx1 = start1, idx2 = start2;
    size_t tmpidx = 0;
    while (idx1 <= end1 && idx2 <= end2) {
        if (arr[idx1] <= arr[idx2]) {
            temp_arr[tmpidx++] = arr[idx1];
            idx1++;
        } else {
            temp_arr[tmpidx++] = arr[idx2];
            idx2++;
            inversions += (end1-idx1+1); 
        }
    }
    while (idx1 <= end1) {
        temp_arr[tmpidx++] = arr[idx1++];
    }
    while (idx2 <= end2) {
        temp_arr[tmpidx++] = arr[idx2++];
    }
    assert(tmpidx == (end2-start1+1));
    for (size_t i=start1,j=0; j<tmpidx; ++j,++i) {
        arr[i] = temp_arr[j];
    }
}

void mergesort(vector<int>& arr, size_t start, size_t end, vector<int>& temp_arr)
{
    assert(start>=0);
    assert(end<arr.size());

    if (start >= end) {
        return;
    }
    size_t mid = (start+end)/2;
    mergesort(arr, start, mid, temp_arr);
    mergesort(arr, mid+1, end, temp_arr);
    merge(arr, start, mid, mid+1, end, temp_arr);
    return;
}

int main(int argc, char** argv)
{
#if 0
    if (argc < 2) {
        cout << "Provide a size of the array as the second arg.\n";
        return 1;
    }
    int nr_array;
    nr_array = atoi(argv[1]);
    int *arr = new int[nr_array];
    for (int i=0; i<nr_array; ++i) {
        arr[i] = (i+1);
    }
    cout << "orig array:\n";
    print_arr(arr, nr_array);
    //shuffle(arr, nr_array);
    cout << "shuffled array:\n";
    print_arr(arr, nr_array);

    vector<int> orig(arr, arr+nr_array);
    vector<int> result = randomly_pick_m_out_of_n(orig, nr_array, nr_array/2);
    cout << "randomly picked half of the number of elements in the original array:\n";
    for (int val: result) {
        cout << val << " ";
    }
    cout << endl;
#endif

    vector<int> arr;
    int n;
    cin >> n;
    while (n>0) {
        int q;
        cin >> q;
        arr.push_back(q);
        n--;
    }

    cout << "original" << endl;
    for (int i: arr) {
        //cout << i << endl;
    }
    vector<int> tmp(arr.size());
    mergesort(arr, 0, arr.size()-1, tmp);
    cout << "sorted" << endl;
    for (int i: arr) {
        //cout << i << endl;
    }

    cout << "Number of inversions: " << inversions << endl;
    return 0;
}

