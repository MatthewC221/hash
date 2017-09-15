
#include "hash.h"

// Precomputed hash sizes

/*
int prime_size[20] = {
    5, 11, 23, 47, 97,
    197, 397, 797, 1597, 3203,
    6421, 12853, 25717, 51437, 102877,
    205759, 411527, 823117, 1646237, 3292489
};

int log_prime[20] = {
    3, 4, 5, 6, 7,
    8, 9, 10, 11, 12,
    13, 14, 15, 16, 17,
    18, 19, 20, 21, 22
};

int resize_default[20] = {
    3, 8, 17, 35, 72,
    147, 297, 597, 1197, 2402,
    4815, 9639, 19287, 38577, 77157,
    154319, 308645, 617337, 1234677, 2469366
};
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

/*
int log_prime[24] = {
    3, 4, 5, 6, 7, 
    8, 9, 10, 11, 12, 13, 
    14, 15, 16, 17, 18, 19, 
    20, 21, 22, 23, 24, 25, 26, 27
};
*/

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
            new_hash->int_k_int_v = (INT_k_INT_v **)calloc(starting_size, SIZE_int_k_int_v);
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
void put(Hash * H, void *cur_key, void *cur_value) {

    switch (H->k_v_type) {
        case 1:     // int key, int val
        {
            int k = *((intptr_t *) cur_key);
            int v = *((intptr_t *) cur_value);
            put_INT_k_INT_v(H, k, v);
            break;
        }
        case 2:     // int key, str value
        {
            int k = *((intptr_t *) cur_key); 
            char *v = *((char **) cur_value);
            put_INT_k_STR_v(H, k, v);
        }
    }
}

// Putting int key and int val in
void put_INT_k_INT_v (Hash * H, int cur_key, int cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));

    // Linear probing once around for a spot  
    INT_k_INT_v * new_node = createINT_k_INT_v(cur_key, cur_value, 0);
    while (1) {
        // Inserting new key
        if (H->int_k_int_v[gen_key] == NULL || H->int_k_int_v[gen_key]->distance == SHRT_MAX) {
            H->num_elem++;
            H->int_k_int_v[gen_key] = new_node;
            break;
        // Overwriting key
        } else if (H->int_k_int_v[gen_key]->k == new_node->k) {
            swap_INT_k_INT_v(&H->int_k_int_v[gen_key], &new_node);
            free(new_node);
            return;
        }
        // Robin hood hash
        // If the distance of the current key has probed less, swap_INT_k_INT_v and insert the curr key

        if (H->int_k_int_v[gen_key]->distance < new_node->distance) {
            swap_INT_k_INT_v(&H->int_k_int_v[gen_key], &new_node);
            gen_key = (cur_key & (H->cur_size - 1));
            new_node->distance--;
        }

        gen_key++;
        new_node->distance++;  

        if (gen_key >= H->cur_size) gen_key = 0;
        // If we reach the probe limit, resize the hash 
           
        if (new_node->distance >= H->probe_limit) {
            resize_OPEN_INT_k_INT_v(H);
            gen_key = (new_node->k & (H->cur_size - 1));
            new_node->distance = 0;
        }
    }

    if (H->num_elem >= H->to_resize) {
        resize_OPEN_INT_k_INT_v(H);
    }

    return;
}

