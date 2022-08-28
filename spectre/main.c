#include <stdio.h>
#include <stdlib.h>

char probe_array[4096 * 256];
char victim_array[10];

int victim_array_init()
{
    int i;
    for (i = 0; i < 9; i++)
    {
        victim_array[i] = 'a';
    }
    victim_array[9] = 'c';

    return 0;
}

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


int main(int argc, char *argv[])
{   
    int aux;
    victim_array_init();
    for (int i = 0; i < 9; i++)
    {
        aux = victim_array[i];
    }



    return 0;
}