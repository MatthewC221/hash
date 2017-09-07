#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>

struct my_struct {
    int id;            /* we'll use this field as the key */
    int val;            
    UT_hash_handle hh; /* makes this structure hashable */
};

struct my_struct *users = NULL;

void add_user(int user_id, int name) {
    struct my_struct *s;

    s = malloc(sizeof(struct my_struct));
    s->id = user_id;
    s->val = name;
    HASH_ADD_INT( users, id, s );  /* id: name of key field */
}

struct my_struct *find_user(int user_id)
{
    struct my_struct *s;

    HASH_FIND_INT( users, &user_id, s );  /* s: output pointer */
    return s;
}

int main(int argc, char *argv[]) 
{

	if (argc == 2) {
		int size = atoi(argv[1]);

		for (int i = 0; i < size; i++) {
			add_user(i, 1);
		}

		for (int i = 0; i < size; i++) {
			int tmp = find_user(i)->val;
			printf("tmp = %d\n", tmp);
		}

	} else {
		fprintf(stderr, "Usage ./time_test_uthash <size>\n");
	}

	return EXIT_SUCCESS;

}