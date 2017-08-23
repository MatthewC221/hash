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

// Colours for error and diagnostic prints
#define RED  "\x1B[31m"            
#define GREEN "\e[32m"
#define YELLOW "\e[93m"
#define END  "\e[0m"   

#define POW_SIZE 22

#define COLLISION 1
#define OPEN_ADDR 2

// Pre-defined sizes for optimisations
#define SIZE_kv sizeof(struct INT_kv)
#define SIZE_hash sizeof(struct Hash)
#define SIZE_int_node sizeof(struct int_node)
#define SIZE_int sizeof(int)

#define DEFAULT_LF 0.75

typedef struct int_node
{
    int k;
    int v;
    struct int_node *next; 
    
} int_node;

typedef struct INT_kv
{
    int k;
    int v;
    int distance;

} INT_kv;

typedef struct Hash
{

    // Open addressing and collision nodes
    int_node ** K;   
    INT_kv ** key_value;       
    int cur_size;
    int num_elem;              
    // Size until next resize
    int to_resize;     
    // Max probes until resize     
    int probe_limit;
    double load_factor;     
    int type;               

} Hash;

// Function decl
extern INT_kv * createKV(int cur_key, int cur_value, int dist);
extern int_node * createNode(int cur_key, int cur_value);
extern Hash * createHash(int elements, ...);

extern void put (Hash * H, int cur_key, int cur_value);
extern int get(Hash * H, int key);
extern void del(Hash * H, int key);
extern void resize(Hash * H);
extern void resize_OPEN(Hash * old_H);
extern unsigned int overwriteKey(Hash * H, int key, int val, int gen_key);
extern void set_lf (Hash * H, double new_load);

extern void free_hash(Hash * H);
extern void printHash(Hash * H);
extern double load_factor(Hash * H);

extern void swap(INT_kv ** tmp1, INT_kv ** tmp2);

// Currently unused
extern Hash * copyHashCollision(Hash * H);
extern INT_kv ** copyHashOpen(Hash * H);
extern void swap_Hash(Hash ** tmp1, Hash ** tmp2);
// Function decl

#endif