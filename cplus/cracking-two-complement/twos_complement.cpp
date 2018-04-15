#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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

std::string int_to_binary(unsigned int num, bool prepend_zeros = false)
{
    std::string result;
    if (num == 0) result.append("0");
    while (num > 0) {
        if (num & 0x01) result.append("1");
        else result.append("0");
        num >>= 1;
    }
    if (prepend_zeros) {
        size_t nr_pads = 8*sizeof(unsigned int) - result.size(); //8 bits in 1 byte
        while (nr_pads > 0) {
            result.append("0");
            nr_pads --;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

/* 
If we want the binary representation of say, (-10), then pass (10) to this function.
This function will then give the binary rep of (-10) as in 2's complement.
*/
std::string negative_int_to_binary(int num)
{
    /*
    2's complement of a negative number is as follows:-
    1. Take the positive value of the number and put it out in binary.
    2. Flip each of the 32 bits. This will make sure, the highest bit is set to 1.
    3. Add 1 to the result.
    */
    unsigned int answer = (unsigned int)num;
    answer = ~answer; //bitwise not 
    answer += 0x1;
    return int_to_binary(answer, true);
}

int main(int argc, char** argv)
{
    int input = 0;
    if (argc < 2) {
        printf("Give a number between [1,100]. : ");
        assert(scanf("%d", &input) == 1);
    } else {
        assert(sscanf(argv[1],"%d", &input) == 1);
    }
    printf("Binary repr of %d is %s\n",
                input, int_to_binary((unsigned int)input, true).c_str());
    printf("Binary repr of -%d is %s\n",
                input, negative_int_to_binary(input).c_str());
    printf("Binary repr of -%d via computer built-in is %s\n",
                input, int_to_binary((0-input), true).c_str());

    for (int i=0; i<10000; i++) {
        assert(negative_int_to_binary(input) == int_to_binary((0-input), true));
    }
    return 0;    
}


