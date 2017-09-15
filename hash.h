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
#include <inttypes.h>
#include <math.h>

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

    // Open addressing and collision nodes  
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

extern int get(Hash * H, void * key);
extern int get_INT_k_INT_v(Hash * H, int key);
extern int get_INT_k_STR_v(Hash * H, int key);

extern void del(Hash * H, int key);
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

#endif