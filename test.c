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
    
    /*
    for (int i = 0; i < 20; i++) {
        free_node(H, i);
        printHash(H);
    }
    
    free_node(H, 421);
    free_node(H, 512);
    free_node(H, 1);
    
    */
    printHash(H);
    //free_hash(H);
    printHash(H);



    return EXIT_SUCCESS;
}