void put_INT_k_STR_v (Hash * H, int cur_key, char * cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));

        // Linear probing once around for a spot  
    INT_k_STR_v * new_node = createINT_k_STR_v(cur_key, cur_value, 0);

    while (1) {
        // Inserting new key
        if (H->int_k_str_v[gen_key] == NULL || H->int_k_int_v[gen_key]->distance == SHRT_MAX) {
            H->num_elem++;
            H->int_k_str_v[gen_key] = new_node;
            break;
        // Overwriting key
        } else if (H->int_k_str_v[gen_key]->k == new_node->k) {
            swap_INT_k_STR_v(&H->int_k_str_v[gen_key], &new_node);
            free(new_node->v);
            free(new_node);
            return;
        }
        // Robin hood hash
        // If the distance of the current key has probed less, swap_INT_k_INT_v and insert the curr key
        // now that the new key is inserted       
        if (H->int_k_str_v[gen_key]->distance < new_node->distance) {
            swap_INT_k_STR_v(&H->int_k_str_v[gen_key], &new_node);
            gen_key = (cur_key & (H->cur_size - 1));
            // Don't increment distance until next check
            new_node->distance--;
        }
        gen_key++;
        new_node->distance++;  

        if (gen_key >= H->cur_size) gen_key = 0;
        // If we reach the probe limit, resize the hash     
        if (new_node->distance >= H->probe_limit) {
            resize_OPEN_INT_k_STR_v(H);
            gen_key = (new_node->k & (H->cur_size - 1));
            new_node->distance = 0;
        }
    }

    if (H->num_elem >= H->to_resize) resize_OPEN_INT_k_STR_v(H);

    return;
}

//** swap_INT_k_INT_v two nodes
void swap_INT_k_INT_v(INT_k_INT_v ** tmp1, INT_k_INT_v ** tmp2) 
{
    INT_k_INT_v * tmp = *tmp1; 
    *tmp1 = *tmp2;
    *tmp2 = tmp;
}

void swap_INT_k_STR_v(INT_k_STR_v ** tmp1, INT_k_STR_v ** tmp2) 
{
    INT_k_STR_v * tmp = *tmp1; 
    *tmp1 = *tmp2;
    *tmp2 = tmp;
}

//** Attempts an overwrite, if no key present return 0
unsigned int overwriteKey(Hash * H, int key, int val, int gen_key)
{

    unsigned int count = 0;
    for (; count < H->probe_limit; count++, gen_key++) {
        if (H->int_k_int_v[gen_key] && H->int_k_int_v[gen_key]->k == key) {
            H->int_k_int_v[gen_key]->v = val;
            return 1;
        } else if (H->int_k_int_v[gen_key] == NULL) {
            break;
        }

        if (gen_key >= H->cur_size - 1) {
            gen_key = 0;
        }
    }

    return 0;
}

