#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{
    
    // Unit tests for put and get (with free)
    
    printf("\n/// Unit testing put and get /// \n\n");
    srand(time(NULL));
    int hash_size = rand() % 100 + 60;
    Hash * H = createHash(1, hash_size);
    
    printf("Creating hash of size: %d\n", hash_size);
    
    int *array_k = malloc(sizeof(int) * hash_size);     // Array of keys
    int *array_v = malloc(sizeof(int) * hash_size);     // Array of values
    
    for (int i = 0; i < 50; i++) {
        int rand_k = i;
        int rand_v = rand() % 100;
        printf("Inserting key:value = %d:%d\n", rand_k, rand_v);
        put(H, rand_k, rand_v);
        array_k[i] = rand_k;
        array_v[i] = rand_v;
    }
    
    printf("\n");
    for (int i = 0; i < 50; i++) {
        printf("Test %d: look up on key %d\n", i + 1, i);
        int res = get(H, array_k[i]);
        assert(array_v[i] == res);
        printf("Test %d passed\n\n", i + 1);
    }
     
    printHash(H);
    printf("Load factor = %2f\n", load_factor(H));
    
   
    free(array_k);
    free(array_v);
    free_hash(H);
    



    return EXIT_SUCCESS;
}
