#ifndef _mhash_h
#define _mhash_h
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef NAN
/* NAN is supported */
#endif

#ifdef INFINITY
/* INF is supported */
#endif

// Colours for error and diagnostic prints
#define RED  "\x1B[31m"            
#define GREEN "\e[32m"
#define YELLOW "\e[93m"
#define END  "\e[0m"   

#define POW_SIZE 24

#define COLLISION 1
#define OPEN_ADDR 2

#define INT_KEY_INT_VAL 1
#define INT_KEY_STR_VAL 2
#define STR_KEY_STR_VAL 3
#define STR_KEY_INT_VAL 4

// Pre-defined sizes for optimisations
#define SIZE_int_k_int_v sizeof(struct INT_k_INT_v)
#define SIZE_int_k_str_v sizeof(struct INT_k_STR_v)
#define SIZE_str_k_str_v sizeof(struct STR_k_STR_v)
#define SIZE_str_k_int_v sizeof(struct STR_k_INT_v)

#define SIZE_hash sizeof(struct Hash)
#define SIZE_int_node sizeof(struct int_node)
#define SIZE_int sizeof(int)

#define DEFAULT_LF 0.75

// Collision nodes
typedef struct int_node
{
    int k;
    int v;
    struct int_node *next; 
    
} int_node;

// Open addressing: int key, int val
typedef struct INT_k_INT_v
{
    int k;
    int v;
    short distance;

} INT_k_INT_v;

// Open addressing: int key, string val
typedef struct INT_k_STR_v
{
    int k;
    char *v;
    int distance;

} INT_k_STR_v;

typedef struct STR_k_STR_v
{
    char *k;
    char *v;
    short distance;

} STR_k_STR_v;

typedef struct STR_k_INT_v
{
    char *k;
    int v;
    short distance;

} STR_k_INT_v;

typedef struct Hash
{
    // Different key, value types 
    INT_k_INT_v ** int_k_int_v;   
    INT_k_STR_v ** int_k_str_v;
    STR_k_STR_v ** str_k_str_v;
    STR_k_INT_v ** str_k_int_v;

    unsigned int cur_size;
    unsigned int num_elem; 
    unsigned int k_v_type;             
    // Size until next resize
    unsigned int to_resize;     
    // Max probes until resize     
    unsigned int probe_limit;
    double load_factor;     
    unsigned int type;               

} Hash;

// Function decl
extern INT_k_INT_v * createINT_k_INT_v(int cur_key, int cur_value, int dist);
extern INT_k_STR_v * createINT_k_STR_v(int cur_key, char *cur_value, int dist);

extern Hash * createHash(int elements, ...);

extern void put (Hash * H, void *cur_key, void *cur_value);
extern void put_INT_k_INT_v (Hash * H, int cur_key, int cur_value);
extern void put_INT_k_STR_v (Hash * H, int cur_key, char * cur_value);

extern int get_INT_k_INT_v(Hash * H, int key);
extern char * get_INT_k_STR_v(Hash * H, int key);

extern void del(Hash * H, int key);
extern void del_INT_k_INT_v(Hash * H, int key);
extern void del_INT_k_STR_v(Hash * H, int key);
extern void resize(Hash * H);

extern void resize_OPEN_INT_k_INT_v(Hash * old_H);
extern void resize_OPEN_INT_k_STR_v(Hash * old_H);

extern unsigned int overwriteKey(Hash * H, int key, int val, int gen_key);
extern void set_lf (Hash * H, double new_load);

extern void insert_int_int(Hash * H, int cur_key, int cur_value);
extern void insert_int_str(Hash * H, int cur_key, char * cur_value);
extern void free_hash(Hash * H);
extern void printHash(Hash * H);
extern double load_factor(Hash * H);

extern void swap_INT_k_INT_v(INT_k_INT_v ** tmp1, INT_k_INT_v ** tmp2);
extern void swap_INT_k_STR_v(INT_k_STR_v ** tmp1, INT_k_STR_v ** tmp2);

// Precomputed hash sizes

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

