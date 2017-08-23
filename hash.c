
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

int powers[22] = {
    2, 4, 8, 16, 32,
    64, 128, 256, 512, 1024,
    2048, 4096, 8192, 16384, 32768,
    65536, 131072, 262144, 524288, 1048576,
    2097152, 4194304
};

int log_prime[22] = {
    1, 2, 3, 4, 5,
    6, 7, 8, 9, 10,
    11, 12, 13, 14, 15,
    16, 17, 18, 19, 20,
    21, 22
};

int resize_default[22] = {
    1, 3, 6, 12, 24, 48, 
    96, 192, 384, 768, 1536, 
    3072, 6144, 12288, 24576, 49152, 
    98304, 196608, 393216, 786432, 1572864, 
};

void set_lf (Hash * H, double new_load)
{
    if (new_load) {
        H->load_factor = new_load;
        H->to_resize = new_load * H->cur_size;
    }
}

/* Incorporate type later
// Allow for open addressing/chaining option
// ** Creates the hash
// @args: type = open addr vs collision, optional starting_size
*/
Hash * createHash(int elements, ...) 
{
    va_list arg_list;
    int starting_size = 8;     // Default size
    int type = OPEN_ADDR;       // Default type
    
    if (elements > 2) {
        fprintf(stderr, "<Hash.h>: supplied too many parameters to <Hash.h>: createHash(): max 2\n");
        return NULL;
    }
    
    va_start (arg_list, elements);  
    for (int i = 0; i < elements; i++) {
        if (i == 0) {
            starting_size = va_arg(arg_list, int);
        } else {
            type = va_arg(arg_list, int);
            assert(type == 1 || type == 2);
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
    // if (new_hash == NULL) fprintf(stderr, "Failed to malloc hash.c: line 93\n"); exit(0);
    
    if (type == COLLISION) {        // Collision hash
        new_hash->K = (int_node **)malloc(starting_size * sizeof(struct int_node));
        for (int i = 0; i < starting_size; i++) {
            new_hash->K[i] = NULL;
        }
    } else {                        // Open address hash
        new_hash->key_value = (k_v **)calloc(starting_size, SIZE_kv);
        // if (!new_hash->key_value) fprintf(stderr, "Failed to create memory from malloc\n"); exit(0);
        new_hash->probe_limit = index + 1;      // log2(size) = index + 1
    }
    new_hash->type = type;                        // Collision / open addr
    new_hash->cur_size = starting_size;
    new_hash->num_elem = 0;
    new_hash->load_factor = DEFAULT_LF;           // Default load factor (change at 0.75 = N / size) 
    new_hash->to_resize = resize_default[index];             
    return new_hash;
}

//** Insert element in hash

// Currently resize AFTER we insert a new node
void put (Hash * H, int cur_key, int cur_value) 
{
    int gen_key = cur_key;
    if (gen_key < 0) gen_key *= -1; 
    if (H->cur_size <= gen_key) gen_key = (cur_key & (H->cur_size - 1));

    // #### Collision put ####
    if (H->type == COLLISION) {
        int_node * tmp = H->K[gen_key]; 
        int_node * new_N = createNode(cur_key, cur_value);     
        // Empty LL
        if (tmp == NULL) {
            H->K[gen_key] = new_N;
            H->num_elem++;       
        } else {   
            int_node * before = H->K[gen_key];
            while (tmp != NULL) {
                if (tmp->k == cur_key) {
                    tmp->v = cur_value;
                    free(new_N);
                    return;
                } 
                before = tmp;
                tmp = tmp->next;
            }              
            // If no conflicts insert at tail
            before->next = new_N;
            H->num_elem++;
        }
    // #### Open addressing put ####
    } else if (H->type == OPEN_ADDR) {
        // Linear probing once around for a spot  
        k_v * new_node = createKV(cur_key, cur_value, 0);
        while (1) {
            // Inserting new key
            if (H->key_value[gen_key] == NULL || (H->key_value[gen_key]->k == INT_MIN)) {
                H->num_elem++;
                H->key_value[gen_key] = new_node;
                break;
            // Overwriting key
            } else if (H->key_value[gen_key]->k == new_node->k) {
                swap(&H->key_value[gen_key], &new_node);
                free(new_node);
                return;
            }
            // Robin hood hash
            // If the distance of the current key has probed less, swap and insert the curr key
            // now that the new key is inserted       
            if (H->key_value[gen_key]->distance < new_node->distance) {
                swap(&H->key_value[gen_key], &new_node);

                gen_key = cur_key;
                if (gen_key < 0) gen_key *= -1;
                gen_key = (gen_key & (H->cur_size - 1));
                // Don't increment distance until next check
                new_node->distance--;
            }
            gen_key++;
            new_node->distance++;  

            if (gen_key >= H->cur_size) gen_key = 0;
            // If we reach the probe limit, resize the hash     
            if (new_node->distance >= H->probe_limit) {
                resize_OPEN(H);
                gen_key = new_node->k;
                if (gen_key < 0) gen_key *= -1; 
                gen_key = (gen_key & (H->cur_size - 1));
            }
        }
    }

    if (H->num_elem >= H->to_resize) {
        if (H->type == OPEN_ADDR) {
            resize_OPEN(H);
        } else {
            resize(H);
        }
    }

    return;
}


void swap(k_v ** tmp1, k_v ** tmp2) 
{
    k_v * tmp = *tmp1; 
    *tmp1 = *tmp2;
    *tmp2 = tmp;
}


//** Attempts an overwrite, if no key present return 0
unsigned int overwriteKey(Hash * H, int key, int val, int gen_key)
{

    unsigned int count = 0;
    for (; count < H->probe_limit; count++, gen_key++) {
        if (H->key_value[gen_key] && H->key_value[gen_key]->k == key) {
            H->key_value[gen_key]->v = val;
            return 1;
        } else if (H->key_value[gen_key] == NULL) {
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
    printf("    Hash\n");

    if (H->type == COLLISION) {
        printf("Collision hash\n");
        for (int i = 0; i < H->cur_size; i++) {
            printf("[%s%d%s] : ", YELLOW, i, END);       
            int_node * tmp = H->K[i];
            
            if (tmp != NULL) {        
                while (tmp->next != NULL) {
                    printf("(%d:%d)->", tmp->k, tmp->v);
                    tmp = tmp->next;
                }
                printf("(%d:%d)", tmp->k, tmp->v);
            }
            printf("\n");
        }
        
    } else if (H->type == OPEN_ADDR) {
        printf("Open addressing hash\n");
        for (int i = 0; i < H->cur_size; i++) {
            printf("[%s%d%s] : ", YELLOW, i, END);
            if (H->key_value[i] && H->key_value[i]->k != INT_MIN) {
                printf("(%d:%d) d=%d\n", H->key_value[i]->k, H->key_value[i]->v, H->key_value[i]->distance);
            } else {
                printf("\n");
            } 
        }
    }
}

// (TODO) OPTIMISE THE RESIZE! This will be much faster...
void resize_OPEN(Hash * old_H)
{

    Hash * new_H = createHash(2, 2 * old_H ->cur_size, 2);

    for (int i = 0; i < old_H->cur_size; i++) {
        if (old_H->key_value[i] && old_H->key_value[i]->k != INT_MIN) {
            int k = old_H->key_value[i]->k;
            int v = old_H->key_value[i]->v;
            put(new_H, k, v);
            free(old_H->key_value[i]);
        }
    }

    free(old_H->key_value);
    *old_H = *new_H;
    free(new_H);
}

//** Resize the hash, copy over using different indexes?
void resize (Hash * H) 
{    
    
    int new_size = -1;
    int tmp = H->cur_size * 2;
    int index = 0;

    for (; index < POW_SIZE; index++) {
        if (powers[index] >= tmp) {
            new_size = powers[index];
            break;
        }
    }

    int old_size = H->cur_size;  
    int old_elem = H->num_elem;

    Hash * copy_H = NULL;
    k_v ** copy = NULL;

    // FASTER FOR MANY COLLISIONS, copyHashOpen, faster for not many collisions, current int *

    if (H->type == COLLISION) {
        copy_H = copyHashCollision(H);
    } 

    H->num_elem = 0;
    H->cur_size = new_size;
    H->to_resize = resize_default[index];
        
    if (H->type == COLLISION) {          
        // Free the nodes first
        for (int i = 0; i < old_size; i++) {
            int_node * del = H->K[i];
            
            while (del != NULL) {
                int_node *tmp = del;
                del = del->next;
                free(tmp);
            }
            H->K[i] = NULL;
        }
        
        H->K = (int_node **)realloc(H->K, sizeof(struct int_node) * new_size);
        
        // Initialise the rest of the nodes to NULL
        for (int i = old_size; i < new_size; i++) H->K[i] = NULL;
        
        for (int i = 0; i < copy_H->cur_size; i++) {
            int_node * tmp = copy_H->K[i];
            
            while (tmp != NULL) {
                int key = tmp->k;
                int val = tmp->v;
                put(H, key, val);
                tmp = tmp->next;
            }          
        }
        free_hash(copy_H);

    } else if (H->type == OPEN_ADDR) {
        // Free the nodes first

        //int *keys = (int *)malloc(SIZE_int * old_elem);
        //int *vals = (int *)malloc(SIZE_int * old_elem);

        Hash * new_H = createHash(2, powers[index], 2);

        // H->key_value = (k_v **)realloc(H->key_value, SIZE_kv * new_size);

        // if (H->key_value == NULL) exit(0);
        int count = 0;
        for (int i = 0; i < old_size; i++) {
            if (H->key_value[i] && H->key_value[i]->k != INT_MIN) {
                int key = H->key_value[i]->k;
                int val = H->key_value[i]->v;
                put(new_H, key, val);
                free(H->key_value[i]);
            }
            H->key_value[i] = NULL;
        }
        free(H->key_value);

        // TODO!!
        *H = *new_H;
        H->probe_limit++;
    }
    
    return;
}

void swap_Hash(Hash ** tmp1, Hash ** tmp2) 
{
    Hash* tmp = *tmp1; 
    *tmp1 = *tmp2;
    *tmp2 = tmp;
}


//** Works with resize, requires a deep copy of original Hash so our original can realloc
Hash * copyHashCollision(Hash * H)
{

    Hash * new_H = createHash(2, H->cur_size, 1);
    new_H->num_elem = INT_MIN;                      // So we won't go into a resizing loop

    for (int i = 0; i < H->cur_size; i++) {
        int_node * tmp = H->K[i];
        int_node * cur;               // Keep track of current
        while (tmp != NULL) {
            int key = tmp->k;
            int val = tmp->v;        
            int_node * new_node = createNode(key, val);
            // Instead of calling put, copy LL here
            // Saves ~0.07 seconds
            if (new_H->K[i] == NULL) {
                new_H->K[i] = new_node;
                cur = new_H->K[i];
            } else {
                cur->next = new_node;
                cur = new_node;
            }
            tmp = tmp->next;
        }          
    }    
    
    return new_H;
}

//** Works with resize, copies open_addr hash
k_v ** copyHashOpen(Hash * H) 
{
    // Hash * new_H = createHash(2, H->cur_size, 2);

    k_v ** copy = (k_v **)malloc(SIZE_kv * H->num_elem);

    int count = 0;

    for (int i = 0; i < H->cur_size; i++) {
        // Requires a deep copy
        if (H->key_value[i] && H->key_value[i]->k != INT_MIN) {
            k_v * node = createKV(H->key_value[i]->k, H->key_value[i]->v, 0);
            copy[count++] = node;
        } 
    }

    return copy;
}

//** Calculates load factor
double load_factor(Hash * H) 
{
    printf("Nodes / size = %d / %d\n", H->num_elem, H->cur_size);
    H->load_factor = (double)H->num_elem / H->cur_size;
    return H->load_factor;
}


//** Frees entire hash
void free_hash (Hash * H) 
{
    if (H->type == COLLISION) {
        for (int i = 0; i < H->cur_size; i++) {
            int_node *del = H->K[i];
            while (del != NULL) {
                int_node *tmp = del;
                del = del->next;
                free(tmp);
            } 
            H->K[i] = NULL;
        }
        free(H->K);
        H->K = NULL;
        free(H);

    } else if (H->type == OPEN_ADDR) {
        for (int i = 0; i < H->cur_size; i++) {
            if (H->key_value[i]) {
                free(H->key_value[i]);
            }
        }
        free(H->key_value);
        H->key_value = NULL;
        free(H);
    }
}


//** Frees a node, if that node doesn't exist print err
void del(Hash * H, int key) 
{
    int gen_key = 0;
    
    // Don't mod by 0
    if (key < 0) key = key * -1;
    gen_key = (key & (H->cur_size - 1));
    
    if (H->type == COLLISION) {
        int_node * del_before = H->K[gen_key]; 
        int_node * del_after = H->K[gen_key];
        if (del_after == NULL) {
            fprintf(stderr, "<Hash.h>: illegal removal of non-existent key\n");
            return;
        } else {
            bool exists = false;
            while (del_after->next != NULL) {       
                if (del_after->k == key) {                       // After finding the node

                    // Case 1: Deleting the head (either a single head or a LL)
                    if (del_after == H->K[gen_key]) {          
                        if (del_after->next == NULL) {
                            H->K[gen_key] = NULL;
                        } else {                                                                    
                            H->K[gen_key] = del_after->next;
                        }
                    // Case 2: If node was in the middle of LL
                    } else if (del_after->next != NULL) {
                        int_node * new_next = del_after->next;
                        del_before->next = new_next;
                    // Case 3: If node was at the end
                    } else if (del_after->next == NULL) {
                        del_before->next = NULL;
                    }
                    exists = true;
                    free(del_after);
                    break;
                }
                del_before = del_after;
                del_after = del_after->next;
            }

            // If the last node
            if (!exists && del_after->k == key) {
                if (del_after == H->K[gen_key]) {
                    H->K[gen_key] = NULL;
                } else {
                    del_before->next = NULL;
                }
                free(del_after);
            } else if (!exists) {
                fprintf(stderr, "<Hash.h>: illegal removal of non-existent key\n");
            }
        }
    } else if (H->type == OPEN_ADDR) {
        int saved = gen_key;
        int dist_from_key = 0;
        // Search for key
        while (1) {
            // Can't free the node, just set it to a marker (INT_MIN)
            if (H->key_value[gen_key] && H->key_value[gen_key]->k == key) {
                H->key_value[gen_key]->k = INT_MIN;
                H->key_value[gen_key]->v = INT_MIN;
                break;
            }
            gen_key++;
            dist_from_key++;

            // Limit for probing exceeded, non existent
            if (H->key_value[gen_key] == NULL || dist_from_key >= H->probe_limit) break;    
            if (gen_key >= H->cur_size) gen_key = 0;

        }        
    }
}

//** Get value from key
int get(Hash * H, int key) 
{
    int gen_key = 0;

    // Turn negative keys positive
    if (key < 0) key *= -1;
    gen_key = (key & (H->cur_size - 1));

    if (H->type == COLLISION) {
        int_node * lookup = H->K[gen_key];
        while (lookup) {
            if (lookup->k == key) {
                return lookup->v;
            }
            lookup = lookup->next;
        }
    } else if (H->type == OPEN_ADDR) {
        int saved = gen_key;
        int dist_from_key = 0;

        while (1) {
            if (H->key_value[gen_key] && H->key_value[gen_key]->k == key) {
                return H->key_value[gen_key]->v;
            }
            gen_key++;
            dist_from_key++;

            if (gen_key >= H->cur_size) gen_key = 0;
            if (H->key_value[gen_key] == NULL || dist_from_key >= H->probe_limit) break;     // Non-existent key
        }
    }    // fprintf(stderr, "<Hash.h>: illegal key lookup of non-existent or deleted key %d\n", key);
    return INT_MIN;
}

//** Creates a node for collision handling
int_node * createNode(int cur_key, int cur_value)
{
    int_node * N = (int_node *)malloc(sizeof(struct int_node));
    N->k = cur_key;
    N->v = cur_value;
    N->next = NULL;

    return N;   
}

//** Creates a node for open addressing
k_v * createKV(int cur_key, int cur_value, int dist)
{
    k_v * N = (k_v *)malloc(SIZE_kv);
    N->k = cur_key;
    N->v = cur_value;
    N->distance = dist;
    // if (!N) fprintf(stderr, "Malloc failed hash.c: 605\n"); exit(0);

    return N;
}

//** Finds next prime during resizing

/*
int nextPrime(int num)
{
    for (int i = num + 1; ; i++) {
        if (isPrime(i)) {
            return i;
        }
    }
    
    return -1;
}

//** Determines if prime
int isPrime(int num)
{
    if (num == 1) return 1;

    int upper_bound = sqrt(num);
    for (int div = 2; div <= upper_bound; div++) {
        if (num % div == 0) {
            return 0;
        }
    }
    
    return 1;
}
*/