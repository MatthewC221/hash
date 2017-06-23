#include <stdio.h>
#include "hash.h"
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    Hash * H = createHash(6);
    
    put(H, 3, 1);
    put(H, 512, 1);
    put(H, 421, 1);
    put(H, 1, 421);
    
    for (int i = 0; i < 20; i++) {
        put(H, i, 1);
    }
    
    printHash(H);
    



    return EXIT_SUCCESS;
}