/*
 * Function: createHash()
 * ----------------------
 *   Creates a hash object and returns it
 *
 *   elements: number of arguments (not including elements)
 *   optional first_arg: starting_size (rounded to power of 2)
 *   optional second_arg: type (OPEN_ADDR / COLLISION)
 *   optional third_arg: key val type (defaults to INT_KEY_INT_VAL)
 *
 *   returns: pointer to hash object
 */

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
    new_hash->probe_limit = index + 1;     

    new_hash->type = type;                        // Collision / open addr
    new_hash->cur_size = starting_size;
    new_hash->num_elem = 0;
    new_hash->load_factor = DEFAULT_LF;           // Default load factor (change at 0.75 = N / size) 
    new_hash->to_resize = resize_default[index];    
    new_hash->k_v_type = type_k_v;

    return new_hash;
}

/*
 * Function: put()
 * ---------------
 *   Adds a key, value pair to the hash (calls the specific function)
 *
 *   H: the hash object to add the kv pair to
 *   cur_key: the address of a key
 *   cur_value: the address of a value
 *
 *   returns: void
 */

void put(Hash * H, void *cur_key, void *cur_value) {

    switch (H->k_v_type) {
        case (1):                 // int key, int val
        {
            int k = *((intptr_t *) cur_key);
            int v = *((intptr_t *) cur_value);
            put_INT_k_INT_v(H, k, v);
            break;
        }
        case (2):                 // int key, str value
        {
            int k = *((intptr_t *) cur_key); 
            char *v = *((char **) cur_value);
            put_INT_k_STR_v(H, k, v);
            break;
        }
    }
}

/*
 * Function: put_INT_k_INT_v()
 * ---------------------------
 *   Adds an int key, int value pair to the hash
 *
 *   H: the hash object to add the kv pair to
 *   cur_key: the address of the int key
 *   cur_value: the address of the int value
 *
 *   returns: void
 */

void put_INT_k_INT_v (Hash * H, int cur_key, int cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));

    // Linear probing once around for a spot  
    INT_k_INT_v * new_node = createINT_k_INT_v(cur_key, cur_value, 0);
    while (1) {
        // Inserting new key
        if (H->int_k_int_v[gen_key] == NULL) {
            H->num_elem++;
            H->int_k_int_v[gen_key] = new_node;
            break;
        // Overwriting key
        } else if (H->int_k_int_v[gen_key]->k == new_node->k || 
            H->int_k_int_v[gen_key]->distance == SHRT_MAX) {

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

/*
 * Function: put_INT_k_STR_v()
 * ---------------------------
 *   Adds an int key, str value pair to the hash
 *
 *   H: the hash object to add the kv pair to
 *   cur_key: the address of the int key
 *   cur_value: the address of the str value
 *
 *   returns: void
 */

void put_INT_k_STR_v (Hash * H, int cur_key, char * cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));

    INT_k_STR_v * new_node = createINT_k_STR_v(cur_key, cur_value, 0);

    while (1) {
        // Inserting new key
        if (H->int_k_str_v[gen_key] == NULL) {
            H->num_elem++;
            H->int_k_str_v[gen_key] = new_node;
            break;
        // Overwriting key
        } else if (H->int_k_str_v[gen_key]->k == new_node->k || 
            H->int_k_str_v[gen_key]->distance == SHRT_MAX) {

            swap_INT_k_STR_v(&H->int_k_str_v[gen_key], &new_node);
            free(new_node->v);
            free(new_node);
            return;
        }
        // Robin hood hash    
        if (H->int_k_str_v[gen_key]->distance < new_node->distance) {
            swap_INT_k_STR_v(&H->int_k_str_v[gen_key], &new_node);
            gen_key = (cur_key & (H->cur_size - 1));
            new_node->distance--;
        }
        gen_key++;
        new_node->distance++;  

        if (gen_key >= H->cur_size) gen_key = 0;
   
        if (new_node->distance >= H->probe_limit) {
            resize_OPEN_INT_k_STR_v(H);
            gen_key = (new_node->k & (H->cur_size - 1));
            new_node->distance = 0;
        }
    }

    if (H->num_elem >= H->to_resize) resize_OPEN_INT_k_STR_v(H);

    return;
}

/*
 * Function: swap_INT_k_INT_v()
 * ----------------------------
 *   Swaps two pointers to int key, int value nodes
 *
 *   tmp1: first node
 *   tmp2: second node
 *
 *   returns: void
 */

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

/*
 * Function: UNUSED
 * ----------------
 */

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

/*
 * Function: printHash()
 * ---------------------
 *   Prints the complete hash including key values and distances
 *
 *   H: hash to print
 *
 *   returns: void
 */
