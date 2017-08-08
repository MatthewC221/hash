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

		GHashTable *hash = g_hash_table_new_full (g_int_hash, g_int_equal, NULL, NULL);
		int * keys = malloc(sizeof(int) * size);

		for (int i = 0; i < size; i++) {
			int r = rand() % (size * 3);
			keys[i] = r;
			int *tmp = g_new0(gint, 1);
			*tmp = r;
			g_hash_table_insert(hash, tmp, GINT_TO_POINTER(1));
		}

		g_hash_table_destroy(hash);
	} else {
		fprintf(stderr, "Usage ./test <size>\n");
	}
    
    return EXIT_SUCCESS;

}
