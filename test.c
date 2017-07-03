#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{

    // Basic test
    
    Hash * H = createHash(1, 100);
    
    for (int i = 0; i < 99; i++) {
        int num = rand() % 1000 + i;
        put(H, num, 10);
    }
    
    printHash(H);

    
    return EXIT_SUCCESS;

}