void printHash (Hash * H) 
{

    printf("Open addressing hash\n");

    switch(H->k_v_type) {
        case (1):
            for (int i = 0; i < H->cur_size; i++) {
                printf("[%s%d%s] : ", YELLOW, i, END);
                if (H->int_k_int_v[i] && H->int_k_int_v[i]->distance != SHRT_MAX) {
                    printf("(%d:%d) d=%d\n", H->int_k_int_v[i]->k, H->int_k_int_v[i]->v,
                     H->int_k_int_v[i]->distance);
                } else {
                    printf("\n");
                } 
            }
            break;
        case (2):
            for (int i = 0; i < H->cur_size; i++) {
                printf("[%s%d%s] : ", YELLOW, i, END);
                if (H->int_k_str_v[i] && H->int_k_str_v[i]->distance != SHRT_MAX) {
                    printf("(%d:%s) d=%d\n", H->int_k_str_v[i]->k, H->int_k_str_v[i]->v,
                     H->int_k_str_v[i]->distance);
                } else {
                    printf("\n");
                } 
            }
            break;            
    }
}


/*
 * Function: resize_OPEN_INT_k_INT_v()
 * -----------------------------------
 *   Enlarges the hash by a power of 2, called automatically
 *
 *   old_H: pointer to the old hash
 *
 *   returns: void
 */
void resize_OPEN_INT_k_INT_v(Hash * old_H)
{

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
    *old_H = *new_hash;
    old_H->num_elem = saved;
    free(new_hash);
}


void resize_OPEN_INT_k_STR_v(Hash * old_H)
{
    Hash * new_hash = (Hash *)malloc(SIZE_hash);
    int saved = old_H->num_elem;

    new_hash->probe_limit = old_H->probe_limit + 1;     
    new_hash->type = old_H->type;                        // Collision / open addr
    new_hash->cur_size = old_H->cur_size * 2;
    new_hash->load_factor = DEFAULT_LF;           // Default load factor (change at 0.75 = N / size) 
    new_hash->to_resize = old_H->to_resize * 2;    
    new_hash->k_v_type = old_H->k_v_type;

    new_hash->int_k_str_v = (INT_k_STR_v **)calloc(new_hash->cur_size, SIZE_int_k_str_v); 

    for (int i = 0; i < old_H->cur_size; i++) {
        if (old_H->int_k_str_v[i] && old_H->int_k_str_v[i]->distance != SHRT_MAX) {
            insert_int_str(new_hash, old_H->int_k_str_v[i]->k, old_H->int_k_str_v[i]->v);
            free(old_H->int_k_str_v[i]->v);
            free(old_H->int_k_str_v[i]);
        }
    }

    free(old_H->int_k_str_v);
    *old_H = *new_hash;
    old_H->num_elem = saved;
    free(new_hash);
}

/*
 * Function: insert_int_int()
 * --------------------------
 *   Faster version of put (we don't need to check for deleted nodes)
 *
 *   H: hash to insert element
 *   cur_key: current int key to insert
 *   cur_value: current int value to insert
 *
 *   returns: void
 */
