#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

void printArgs(void *first, void *second);

int main(int argc, char *argv[]) {
	
	int a = 2147483647;
	char *str = "hello";
	void *ptr = &a;
	void *ptr1 = &str;
	printArgs(&a, &str);

	return EXIT_SUCCESS;
}

void printArgs(void *first, void *second) {

	int i = *((int *) first);
	char *j = *((char **) second);

	printf("%d\n", i);
	printf("%s\n", j);
	size_t len = strlen(j);
	printf("strlen = %lu\n", len);
}