#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>
#include <assert.h>

#ifndef _hash_h
#define _hash_h
#define RED  "\x1B[31m"            // For start RED
#define GREEN "\e[32m"
#define YELLOW "\e[93m"
#define END  "\e[0m"                // For end colours

#define COLLISION 1
#define OPEN_ADDR 2

// Allow string hash as well
// Hash * createHash(int size);
int isPrime(int num);
int nextPrime(int num);

typedef struct int_node
{
    int k;
    int v;
    struct int_node *next; 
    
} int_node;

typedef struct k_v
{
    int k;
    int v;

} k_v;

typedef struct Hash
{

    // For collisions
    int_node ** K;
    int cur_size;
    int num_elem;   
    
    // For key value pairs
    k_v ** key_value;

    // Number of elements until resize            
    int to_resize;
    
    // The load factor before resizing (num_elem / cur_size)
    // Can be > 1 for large chains
    
    double load_factor;   
    int type;      

} Hash;



// Function decl
k_v * createKV(int cur_key, int cur_value);
int_node * createNode(int cur_key, int cur_value);
void resize(Hash * H);
Hash * copyHash(Hash * H);
void free_hash(Hash * H);
void del(Hash * H, int key);
void printHash(Hash * H);
// Function decl

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
    
    // Default size
    int starting_size = 17;
    
    // Default collision
    int type = COLLISION;               
    
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
        }
    }   

    if (!starting_size) {
        fprintf(stderr, "hash err: invalid starting_size\n"); 
        return NULL; 
    } else if (!isPrime(starting_size)) {
        starting_size = nextPrime(starting_size);
        printf("Resizing to prime number: %d\n", starting_size);
    }
    
    // printf("%sHash%s created: init size = %s%d%s\n", RED, END, GREEN, starting_size, END);
    
    Hash * new_hash = (Hash *)malloc(sizeof(struct Hash));
    
    // For collision, create the array of LL
    if (type == COLLISION) {
        new_hash->K = (int_node **)malloc(starting_size * sizeof(struct int_node));
        for (int i = 0; i < starting_size; i++) {
            new_hash->K[i] = NULL;
        }
    // For open addr, create array of tuples
    } else {
        new_hash->key_value = (k_v **)malloc(starting_size * sizeof(struct k_v));
        for (int i = 0; i < starting_size; i++) {
            new_hash->key_value[i] = NULL;
        }
    }

    new_hash->type = type;                  // Collision / open addr
    new_hash->cur_size = starting_size;
    new_hash->num_elem = 0;
    new_hash->load_factor = 0.75;           // Default load factor (change at 0.75 = N / size)
    
    new_hash->to_resize = new_hash->cur_size * new_hash->load_factor;
    
    return new_hash;
}

//** Insert element in hash

// Currently resize AFTER we insert a new node
void put (Hash * H, int cur_key, int cur_value) 
{
    int gen_key = 0;
    
    if (cur_key < 0) cur_key = cur_key * -1;
    if (cur_key) gen_key = (cur_key - (cur_key / H->cur_size) * H->cur_size); 

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

            if (H->num_elem >= H->to_resize) resize(H); 
        }

    // #### Open addressing put ####
    } else if (H->type == OPEN_ADDR) {
        k_v * new_N = createKV(cur_key, cur_value);
        // Linear probing once around for a spot
        int saved = gen_key;     
        while (1) {
            // Inserting new key
            if (!H->key_value[gen_key] || (H->key_value[gen_key]->k == INT_MIN)) {
                H->num_elem++;
                H->key_value[gen_key] = new_N;
                break;
            // Overwriting key
            } else if (H->key_value[gen_key] && (H->key_value[gen_key]->k == cur_key)) {
                H->key_value[gen_key]->v = cur_value;
                break;
            }
            gen_key++;

            if (gen_key >= H->cur_size) gen_key = 0;
            // If out of space
            if (gen_key == saved) {
                printHash(H);
                printf("Need to resize\n");
                exit(0);                
            }

        }

    }


    return;
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
                printf("(%d:%d)\n", H->key_value[i]->k, H->key_value[i]->v);
            } else {
                printf("\n");
            } 
        }
    }
}

//** Resize the hash, copy over using different indexes?
void resize (Hash * H) 
{    
    // Resize to two times as large
    int new_size = nextPrime(H->cur_size * 2);
    
    /*
    printf("----------\nNew hash size = %d\n", new_size);
    printf("Current %sLF%s = %s%.2f%s\n", YELLOW, END, YELLOW, H->load_factor, END);
    printf("Nodes / Size = %s%d%s / %s%d%s\n----------\n", RED, H->num_elem, END, GREEN, H->cur_size, END);
    */
        
    Hash * copy_H = copyHash(H);        // Use this to copy over original H
    int saved = H->cur_size;
    
    // Have to free the nodes first?
    for (int i = 0; i < H->cur_size; i++) {
        int_node * del = H->K[i];
        
        while (del != NULL) {
            int_node *tmp = del;
            del = del->next;
            free(tmp);
        }
        H->K[i] = NULL;
    }
    
    H->K = (int_node **)realloc(H->K, sizeof(struct int_node) * new_size);
    H->cur_size = new_size;
    H->to_resize = new_size * H->load_factor;    
    H->num_elem = 0;
    
    // Initialise the rest of the nodes to NULL
    for (int i = saved; i < H->cur_size; i++) H->K[i] = NULL;
    
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
    
    return;
}

//** Works with resize, requires a deep copy of original Hash so our original can realloc
Hash * copyHash(Hash * H)
{

    Hash * new_H = createHash(1, H->cur_size);
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
            //put(new_H, key, val);
            tmp = tmp->next;
        }          
    }    
    
    return new_H;
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
    if (key) gen_key = (key - (key / H->cur_size) * H->cur_size);
    
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
        // Search for key
        while (1) {
            // Can't free the node, just set it to a marker (INT_MIN)
            if (H->key_value[gen_key] && H->key_value[gen_key]->k == key) {
                H->key_value[gen_key]->k = INT_MIN;
                H->key_value[gen_key]->v = INT_MIN;
                break;
            }
            gen_key++;
            if (gen_key == saved || H->key_value[gen_key] == NULL) break;    
            if (gen_key >= H->cur_size) gen_key = 0;

        }        
    }
}

//** Get value from key
int get(Hash * H, int key) 
{
    int gen_key = 0;

    // Turn negative keys positive
    if (key < 0) key = key * -1;
    if (key) gen_key = (key - (key / H->cur_size) * H->cur_size);

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
        while (1) {
            if (H->key_value[gen_key] && H->key_value[gen_key]->k == key) {
                return H->key_value[gen_key]->v;
            }
            gen_key++;
            if (gen_key >= H->cur_size) gen_key = 0;
            if (gen_key == saved || H->key_value[gen_key] == NULL) break;     // Non-existent key
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
k_v * createKV(int cur_key, int cur_value)
{
    k_v * N = (k_v *)malloc(sizeof(struct k_v));
    N->k = cur_key;
    N->v = cur_value;

    return N;
}

//** Finds next prime during resizing
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



#endif






