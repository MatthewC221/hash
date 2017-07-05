#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include <time.h>

int main(int argc, char * argv[])
{
    
    // Testing changing the load factor
    
    Hash * H = createHash(0);
    set_lf(H, 1.5);
    srand(time(NULL));
    
    for (int i = 0; i < 1000; i++) {
        int num = rand() % 10000;
        put(H, num, 1);
    }
    
    printHash(H);
    free_hash(H);


    return EXIT_SUCCESS;
}
