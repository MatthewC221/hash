#include <stdio.h>
#include <stdlib.h>
#include "../hash.h"
#include <time.h>

int main(int argc, char *argv[]) 
{
	
	if (argc == 4) {

		srand(time(NULL));
		int size = atoi(argv[1]);
		int inserts = atoi(argv[2]);
		int type = atoi(argv[3]);
		Hash * H = createHash(2, size, type);

		// Should be inserts * 3
		int range = inserts * 3; 

		int key = 1;
		for (int i = 0; i < inserts; i++) {
			int k = rand() % range;
			put(H, &k, &key);
		}

		free_hash(H);

	} else {
		fprintf(stderr, "Usage ./collision_speed <size_of_hash> <num_elem> <type>\n");
	}

	return EXIT_SUCCESS;

}