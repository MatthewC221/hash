#ifndef _hash_h
#define _hash_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

#define RED  "\x1B[31m"            // For start RED
#define GREEN "\e[32m"
#define YELLOW "\e[93m"
#define END  "\e[0m"                // For end colours
#define PRIME 20

#define COLLISION 1
#define OPEN_ADDR 2

#define SIZE_kv sizeof(struct k_v)
#define SIZE_hash sizeof(struct Hash)
#define SIZE_int_node sizeof(struct int_node)
#define SIZE_int sizeof(int)

#define DEFAULT_LF 0.75

// Allow string hash as well
// Hash * createHash(int size);

/*
int isPrime(int num);
int nextPrime(int num);
*/

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
    int distance;

} k_v;

typedef struct Hash
{

    int_node ** K;          // Collisions
    int cur_size;
    int num_elem;   
    
    k_v ** key_value;       // Open addressing
           
    int to_resize;          // Elements to resize
    
    int probe_limit;
    double load_factor;     
    int type;               // OPEN_ADDR or COLLISION

} Hash;

// Function decl
extern k_v * createKV(int cur_key, int cur_value, int dist);
extern int_node * createNode(int cur_key, int cur_value);
extern void resize(Hash * H);
extern Hash * copyHashCollision(Hash * H);
extern void swap_Hash(Hash ** tmp1, Hash ** tmp2);

extern Hash * resize_OPEN(Hash * old_H);
extern k_v ** copyHashOpen(Hash * H);
extern void free_hash(Hash * H);
extern void del(Hash * H, int key);
extern void printHash(Hash * H);
extern void swap(k_v ** tmp1, k_v ** tmp2);
extern unsigned int overwriteKey(Hash * H, int key, int val, int gen_key);
extern Hash * createHash(int elements, ...);
extern void put (Hash * H, int cur_key, int cur_value);
extern int get(Hash * H, int key);
extern double load_factor(Hash * H);
// Function decl

#endif