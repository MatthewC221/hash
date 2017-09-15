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

		for (int i = 0; i < size; i++) {
			int r = rand() % (size * 3);
		  	key = malloc( sizeof(*key) );
		  	*key = r;
		  	g_hash_table_insert(hash, key, GINT_TO_POINTER(1));
		}

		g_hash_table_destroy(hash);
		hash = NULL;
	} else {
		fprintf(stderr, "Usage ./test <size>\n");
	}
    
    return EXIT_SUCCESS;

}
