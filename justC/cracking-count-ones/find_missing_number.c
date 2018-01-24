#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#if !defined(uint16_t)
typedef unsigned short uint16_t;
#endif


unsigned two_raised_to(unsigned exp)
{
    return ((unsigned)1 << exp);
}

unsigned find_highest_bit(unsigned number)
{
    size_t nr_bytes = sizeof(number);
    size_t nr_bits = nr_bytes * 8;
    unsigned mask = ((unsigned)1 << (nr_bits-1));
    unsigned highest_bit = nr_bits-1;
    while (highest_bit > 0) {
        if ((mask & number) == mask) {
            return highest_bit;
        }
        mask >>= 1;
        highest_bit --;
    }
    return highest_bit;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage: %s <a-positive-number-N>\n", argv[0]);
        printf("Imagine the numbers [0..N-1] written out in sequence one below another forming a matrix.\n");
        printf("Each number is written in its binary representation.\n");
        printf("Thus, if N is 5, the numbers are written as:-\n");
        printf("000\n001\n010\n011\n100\n");
        printf("This routine prints the number of 1's in each of the three columns, \n"
                "with column number 0 representing the least significant bit.\n");
        printf("So, the answer will be for N=5, (1,2,2) indicating that at column number 2, 1 and 0, there\n"
                "are 1, 2 and 2 one's respectively\n");
        return 1;
    }
 
    unsigned int n = atoi(argv[1]);

    unsigned int highest_bit = find_highest_bit(n-1);
    printf ("Highest bit for N=%u is %u\n", n, highest_bit);
    unsigned i = highest_bit;
    while (1) {
        unsigned Ni = n / two_raised_to(i+1);
        unsigned Ri = n % two_raised_to(i+1);
        unsigned nr_ones = (Ni * two_raised_to(i)) + (Ri > two_raised_to(i) ? (Ri - two_raised_to(i)) : 0);
        printf("Column %u: %u\n", i, nr_ones);
        if (i==0) break;
        i --;
    }
    return 0;
}

