#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INT 1000000

int main(int argc, char *argv[]) 
{
	if (argc == 2) {

		int size = atoi(argv[1]);
		int val;
		for (int i = 1; i < INT; i++) {
			val = i % size; 
		}
	} 
	return EXIT_SUCCESS;
}