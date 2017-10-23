#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int argc, char ** argv)
{
    if (argc != 2) {
        printf("Use x <integer>\n") ;
        return -1;
    }
    int the_num = atoi(argv[1]) ;

    int num_digits=0, runner=1;
    while (runner <= the_num) {num_digits++, runner *= 10 ;}
    //printf("Number of digits in %d is %d\n", the_num, num_digits) ;

    runner /= 10 ;
    int top_digit, remain=the_num, total=0;

    while (remain){
        top_digit = remain/runner ;
        remain %= runner ;
        num_digits -= 1;
        total += top_digit * (num_digits)*(runner)/10;
        switch (top_digit) {
            case 0:
            case 1:
                break;
            case 2:
                total += (remain+1);
                break;
            default:  
                total += runner ;
        }
        runner /= 10 ;
        //printf("top=%d remainder=%d total=%d\n", top_digit, remain, total);
    }
    printf("Answer: %d\n", total);
    return 0;
}
