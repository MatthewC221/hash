#include <stdio.h>

int main() {
	
	int divisor = 32;
	for (int i = 0; i < 9000000; i++) {
		// int tmp = (i & (divisor - 1));
		int tmp = i % divisor;
	}

	return 0;
}