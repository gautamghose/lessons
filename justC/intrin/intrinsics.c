#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <stdint.h>
#include <immintrin.h>
#include <x86intrin.h>


#if !defined(uint16_t)
typedef unsigned short uint16_t;
#endif

#define _Bit_scan_forward(x) __builtin_ctz((unsigned int)(x))
#define _Bit_scan_reverse(x) (31 - __builtin_clz((unsigned int)(x)))
#define _Bextr_u64(X,Y,Z) __builtin_ia32_bextr_u64 (X, ((Y & 0xff) | ((Z & 0xff) << 8)))

static inline uint32_t compare32bytes(const void *b1, const void *b2)
{
    __m256i bigmask;
    __m256i b1Data = _mm256_lddqu_si256((__m256i*) b1);
    __m256i b2Data = _mm256_lddqu_si256((__m256i*) b2);

    bigmask = _mm256_cmpeq_epi8(b1Data, b2Data);
    uint32_t bitmask = _mm256_movemask_epi8(bigmask);

    return bitmask;
}

static inline uint32_t match_forward_u16(uint16_t search_this, uint16_t* start_addr,  int nr_entries)
{
    // broadcast the next element sig value to all 8 slots of an __m256
    __m256i broadcasted = _mm256_set1_epi16 (search_this);
    __m256i all_values;
    __m256i compares;
    uint32_t bitmask32;
    uint16_t* start_addr_base = start_addr;
    
    
    int nr_bytes = 2 * nr_entries; // to convert into number of bytes to search
    if(nr_bytes >= 32) { // we are working with 32-byte maximum operating instructions
        all_values = _mm256_lddqu_si256((__m256i*)(start_addr)); // copy 32-bytes from possibly unaligned memory addr into register(?)
        compares = _mm256_cmpeq_epi16(broadcasted, all_values); // 16-bit pairwaise comparisons are stored in 'compares'.
                                                                // Whichever 16-bits matched, all-1's will be there in 
                                                                // corresponding location in 'compares'
        bitmask32 = _mm256_movemask_epi8(compares); // Look at 8 bits at a time, and whenver highest bit is 1, put a '1' in corresponding bit
                                                    // Thus, bitmask32 is 32-bit wide.
        if(bitmask32 != 0) {
            return _Bit_scan_forward(bitmask32) >> 1; // bit_scan_forward will return the index of the first 1-bit from left. 
                                                        // Documentation says, index of the least significant set 1-bit.
        }
        start_addr += 32/sizeof(uint16_t); // we looked at 32 bytes already; so, go past those.
    }

    while (start_addr - start_addr_base < nr_bytes) {
        if (*start_addr == search_this) {
            return (start_addr - start_addr_base)/2;
        }
        start_addr ++;
    }
    return 999999;
}

int main()
{
    uint16_t ieps[128];
    int input = 0, i = 0;
printf ("sizeof int %d long int %d\n", (int)sizeof(int), (int)sizeof(long int));
    unsigned int r = 0;
    uint16_t r16 = 0;
    printf ("Testing _Bit_scan_forward...\n");
    for (i=0; i<100000; i++) {
        r = (unsigned int) random();
        if (r == 0) continue;
        r16 = (uint16_t)r;
        int val1 = _bit_scan_forward(r);
        int val2 = __builtin_ctz((r));  
        assert(val1 == val2);
        val1 = _bit_scan_forward(r16);
        val2 = __builtin_ctz((r16));
        assert(val1 == val2);
    }
    printf ("Testing _Bit_scan_reverse...\n");
    for (i=0; i<100000; i++) {
        r = (uint16_t) random();
        if (r == 0) continue;
        r16 = (uint16_t)r;
        int val1 = _bit_scan_reverse(r);
        int val2 = 31 - __builtin_clz((r));  
        if (val1 != val2) {
            printf("i = %d, r = %u 0x%08x, bsr = %d, clz = %d\n", i, r, r, val1, val2);
            assert(val1 == val2);
        }
        val1 = _bit_scan_forward(r16);
        val2 = __builtin_ctz((r16));
        assert(val1 == val2);
    }


    for (i=0; i<128; i++) {
        if (i<5)
            ieps[i] = (uint16_t)(10);
        else if (i<10)
            ieps[i] = (uint16_t)(20);
        else
            ieps[i] = (uint16_t)(30+i);
    }
    while (input != -1)
    {
        printf("Give input: ");
        int rc = scanf("%d",&input);
        assert(rc == 1);
        if (input < 0 || input > 65535) {
            if (input != -1) {
                printf("Invalid input: %d\n", input);
                continue;
            }
        } else {
            uint16_t searchval = (uint16_t)input;
            printf("Found %d at index %d\n", (int)searchval, (int)match_forward_u16(searchval, ieps+1, 128)); // Playing with starting offset
            //printf("Found %d at index %d\n", (int)searchval, (int)findMatchForward16u(searchval, ieps, 128));
        }
    }
    return 0;
}

