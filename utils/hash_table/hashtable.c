#include "hashtable.h"

int base_hash(char *key, int M, int base)
{
    int h = 0, size = strlen(key);
    for (int i = 0; i < size; ++i)
        h = (base * h + key[i]) % M;
    return h;
}

int prime_hash(char *key, int M)
{
    return base_hash(key, M, 89);
}
int binary_hash(char *key, int M)
{
    return base_hash(key, M, 2);
}
int length_hash(char *key, int M)
{
    return base_hash(key, M, strlen(key));
}

int ascii_hash(char *key, int M)
{
    int size = strlen(key), sum = 0;
    for (int i = 0; i < size; ++i)
    {
        sum += key[i];
        sum %= M;
    }
    return sum;
}

// copied from "https://github.com/TheAlgorithms/C/blob/master/data_structures/hash_set/hash_set.c"
int adler_hash(void *key, int M)
{
    char *str = key;

    int a = 1;
    int b = 0;
    const int MODADLER = 65521;

    for (int i = 0; str[i] != '\0'; i++)
    {
        a = (a + str[i]) % MODADLER;
        b = (b + a) % MODADLER;
    }

    long long hash = (b << 16) | a;

    return (hash ^ (hash >> 12)) % M;
}

int quad_rehash(int *hv, int M, hash_table table)
{
    for (int i = 1; i < M; ++i)
    {
        int idx = (*hv + i * i) % M;

        if (table->data[idx] == NULL)
        {
            *hv = idx;
            return i;
        }
    }

    // should never reach here but in case    
    return -1;
}

int plus_n_rehash(int *hv, int M, hash_table table, int N)
{
    int probes = 0;
    do
    {
        probes++;
        *hv = (*hv + N) % M;
    } while (table->data[*hv] != NULL); 

    return probes;
}

int linear_probe(int *hv, int M, hash_table table)
{
    return plus_n_rehash(hv, M, table, 1);
}

int plus_5(int *hv, int M, hash_table table)
{
    return plus_n_rehash(hv, M, table, 5);
}

//TODO: change checks to asserts
hash_table new_table(free_fn fn, int size, float lf)
{
    if (size < 0)
        size = 0;

    if (lf > 1 || lf <= 0)
        lf = DEFAULT_LF;

    size = ((double)size / lf) + 0.5;

    hash_table table;
    table = malloc(sizeof(struct _hash_table));

    table->size = 0;
    table->capacity = size;
    table->free_fn = fn;
    table->LF = lf;

    table->data = calloc(table->capacity, sizeof(void *));
    return table;
}

void free_table(hash_table table)
{
    for (int i = 0; i < table->capacity; ++i)
    {
        if (table->data[i])
        {
            table->free_fn(table->data[i]);
        }
    }
    free(table->data);
    free(table);
}