void insert_int_int(Hash * H, int cur_key, int cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));
    // Linear probing once around for a spot  
    INT_k_INT_v * new_node = createINT_k_INT_v(cur_key, cur_value, 0);
    while (1) {
        // Inserting new key
        if (H->int_k_int_v[gen_key] == NULL) {
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
 * Function: insert_int_str()
 * --------------------------
 *   Faster version of put (we don't need to check for deleted nodes)
 *
 *   H: hash to insert element
 *   cur_key: current int key to insert
 *   cur_value: current str value to insert
 *
 *   returns: void
 */

void insert_int_str(Hash * H, int cur_key, char * cur_value) 
{
    int gen_key = (cur_key & (H->cur_size - 1));
    // Linear probing once around for a spot  
    INT_k_STR_v * new_node = createINT_k_STR_v(cur_key, cur_value, 0);
    while (1) {
        // Inserting new key
        if (H->int_k_str_v[gen_key] == NULL) {
            H->num_elem++;
            H->int_k_str_v[gen_key] = new_node;
            break;
        } 
        // Robin hood hash
        if (H->int_k_str_v[gen_key]->distance < new_node->distance) {
            swap_INT_k_STR_v(&H->int_k_str_v[gen_key], &new_node);
            gen_key = (cur_key & (H->cur_size - 1));
            new_node->distance--;
        }

        gen_key++;
        new_node->distance++;  

        if (gen_key >= H->cur_size) gen_key = 0;
    }
}

/*
 * Function: UNUSED
 * ----------------
 */
double load_factor(Hash * H) 
{
    printf("Nodes / size = %d / %d\n", H->num_elem, H->cur_size);
    H->load_factor = (double)H->num_elem / H->cur_size;
    return H->load_factor;
}


/*
 * Function: free_hash()
 * ---------------------
 *   Frees the hash object (completely)
 *
 *   H: hash to free
 *
 *   returns: void
 */
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


/*
 * Function: del()
 * ---------------
 *   Delete a key from a hash (set distance=SHRT_MAX), will free later
 *
 *   H: hash to delete from
 *   key: key to delete
 *
 *   returns: void
 */
void del(Hash * H, int key) 
{

    switch (H->k_v_type) {
        case 1:                 // int key, int val
        {
            del_INT_k_INT_v(H, key);
            break;
        }
        case 2:                 // int key, str value
        {
            del_INT_k_STR_v(H, key); 
            break;
        }
    }       
}

/*
 * Function: del_INT_k_INT_v()
 * ---------------------------
 *   Delete an int kv pair from a hash (set distance=SHRT_MAX), will free later
 *
 *   H: hash to delete from
 *   key: key to delete
 *
 *   returns: void
 */
void del_INT_k_INT_v(Hash * H, int key) 
{ 
    int gen_key = (key & (H->cur_size - 1));
    int dist_from_key = 0;
    // Search for key
    while (1) {
        // Can't free the node, just set distance to shrt_max
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

/*
 * Function: del_INT_k_STR_v()
 * ---------------------------
 *   Delete an int_k str_v pair from a hash (set distance=SHRT_MAX), will free later
 *
 *   H: hash to delete from
 *   key: key to delete
 *
 *   returns: void
 */
void del_INT_k_STR_v(Hash * H, int key) 
{ 
    int gen_key = (key & (H->cur_size - 1));
    int dist_from_key = 0;
    // Search for key
    while (1) {
        // Can't free the node, just set distance to shrt_max
        if (H->int_k_str_v[gen_key] && H->int_k_str_v[gen_key]->k == key) {
            H->int_k_str_v[gen_key]->distance = SHRT_MAX;         // Distance MAX_INT (this is not possible)
            break;
        }
        gen_key++;
        dist_from_key++;

        // Limit for probing exceeded, non existent
        if (H->int_k_str_v[gen_key] == NULL || dist_from_key >= H->probe_limit) break;    
        if (gen_key >= H->cur_size) gen_key = 0;

    }     
}

/*
 * Function: get_INT_k_INT_v()
 * ---------------------------
 *   Specific return function for int keys int vals
 *
 *   H: hash to retrieve from
 *   key: key to retrieve
 *
 *   returns: the corresponding value
 */
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

/*
 * Function: get_INT_k_STR_v()
 * ---------------------------
 *   Specific return function for int keys str vals
 *
 *   H: hash to retrieve from
 *   key: key to retrieve
 *
 *   returns: the corresponding value
 */

char * get_INT_k_STR_v(Hash * H, int key) 
{
    int gen_key = (key & (H->cur_size - 1));
    int dist_from_key = 0;

    while (1) {
        if (H->int_k_str_v[gen_key] && H->int_k_str_v[gen_key]->k == key) {
            char *ret = calloc(strlen(H->int_k_str_v[gen_key]->v) + 1, sizeof(char));
            strncpy(ret, H->int_k_str_v[gen_key]->v, strlen(H->int_k_str_v[gen_key]->v));
            return ret;
        }
        gen_key++;
        dist_from_key++;

        if (gen_key >= H->cur_size) gen_key = 0;
        if (H->int_k_str_v[gen_key] == NULL || dist_from_key >= H->probe_limit) break;     // Non-existent key
    }

    return NULL; 
}

//** Creates a node for open addressing
INT_k_INT_v * createINT_k_INT_v(int cur_key, int cur_value, int dist)
{
    INT_k_INT_v * N = (INT_k_INT_v *)malloc(SIZE_int_k_int_v);
    N->k = cur_key;
    N->v = cur_value;
    N->distance = dist;
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

#endif