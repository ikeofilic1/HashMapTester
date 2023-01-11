#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdlib.h>
#include <string.h>

#define DEFAULT_LF 0.75

typedef void (*free_fn)(void *);

struct _hash_table
{
    // The entire table data
    void **data;

    // actual capacity of the hash_table
    int capacity;

    // cur size
    int size;

    // load factor
    float LF;

    // set a free function for your data
    free_fn free_fn;

    // will also have a rehash function for each hash table
    //
};

typedef struct _hash_table *hash_table;

/* typedef struct table_node *hash_table;*/
// hash_table new_table(free_fn);

hash_table new_table(free_fn, int size, float lf);

int base_hash  (char *key, int M, int base);
int binary_hash(char *key, int M);
int length_hash(char *key, int M);
int prime_hash (char *key, int M);
int ascii_hash (char *key, int M);
int adler_hash (void *key, int M);
 
// rehash should put in the value and return the number of collisions (for testing purposes only)
int linear_probe (int *,   int,   hash_table);
int quad_rehash  (int *,   int,   hash_table);
int plus_3       (int *hv, int M, hash_table table);
int plus_5       (int *hv, int M, hash_table table);
int plus_n_rehash(int *hv, int M, hash_table table, int N);

void free_table(hash_table);

void put   (hash_table table, void *key,   void *stuff);
void put_at(hash_table table, int index, void *key, void *stuff);

// void* get(hash_table table, void *key);
// bool contains(hash_table table, void *key);
// void resize(hash_table table);


#endif
