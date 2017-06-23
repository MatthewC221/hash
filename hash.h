#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#ifndef _hash_h
#define _hash_h
#define RED  "\x1B[31m"            // For start RED
#define GREEN "\e[32m"
#define YELLOW "\e[93m"
#define END  "\e[0m"                // For end colours

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
    /* Optionals
    double load factor;
    
    
    */
} Hash;

int_node * createNode(int cur_key, int cur_value);

// Incorporate type later
// Allow for open addressing/chaining option
// ** Creates the hash
Hash * createHash(int starting_size) 
{

    if (!starting_size) {
        fprintf(stderr, "%shash err: invalid starting_size\n"); 
        return NULL; 
    } else if (!isPrime(starting_size)) {
        starting_size = nextPrime(starting_size);
        printf("Resizing to prime number: %d\n", starting_size);
    }
    
    printf("%sHash%s created: init size = %s%d%s\n", RED, END, GREEN, starting_size, END);
    
    Hash * new_hash = malloc(sizeof(struct Hash));
    new_hash->K = malloc(sizeof(starting_size) * sizeof(struct int_node));
    new_hash->cur_size = starting_size;
    
    for (int i = 0; i < starting_size; i++) {
        new_hash->K[i] = NULL;
    }
    
    return new_hash;
}

//** Insert element in hash
void put (Hash * H, int cur_key, int cur_value) 
{
    int gen_key = 0;
    
    if (cur_key != 0) {
        int size = H->cur_size;
        gen_key = cur_key % size;           // Generated key
    }
    
    int_node * new_N = createNode(cur_key, cur_value);
    
    int_node * tmp = H->K[gen_key]; 
    
    if (tmp == NULL) {
        H->K[gen_key] = new_N;
        new_N->next = NULL;
    } else {
        
        if (tmp->next == NULL && tmp->k == cur_key) {
            tmp->v = cur_value;
            printf("Overriden hash value %s%d%s at index %s%d%s\n", YELLOW, cur_value, END, YELLOW, gen_key, END);
            return;
        }
    
        while (tmp->next != NULL) {
            if (tmp->k == cur_key) {
                tmp->v = cur_value;
                printf("Overriden hash value %s%d%s at index %s%d%s\n", YELLOW, cur_value, END, YELLOW, gen_key, END);
                return;
            } 
            tmp = tmp->next;
        }
        tmp->next = new_N;
        new_N->next = NULL;
    }
        
    return;
}

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

int_node * createNode(int cur_key, int cur_value)
{
    int_node * N = malloc(sizeof(struct int_node));
    N->k = cur_key;
    N->v = cur_value;
    N->next = NULL;
    
    return N;   
}

int nextPrime(int num)
{

    for (int i = num + 1; ; i++) {
        if (isPrime(i)) {
            return i;
        }
    }
    
    return -1;
}

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