//** Prints all indexes and elements of the hash
void printHash (Hash * H) 
{

    printf("Open addressing hash\n");

    switch(H->k_v_type) {
        case (1):
            for (int i = 0; i < H->cur_size; i++) {
                printf("[%s%d%s] : ", YELLOW, i, END);
                if (H->int_k_int_v[i] && H->int_k_int_v[i]->k != INT_MIN) {
                    printf("(%d:%d) d=%d\n", H->int_k_int_v[i]->k, H->int_k_int_v[i]->v, H->int_k_int_v[i]->distance);
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

    new_hash->probe_limit = old_H->probe_limit + 1;     
    new_hash->type = old_H->type;                        // Collision / open addr
    new_hash->cur_size = old_H->cur_size * 2;
    new_hash->load_factor = DEFAULT_LF;           // Default load factor (change at 0.75 = N / size) 
    new_hash->to_resize = old_H->to_resize * 2;    
    new_hash->k_v_type = old_H->k_v_type;

    new_hash->int_k_int_v = (INT_k_INT_v **)calloc(new_hash->cur_size, SIZE_int_k_int_v); 

    for (int i = 0; i < old_H->cur_size; i++) {
        if (old_H->int_k_int_v[i] && old_H->int_k_int_v[i]->k != INT_MIN) {
            insert_int_int(new_hash, old_H->int_k_int_v[i]->k, old_H->int_k_int_v[i]->v);
            free(old_H->int_k_int_v[i]);
        }
    }

    free(old_H->int_k_int_v);
    // printf("num elem = %d\n", new_H->num_elem);
    *old_H = *new_hash;
    old_H->num_elem = saved;
    free(new_hash);
}


void resize_OPEN_INT_k_STR_v(Hash * old_H)
{

    Hash * new_H = createHash(3, 2 * old_H->cur_size, OPEN_ADDR, INT_KEY_STR_VAL);

    for (int i = 0; i < old_H->cur_size; i++) {
        if (old_H->int_k_str_v[i] && old_H->int_k_str_v[i]->k != INT_MIN) {
            put_INT_k_STR_v(new_H, old_H->int_k_str_v[i]->k, old_H->int_k_str_v[i]->v);
            free(old_H->int_k_str_v[i]->v);
            free(old_H->int_k_str_v[i]);
        }
    }

    free(old_H->int_k_str_v);
    *old_H = *new_H;
    free(new_H);
}

void insert_int_int(Hash * H, int cur_key, int cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));
    // Linear probing once around for a spot  
    INT_k_INT_v * new_node = createINT_k_INT_v(cur_key, cur_value, 0);
    while (1) {
        // Inserting new key
        if (H->int_k_int_v[gen_key] == NULL || H->int_k_int_v[gen_key]->k == INT_MIN) {
            H->num_elem++;
            H->int_k_int_v[gen_key] = new_node;
            break;
        } 
        // Robin hood hash
        if (H->int_k_int_v[gen_key]->distance < new_node->distance) {
            swap_INT_k_INT_v(&H->int_k_int_v[gen_key], &new_node);
            gen_key = (cur_key & (H->cur_size - 1));
            new_node->distance--;
        }

        gen_key++;
        new_node->distance++;  

        if (gen_key >= H->cur_size) gen_key = 0;
    }
}

/*
void insert_int_str(Hash * H, int cur_key, char * cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));
    // Linear probing once around for a spot  
    INT_k_INT_v * new_node = createINT_k_STR_v(cur_key, cur_value, 0);
    while (1) {
        // Inserting new key
        if (H->int_k_int_v[gen_key] == NULL || H->int_k_int_v[gen_key]->k == INT_MIN) {
            H->num_elem++;
            H->int_k_int_v[gen_key] = new_node;
            break;
        } 
        // Robin hood hash
        if (H->int_k_int_v[gen_key]->distance < new_node->distance) {
            swap_INT_k_INT_v(&H->int_k_int_v[gen_key], &new_node);
            gen_key = (cur_key & (H->cur_size - 1));
            new_node->distance--;
        }

        gen_key++;
        new_node->distance++;  

        if (gen_key >= H->cur_size) gen_key = 0;
    }
}
*/
//** Returns the current load factor
double load_factor(Hash * H) 
{
    printf("Nodes / size = %d / %d\n", H->num_elem, H->cur_size);
    H->load_factor = (double)H->num_elem / H->cur_size;
    return H->load_factor;
}


//** Frees entire hash
void free_hash (Hash * H) 
{
    switch (H->k_v_type) {
        case (1):
            for (int i = 0; i < H->cur_size; i++) {
                if (H->int_k_int_v[i]) {
                    free(H->int_k_int_v[i]);
                }
            }
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


//** Frees a node, if that node doesn't exist print err
void del(Hash * H, int key) 
{
    int gen_key = (key & (H->cur_size - 1));
    int dist_from_key = 0;
    // Search for key
    while (1) {
        // Can't free the node, just set it to a marker (INT_MIN)
        if (H->int_k_int_v[gen_key] && H->int_k_int_v[gen_key]->k == key) {
            H->int_k_int_v[gen_key]->distance = SHRT_MAX;         // Distance MAX_INT (this is not possible)
            break;
        }
        gen_key++;
        dist_from_key++;

        // Limit for probing exceeded, non existent
        if (H->int_k_int_v[gen_key] == NULL || dist_from_key >= H->probe_limit) break;    
        if (gen_key >= H->cur_size) gen_key = 0;

    }        
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
    int dist_from_key = 0;

    while (1) {
        if (H->int_k_int_v[gen_key] && H->int_k_int_v[gen_key]->k == key) {
            return H->int_k_int_v[gen_key]->v;
        }
        gen_key++;
        dist_from_key++;

        if (gen_key >= H->cur_size) gen_key = 0;
        if (H->int_k_int_v[gen_key] == NULL || dist_from_key >= H->probe_limit) break;     // Non-existent key
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