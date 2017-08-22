#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <time.h>

/*
 * Prototyping string hash functions
 * This works pretty well, take 4 bytes at a time. Much larger range
 * Formula: (s[0] * 256) + (s[1] * 256 * 256) + (s[2] * 256 * 256 * 256) + (s[3] * 256 * 256 * 256 * 256)
 * However, the numbers can be too large at times
 * For things of (length % 4 != 0), we can sfold most of it. Then 
 * perform a simple addition
*/

void sfold(char *s, int length);

int main(int argc, char *argv[]) {

	if (argc == 2) {
		srand(time(NULL));
		int size = atoi(argv[1]);
		char *tmp = malloc(sizeof(char) * 8);

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 8; j++) {
				tmp[j] = rand() % 93 + 33;
			}
			sfold(tmp, 8);
		}
		free(tmp);
	} else {
		fprintf(stderr, "Usage ./string_hash <number_of_random>\n");
	}

	return EXIT_SUCCESS;
}

void sfold(char *s, int length) {

	assert(length % 4 == 0);
	long sum = 0;
	for (int j = 0; j < length; j = j + 4) {
		int mult = 1;
		for (int k = j; k < j + 4; k++) {
			sum += s[k] * mult;
			mult *= 256;
		}
	}
	printf("%s = %ld\n", s, sum);
 }