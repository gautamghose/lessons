#include <stdio.h>
#include <assert.h>

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

const char* get_binary_rep(double input)
{
    static char binary_bits[64];
    int i=0;
    for (i=0; i<64; i++) binary_bits[i] = '0';
    binary_bits[32]='\0';
    int bit_position = 0; //starting from the left of the string representation
    while (bit_position < 32 && input > 0.0) {
        if (input >= 0.5) {
            binary_bits[bit_position] = '1';
            input -= 0.5;
        } else {
            binary_bits[bit_position] = '0';
        }
        input *= 2.0;
        bit_position ++;
    }
    if (input == 0.0) return &binary_bits[0];
    return "ERROR";
}

int main(int argc, char** argv)
{
    double input = 0.0;
    printf("Give a number between [0.0,1.0): ");
    assert(scanf("%lf", &input) == 1);
    printf("Binary representation of the number %lf is: %s\n", input, get_binary_rep(input));
    return 0;    
}


