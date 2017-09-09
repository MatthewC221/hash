
#include "exp_hash.h"

/* This was entirely experimental
*/

int powers[POW_SIZE] = {
    2, 4, 8, 16, 32,
    64, 128, 256, 512, 1024,
    2048, 4096, 8192, 16384, 32768,
    65536, 131072, 262144, 524288, 1048576,
    2097152, 4194304, 8388608, 16777216
};

int log_prime[POW_SIZE] = {
    1, 2, 3, 4, 5,
    6, 7, 8, 9, 10,
    11, 12, 13, 14, 15,
    16, 17, 18, 19, 20,
    21, 22, 23, 24
};



int resize_default[POW_SIZE] = {
    1, 3, 6, 12, 24, 48, 
    96, 192, 384, 768, 1536, 
    3072, 6144, 12288, 24576, 49152, 
    98304, 196608, 393216, 786432, 1572864, 
    6291456, 12582912
};

void set_lf (Hash * H, double new_load)
{
    if (new_load) {
        H->load_factor = new_load;
        H->to_resize = new_load * H->cur_size;
    }
}

//** Creates the hash
Hash * createHash(int elements, ...) 
{
    va_list arg_list;
    int starting_size = 8;                    // Default size
    int type = OPEN_ADDR;                     // Default type
    int type_k_v = INT_KEY_INT_VAL;           // Default int key, int val

    if (elements > 4) {
        fprintf(stderr, "<Hash.h>: supplied too many parameters to <Hash.h>: createHash(): max 2\n");
        return NULL;
    }
    
    // Hash * H = createHash(4, starting_size, OPEN_ADDR, INT_k, STR_v)

    va_start (arg_list, elements);  
    for (int i = 0; i < elements; i++) {
        switch (i) {
            case 0:               
                starting_size = va_arg(arg_list, int);
                break;
            case 1:
                type = va_arg(arg_list, int);
                assert(type == COLLISION || type == OPEN_ADDR);
                break;
            case 2:
                type_k_v = va_arg(arg_list, int);
                assert(type_k_v > 0 && type_k_v < 5);
                break;
        }
    }   

    int index = 0;
    for (; index < POW_SIZE; index++) {
        if (powers[index] >= starting_size) {
            starting_size = powers[index];
            break;
        }
    }

    Hash * new_hash = (Hash *)malloc(SIZE_hash);

    // 4 cases
    switch (type_k_v) {
        case 1:
            new_hash->int_k_int_v = (int *)calloc(starting_size, sizeof(int));
            break;       
        case 2:
            new_hash->int_k_str_v = (INT_k_STR_v **)calloc(starting_size, SIZE_int_k_str_v);
            break; 
        case 3:
            new_hash->str_k_str_v = (STR_k_STR_v **)calloc(starting_size, SIZE_str_k_str_v);
            break;                         
        case 4:
            new_hash->str_k_int_v = (STR_k_INT_v **)calloc(starting_size, SIZE_str_k_int_v);
            break;                 
    }
    // if (!new_hash->int_k_int_v) fprintf(stderr, "Failed to create memory from malloc\n"); exit(0);
    new_hash->probe_limit = index + 1;     

    new_hash->type = type;                        // Collision / open addr
    new_hash->cur_size = starting_size;
    new_hash->num_elem = 0;
    new_hash->load_factor = DEFAULT_LF;           // Default load factor (change at 0.75 = N / size) 
    new_hash->to_resize = resize_default[index];    
    new_hash->k_v_type = type_k_v;

    return new_hash;
}

//** Insert element in hash
void put(Hash * H, int k) {

    switch (H->k_v_type) {
        case 1:     // int key, int val
        {
            put_INT_k_INT_v(H, k);
            break;
        }
    }
}

// Putting int key and int val in
void put_INT_k_INT_v (Hash * H, int cur_key) 
{
    int gen_key = (cur_key & (H->cur_size - 1));

    while (1) {
        // Inserting new key
        if (H->int_k_int_v[gen_key] == 0) {
            H->num_elem++;
            H->int_k_int_v[gen_key] = cur_key;
            break;
        // Overwriting key
        } else if (H->int_k_int_v[gen_key] == cur_key) {
            return;
        }
        gen_key++;
        if (gen_key >= H->cur_size) gen_key = 0;
        // If we reach the probe limit, resize the hash 
    }

    if (H->num_elem >= H->to_resize) {
        resize_OPEN_INT_k_INT_v(H);
    }

    return;
}

