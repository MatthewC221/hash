#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INT 1000000

int main(int argc, char *argv[])
{

	if (argc != 3) {
		fprintf(stderr, "Usage ./compute_mod <size> <check_flag>\n");
	} else {

		int size = atoi(argv[1]);
		int check_flag = atoi(argv[2]);

		int arr[INT] = {-1};

		arr[0] = 0;
		for (int i = 0; i < INT; i = i + size) {
			arr[i] = -1;
		}
		for (int i = 1; i < INT; i++) {
			if (arr[i - 1] == -1) {			// The last element had mod = 0
				arr[i] = 1;
			} else if (arr[i] != -1) {
				arr[i] = arr[i - 1] + 1;
			}
		}

		for (int i = 0; i < INT; i = i + size) {
			arr[i] = 0;
		}

		if (check_flag) {
			for (int i = 1; i < INT; i++) {
				int val = arr[i];
			}
		}
	}


	return EXIT_SUCCESS;
}