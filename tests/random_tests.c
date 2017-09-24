#include <stdio.h>
#include "../hash.h"
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc == 2) {

		int INSERTS = atoi(argv[1]);
		char *key = "hellosd";
		Hash * H = createHash(3, 8, OPEN_ADDR, INT_KEY_STR_VAL);
		for (int i = 0; i < INSERTS; i++) {
			put(H, &i, &key);
		}

		for (int i = 0; i < INSERTS; i++) {
			del(H, i);
		}	

		for (int i = 0; i < INSERTS; i++) {
			put(H, &i, &key);
		}	

		free_hash(H);
	} else {
		fprintf(stderr, "Usage ./random_tests <size>\n");
	}
	return EXIT_SUCCESS;
}