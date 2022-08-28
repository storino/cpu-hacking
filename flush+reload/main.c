#include <stdio.h>
#include <stdlib.h>

int esoj = 2;

int probe(char *adrs) {

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

int main()
{       
    int time;
    int aux;
    int esoj_local = 2;
    void (*p)(int) = getenv;

    while (1){
        time = probe((char *)p);
        if (time < 300){
            printf("%d\n", time);
        }
    }

    return 0;
}