//** Prints all indexes and elements of the hash
void printHash (Hash * H) 
{

    printf("Open addressing hash\n");

    switch(H->k_v_type) {
        case (1):
            for (int i = 0; i < H->cur_size; i++) {
                printf("[%s%d%s] : ", YELLOW, i, END);
                if (H->int_k_int_v[i] != 0) {
                    printf("(%d:exist)\n", H->int_k_int_v[i]);
                } else {
                    printf("\n");
                } 
            }
            break;
        case (2):
            for (int i = 0; i < H->cur_size; i++) {
                printf("[%s%d%s] : ", YELLOW, i, END);
                if (H->int_k_str_v[i] && H->int_k_str_v[i]->k != INT_MIN) {
                    printf("(%d:%s) d=%d\n", H->int_k_str_v[i]->k, H->int_k_str_v[i]->v, H->int_k_str_v[i]->distance);
                } else {
                    printf("\n");
                } 
            }
            break;            
    }
}


//** Resizing for open addressing hash
void resize_OPEN_INT_k_INT_v(Hash * old_H)
{
    //Hash * new_hash = createHash(3, 2 * old_H->cur_size, OPEN_ADDR, INT_KEY_INT_VAL);

    Hash * new_hash = (Hash *)malloc(SIZE_hash);

    int saved = old_H->num_elem;

    new_hash->type = old_H->type;                        // Collision / open addr
    new_hash->cur_size = old_H->cur_size * 2;
    new_hash->load_factor = DEFAULT_LF;           // Default load factor (change at 0.75 = N / size) 
    new_hash->to_resize = old_H->to_resize * 2;    
    new_hash->k_v_type = old_H->k_v_type;

    new_hash->int_k_int_v = NULL;
    new_hash->int_k_int_v = (int *)calloc(new_hash->cur_size, sizeof(int)); 

    for (int i = 0; i < old_H->cur_size; i++) {
        if (old_H->int_k_int_v[i] != 0) {
            int k = old_H->int_k_int_v[i];
            put_INT_k_INT_v(new_hash, k);
        }
    }

    free(old_H->int_k_int_v);
    *old_H = *new_hash;
    old_H->num_elem = saved;
    free(new_hash);

}


//** Frees entire hash
void free_hash (Hash * H) 
{
    switch (H->k_v_type) {
        case (1):
            free(H->int_k_int_v);
            H->int_k_int_v = NULL;
            break;
        case(2):
            for (int i = 0; i < H->cur_size; i++) {
                if (H->int_k_str_v[i]) {
                    free(H->int_k_str_v[i]->v);
                    free(H->int_k_str_v[i]);
                }
            }
            free(H->int_k_str_v);
            H->int_k_str_v = NULL;
            break;           
    }
    free(H);
}

//** Get value from key
int get(Hash * H, void * key) 
{
    switch (H->k_v_type) {
        case 1:     // int key, int val
        {
            int k = *((int *) key);
            return get_INT_k_INT_v(H, k);
        }
        case 2:     // int key, str value
        {
            int k = *((int *) key); 
            return get_INT_k_STR_v(H, k); 
        }
    }
}

int get_INT_k_INT_v(Hash * H, int key)
{

    int gen_key = (key & (H->cur_size - 1));

    while (1) {
        if (H->int_k_int_v[gen_key] == key) {
            return H->int_k_int_v[gen_key];
        }
        gen_key++;

        if (gen_key >= H->cur_size) gen_key = 0;
        if (H->int_k_int_v[gen_key] == 0) break;     // Non-existent key
    }

    return INT_MIN;
}

// TODO
int get_INT_k_STR_v(Hash * H, int key) { return 0; }

//** Creates a node for open addressing
INT_k_INT_v * createINT_k_INT_v(int cur_key, int cur_value, int dist)
{
    INT_k_INT_v * N = (INT_k_INT_v *)malloc(SIZE_int_k_int_v);
    N->k = cur_key;
    N->v = cur_value;
    N->distance = dist;
    // if (!N) fprintf(stderr, "Malloc failed hash.c: 605\n"); exit(0);
    return N;
}

INT_k_STR_v * createINT_k_STR_v(int cur_key, char *cur_value, int dist) 
{
    INT_k_STR_v * N = (INT_k_STR_v *)malloc(SIZE_int_k_str_v);
    N->k = cur_key;
    N->v = (char *)malloc((strlen(cur_value) + 1) * sizeof(char));
    strcpy(N->v, cur_value);
    N->distance = dist;
    return N;
}