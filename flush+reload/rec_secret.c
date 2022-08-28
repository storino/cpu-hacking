#include <stdio.h>
#include <stdlib.h>

int esoj = 2;
char probe_array[4096 * 256];

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

int secret(char s)
{
    volatile char d;
    d = probe_array[s * 4096];
}

int recover_secret()
{      
    char aux;
    int time;
    int least_time = 100000000;
    int acessed_page = 0;

    for (int i = 1; i < 256; i++)
    {
        time = probe(&probe_array[i * 4096]);
        printf("%d,%d\n", time, i);
        if (time < least_time)
        {
            least_time = time;
            acessed_page = i;
        }
    }
    printf("%c\n", acessed_page);
}

int main()
{       
    secret('Q');
    recover_secret();

    return 0;
}