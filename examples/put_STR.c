#include <mhash.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Hash * H = createHash(3, 16, OPEN_ADDR, INT_KEY_STR_VAL);      // Creates size 16 of int key, str val

    int key = 1023;
    char *val = "hello";

    put(H, &key, &val);
    printHash(H);

    free_hash(H);

    return EXIT_SUCCESS;
}
