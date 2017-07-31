#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include <time.h>

int main(int argc, char *argv[]) 
{

	if (argc == 4) {

		srand(time(NULL));
		int size = atoi(argv[1]);
		int inserts = atoi(argv[2]);
		int type = atoi(argv[3]);
		Hash * H = createHash(2, size, type);

		int range = inserts * 3;

		int * keys = malloc(sizeof(int) * inserts);

		for (int i = 0; i < inserts; i++) {
			int r = rand() % range;
			put(H, r, 1);
			keys[i] = r;
		}

		for (int i = 0; i < inserts; i++) {
			assert(1 == get(H, keys[i]));
		}

		printf("Number of elements = %d, total size = %d\n", H->num_elem, H->cur_size);
		free_hash(H);
		free(keys);

	} else {
		fprintf(stderr, "Usage ./collision_speed <size_of_hash> <num_elem> <type>\n");
	}

	return EXIT_SUCCESS;

}