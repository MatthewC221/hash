#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

void insert(int *arr, int key, int val);

int hash_size = 0;
int unique_elements = 0;

int main(int argc, char *argv[]) 
{
    
    // Unit testing for put and get with collisions
    
    printf("\n/// Unit testing put and get w/ collisions /// \n\n");
    srand(time(NULL));
    
    hash_size = 100;
    Hash * H = createHash(1, hash_size);
    
    printf("Creating hash of size: %d\n", hash_size);
    
    int *arr_k = malloc(sizeof(int) * hash_size);     // Array of keys and values
    
    // Insert -1 to indicate empty key/val pair
    for (int i = 0; i < hash_size; i++) {
        arr_k[i] = -1;
    }
    
    
    for (int i = 0; i < 100; i++) {
        int rand_k = rand() % 99;
        int rand_v = rand() % 100;
        printf("Inserting key:value = %d:%d\n", rand_k, rand_v);
        put(H, rand_k, rand_v);
        arr_k[rand_k] = rand_v;
    }
    
    printf("\n");
    printHash(H);
    
    for (int i = 0; i < 100; i++) {
        printf("Test %d: look up on key %d\n", i + 1, i);
        
        int res = get(H, i);
        printf("The value = %d, %d\n", arr_k[i], res);
        
        if (arr_k[i] == -1) {
            assert(res == INT_MIN);
        } else {
            assert(arr_k[i] == res);
            del(H, i);
        }
        printf("Test %d passed\n\n", i + 1);
    }
     ;
    printf("Load factor = %2f\n", load_factor(H));
    free_hash(H);
    free(arr_k);
   
    return EXIT_SUCCESS;
}

