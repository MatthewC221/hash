#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) 
{

	// Arguments, size, type

	if (argc == 2) {

		int size = atoi(argv[1]);
		Hash * H = createHash(2, size, OPEN_ADDR);

		srand(time(NULL));
		for (int i = 0; i < size; i++) {
			int num = rand() % 100;
			put(H, num, 1);
		}

		printHash(H);
		free_hash(H);

	} else {
		fprintf(stderr, "Usage ./open_addr <size>\n");
	}

	return EXIT_SUCCESS;
}