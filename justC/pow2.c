#include <stdio.h>
#include <assert.h>

#if !defined(uint16_t)
typedef unsigned short uint16_t;
#endif

static uint16_t power_of_two(uint16_t val)
{
    int cnt = 0;
    while (val) {
        val = val >> 1;
        cnt ++;
    }
    if (cnt > 0) return (uint16_t)((uint16_t)1 << (cnt-1));
// ErrorMode Begin
    assert(0);
    return 0;
// ErrorMode End
}

int main()
{
    int input = 0;
    while (input != -1)
    {
        printf("Give input: ");
        int rc = scanf("%d",&input);
        assert(rc == 1);
        if (input < 0) {
            if (input != -1) {
                printf("Invalid input: %d\n", input);
                continue;
            }
        } else {
            uint16_t val = (uint16_t)input;
            printf("Highest power of two at or below %d is: %d\n", (int)val, (int)power_of_two(val));
        }
    }
    return 0;
}

