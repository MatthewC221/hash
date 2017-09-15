#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

void key_destroyed(gpointer data) { free(data); }

int main(int argc, char *argv[]) 
{

    // Basic test
    
    if (argc == 2) {
	    srand(time(NULL));
	    int size = atoi(argv[1]);

	    int *key = NULL;
	    int *val = NULL;
		GHashTable *hash = g_hash_table_new_full (g_int_hash, g_int_equal, free, NULL);
		int * keys = malloc(sizeof(int) * size);

		for (int i = 0; i < size; i++) {
			int r = rand() % (size * 3);
		  	key = malloc( sizeof(*key) );
		  	*key = r;
		  	keys[i] = r;
		  	g_hash_table_insert(hash, key, GINT_TO_POINTER(1));
		}

		
		for (int i = 0; i < size; i++) {
			key = malloc(sizeof(*key));
			*key = keys[i];
			gpointer *p = g_hash_table_lookup(hash, key);
			assert(GPOINTER_TO_INT(p) == 1);
			free(key);
		}
		
		free(keys);
		g_hash_table_destroy(hash);
		hash = NULL;
	} else {
		fprintf(stderr, "Usage ./test <size>\n");
	}
    
    return EXIT_SUCCESS;

}
