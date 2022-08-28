#include <stdio.h>
#include <stdlib.h>

char probe_array[256 * 4096];

int
main(int argc, char const *argv[])
{
    volatile char d;
    d = probe_array['A' * 4096];
    printf("%c", d);
    return 0;
}
