#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

void delete_all() {
  struct my_struct *current_user, *tmp;

  HASH_ITER(hh, users, current_user, tmp) {
    HASH_DEL(users, current_user);  /* delete it (users advances to next) */
    free(current_user);             /* free it */
  }
}

int main(int argc, char *argv[]) 
{

	if (argc == 2) {
		int size = atoi(argv[1]);
        int range = size * 3;
        int * keys = malloc(sizeof(int) * size);

		for (int i = 0; i < size; i++) {
            int v = rand() % range;
			add_user(v, 1);
            keys[i] = v;
		}

        for (int i = 0; i < size; i++) {
            assert(1 == find_user(keys[i])->val);
        }
        free(keys);
        delete_all();

	} else {
		fprintf(stderr, "Usage ./time_test_uthash <size>\n");
	}

	return EXIT_SUCCESS;

}