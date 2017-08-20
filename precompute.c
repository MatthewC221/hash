#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() 
{

	printf("int resize_default[22] = {\n\t");
	int count = 0;
	for (int i = 2; i < 4194304; i = i << 1, count++) {
		int tmp = (int)(i * 0.75);
		printf("%d, ", tmp);
		if (!(count % 5) && count != 0) printf("\n\t");
	}
	printf("};\n");

	return EXIT_SUCCESS;
}