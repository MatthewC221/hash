#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{

    // Basic test
    
    Hash * H = createHash(1, 100);
    srand(time(NULL));
    
    for (int i = 0; i < 200; i++) {
        int num = rand() % 10000 + i;
        put(H, num, 10);
    }
    
    //printHash(H);
    free_hash(H);
    
    return EXIT_SUCCESS;

}
