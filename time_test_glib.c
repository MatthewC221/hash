#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{

    // Basic test
    
    if (argc == 2) {
	    srand(time(NULL));
	    int size = atoi(argv[1]);

	    int *key = NULL;
	    int *val = NULL;
	    
		GHashTable *hash = g_hash_table_new_full (g_int_hash, g_int_equal, free, free);
		// int * keys = malloc(sizeof(int) * size);

		for (int i = 0; i < size; i++) {
		  	key = malloc( sizeof(*key) );
		  	*key = i;
		  	val = malloc( sizeof(*val) );
		  	*val = 1;
		  	g_hash_table_insert(hash, key, val);
		}

		g_hash_table_destroy(hash);
	} else {
		fprintf(stderr, "Usage ./test <size>\n");
	}
    
    return EXIT_SUCCESS;

}
