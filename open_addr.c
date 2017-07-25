#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <limits.h>
#include <time.h>



int main(int argc, char *argv[]) 
{

	// Arguments, size, type

	if (argc == 2) {

		int size = atoi(argv[1]);
		Hash * H = createHash(2, size, OPEN_ADDR);

		srand(time(NULL));

		int * key = (int *)malloc(sizeof(int) * size);

		for (int i = 0; i < size; i++) {
			int key_tmp = 10 * i + rand () % 5;
			put(H, key_tmp, 1);
			key[i] = key_tmp;
		}

		printHash(H);
		for (int i = 0; i < size; i++) {
			printf("Checking %d = %d\n", key[i], get(H, key[i]));
			assert(1 == get(H, key[i]));
		}

		printHash(H);
		free_hash(H);
		free(key);

	} else {
		fprintf(stderr, "Usage ./open_addr <size>\n");
	}

	return EXIT_SUCCESS;
}