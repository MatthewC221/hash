#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{

    // Basic test
    
    if (argc == 2) {
	    srand(time(NULL));
	    int size = atoi(argv[1]);

	    int *key = NULL;
	    int *val = NULL;
	    gint *k_one = NULL;

		GHashTable *hash = g_hash_table_new_full (g_int_hash, g_int_equal, free, NULL);
		// int * keys = malloc(sizeof(int) * size);

		for (int i = 0; i < size; i++) {
			/*
		  	key = malloc( sizeof(*key) );
		  	*key = i;
		  	val = malloc( sizeof(*val) );
		  	*val = 1;
			*/
		  	k_one = g_new(gint, 1);
		  	*k_one = i;
		  	g_hash_table_insert(hash, key, GINT_TO_POINTER(1));
		}

		/*
		for (int i = 0; i < size; i++) {
			key = malloc(sizeof(*key));
			*key = i;
			gpointer *p = g_hash_table_lookup(hash, key);
			assert (GPOINTER_TO_INT(p) == i);
			free(key);
		}	
		*/
		g_hash_table_destroy(hash);
	} else {
		fprintf(stderr, "Usage ./test <size>\n");
	}
    
    return EXIT_SUCCESS;

}
