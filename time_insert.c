#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <float.h>

int main(int argc, char *argv[])
{
    // Timing test for 10000 elements
    
    // Default size = 17
    
    if (argc == 4) {
        
        // Timing everything
        clock_t start = clock(), diff;
        srand(time(NULL));
        
        Hash * H = createHash(0);
    
        double size = atoi(argv[1]);
        int random_range = atoi(argv[2]);
        
        if (!size || size > DBL_MAX) {
            fprintf(stderr, "Use size between 0->DOUBLE_MAX\n");
            return EXIT_FAILURE;
        }
        
        if (random_range < 1 || random_range > INT_MAX) {
            fprintf(stderr, "Use range between 1->INT_MAX\n");
            return EXIT_FAILURE;
        }          

        int *keys = malloc(sizeof(int) * size);
        
        for (int i = 0; i < size; i++) {
            int num = rand() % random_range;
            put(H, num, 1);
            keys[i] = num;
        }

        diff = clock() - start;
        int msec = diff * 1000 / CLOCKS_PER_SEC;
        
        int check_flag = atoi(argv[3]);
        
        if (check_flag) {
            for (int i = 0; i < size; i++) {
                int tmp = keys[i];
                assert(1 == get(H, tmp));
            }
        }

        //printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
        printf("Total items = %d\n", H->num_elem);

        free_hash(H);

    } else {
        fprintf(stderr, "Usage ./time_insert <size> <random_range> <check_flag>\n");
    }

    return EXIT_SUCCESS;
}
