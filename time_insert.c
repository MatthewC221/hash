#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    // Timing test for 10000 elements
    
    clock_t start = clock(), diff;
    // Insert 10000 items
    Hash * H = createHash(0);
    
    // Default size = 17
    
    int keys[100000] = {0};
    
    for (int i = 0; i < 100000; i++) {
        int num = rand() % 100000;
        put(H, num, 1);
        keys[i] = num;
    }

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
    printf("Num elem = %d\n", H->num_elem);

    for (int i = 0; i < 100000; i++) {
        int tmp = keys[i];
        assert(1 == get(H, tmp));
    }

    free_hash(H);

    return EXIT_SUCCESS;
}
