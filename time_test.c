#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <assert.h>
#include <float.h>

int main(int argc, char *argv[])
{
    // Timing test for N elements
    
    // Default size = 17
    if (argc == 6) {
              
        double size = atoi(argv[1]);
        int check_flag = atoi(argv[2]);
        int delete_flag = atoi(argv[3]);
        int starting_size = atoi(argv[4]);
        int type = atoi(argv[5]);

        Hash * H = createHash(3, starting_size, type, INT_KEY_INT_VAL);

        /*
        // Optimising 
        if (!size || size > DBL_MAX) {
            fprintf(stderr, "Use size between 0->DOUBLE_MAX\n");
            return EXIT_FAILURE;
        }        
        */
        int key = 1;

        for (int i = 0; i < size; i++) {
            put(H, &i, &key);
        }
        if (check_flag) {
            for (int i = 0; i < size; i++) {
                assert(1 == get(H, i));
            }
        }

        if (delete_flag) {
            for (int i = 0; i < size; i++) {
                del(H, i);
            }
        }
        free_hash(H);

    } else {
        fprintf(stderr, "Usage ./time_insert <size> <check_flag> <delete_flag> <starting_size> <hash_type>\n");
    }

    return EXIT_SUCCESS;
}