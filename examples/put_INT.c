#include <mhash.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Hash * H = createHash(3, 16, OPEN_ADDR, INT_KEY_INT_VAL);      // Creates size 16 of int key, int val

    int key = 1023;
    int val = 4;

    put(H, &key, &val);
    printHash(H);

    free_hash(H);

    return EXIT_SUCCESS;
}
