#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>

#ifndef _hash_h
#define _hash_h
#define RED  "\x1B[31m"            // For start RED
#define GREEN "\e[32m"
#define YELLOW "\e[93m"
#define END  "\e[0m"                // For end colours

#define COLLISION 1

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

typedef struct Hash
{
    int_node ** K;
    int cur_size;
    int num_elem;   
    
    // Number of elements until resize            
    int to_resize;
    
    // The load factor before resizing (num_elem / cur_size)
    // Can be > 1 for large chains
    
    double load_factor;         

} Hash;

// Function decl
int_node * createNode(int cur_key, int cur_value);
void resize(Hash * H);
Hash * copyHash(Hash * H);
void free_hash(Hash * H);
// Function decl

void set_lf (Hash * H, double new_load)
{
    if (new_load) H->load_factor = new_load;
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
        fprintf(stderr, "%shash err: invalid starting_size\n"); 
        return NULL; 
    } else if (!isPrime(starting_size)) {
        starting_size = nextPrime(starting_size);
        printf("Resizing to prime number: %d\n", starting_size);
    }
    
    printf("%sHash%s created: init size = %s%d%s\n", RED, END, GREEN, starting_size, END);
    
    Hash * new_hash = malloc(sizeof(struct Hash));
    
    new_hash->K = malloc(starting_size * sizeof(struct int_node));
    new_hash->cur_size = starting_size;
    new_hash->num_elem = 0;
    new_hash->load_factor = 0.75;           // Default load factor (change at 0.75 = N / size)
    
    new_hash->to_resize = new_hash->cur_size * new_hash->load_factor;
    
    for (int i = 0; i < starting_size; i++) {
        new_hash->K[i] = NULL;
    }
    
    return new_hash;
}

//** Insert element in hash

// Currently resize AFTER we insert a new node
void put (Hash * H, int cur_key, int cur_value) 
{
    int gen_key = 0;
    
    if (cur_key != 0) {
        int size = H->cur_size;
        gen_key = cur_key % size;           // Generated key
    }
    
    int_node * new_N = createNode(cur_key, cur_value);
    int_node * tmp = H->K[gen_key]; 
    
    // Empty LL
    if (tmp == NULL) {
        H->K[gen_key] = new_N;
        H->num_elem++;
        new_N->next = NULL;
        // if (H->num_elem >= H->to_resize) resize(H);
        
    } else {
        
        // Key to override at head
        if (tmp->next == NULL && tmp->k == cur_key) {
            tmp->v = cur_value;
            printf("Overriden hash value %s%d%s at index %s%d%s\n", YELLOW, cur_value, END, YELLOW, gen_key, END);
            free(new_N);
            return;
        }
        
        // Key to override in middle
        while (tmp->next != NULL) {
            if (tmp->k == cur_key) {
                tmp->v = cur_value;
                printf("Overriden hash value %s%d%s at index %s%d%s\n", YELLOW, cur_value, END, YELLOW, gen_key, END);
                free(new_N);
                return;
            } 
            tmp = tmp->next;
        }
        
        // Key to override at very end
        if (tmp->k == cur_key) {
            tmp->v = cur_value;
            printf("Overriden hash value %s%d%s at index %s%d%s\n", YELLOW, cur_value, END, YELLOW, gen_key, END);
            free(new_N);
            return;
        } 
            
        // If no conflicts insert at tail
        tmp->next = new_N;
        H->num_elem++;
        new_N->next = NULL;
        if (H->num_elem >= H->to_resize) resize(H);
    }
    
    // Resize if load f = 0.75

    return;
}

//** Prints all indexes and elements of the hash
void printHash (Hash * H) 
{
    printf("    Hash\n");
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
}

//** Resize the hash, copy over using different indexes?
void resize (Hash * H) 
{    
    // Resize to two times as large
    int new_size = nextPrime(H->cur_size * 2);
    
    printf("Resizing... to %d\n", new_size);
        
    Hash * copy_H = copyHash(H);        // Use this to copy over original H
    printf("Hey\n");
    
    H->K = realloc(H->K, sizeof(struct int_node) * new_size);
    H->cur_size = new_size;
    H->to_resize = new_size * H->load_factor;

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
    
    printHash(H);
    return;
}

//** Works with resize, requires a deep copy of original Hash so our original can realloc
Hash * copyHash(Hash * H)
{

    Hash * new_H = createHash(1, H->cur_size);
    new_H->num_elem = INT_MIN;                      // So we won't go into a resizing loop

    for (int i = 0; i < H->cur_size; i++) {
        int_node * tmp = H->K[i];
        
        while (tmp != NULL) {
            int key = tmp->k;
            int val = tmp->v;
            put(new_H, key, val);
            tmp = tmp->next;
        }          
    }    
    printf("success!\n");
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

    printf("Deleting 0->%d\n", H->cur_size);
    for (int i = 0; i < H->cur_size; i++) {
        int_node *del = H->K[i];
        
        while (del != NULL) {
            int_node *tmp = del;
            del = del->next;
            free(tmp);
        }
        
        H->K[i] = NULL;
    }
    // H->K = NULL;
    free(H->K);
    H->K = NULL;
    free(H);
}


//** Frees a node, if that node doesn't exist print err
void free_node(Hash * H, int key) 
{
    int gen_key = 0;
    
    // Don't mod by 0
    if (key) gen_key = key % H->cur_size;
    
    int_node * del_before = H->K[gen_key]; 
    int_node * del_after = H->K[gen_key];
    
    // printf("The generated key = %d\n", gen_key);
    
    if (del_after == NULL) {
        fprintf(stderr, "<Hash.h>: illegal removal of non-existent key\n");
        return;
    } else {
    
        bool exists = false;
        while (del_after->next != NULL) {       
            if (del_after->k == key) {
                if (del_after == H->K[gen_key]) {            // Deleting the head of len = 1
                    if (del_after->next == NULL) {
                        H->K[gen_key] = NULL;
                    } else {                                                                    
                        H->K[gen_key] = del_after->next;
                    }
                } else if (del_after->next != NULL) {
                    int_node * new_next = del_after->next;
                    del_before->next = new_next;
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

}

int get(Hash * H, int key) 
{
    int gen_key = 0;
    if (key) gen_key = key % H->cur_size;
    
    int_node * lookup = H->K[gen_key];
    
    while (lookup) {
        if (lookup->k == key) {
            return lookup->v;
        }
        lookup = lookup->next;
    }
    
    // Returns here
    fprintf(stderr, "<Hash.h>: illegal key lookup of non-existent or deleted key %d\n", key);
    return INT_MIN;
}

//** Creates a node
int_node * createNode(int cur_key, int cur_value)
{

    int_node * N = malloc(sizeof(struct int_node));

    N->k = cur_key;
    N->v = cur_value;
    
    N->next = NULL;
    
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






