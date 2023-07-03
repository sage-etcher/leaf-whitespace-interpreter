/* only run once */
/* modern solution */
#pragma once
/* legacy backup solution */
#ifndef _HASHMAP_H_
#define _HASHMAP_H_

/* libraries */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>


/* macros */
#define HASHMAP_MAX(a,b) (a > b ? a : b)
#define HASHMAP_MIN(a,b) (a < b ? a : b)


/* type definitions */
typedef struct
{
    char *key;
    uintmax_t key_size;

    void *value;
    size_t value_size;
} keyValuePair;

typedef struct
{
    keyValuePair **item_list;
    uintmax_t size;
    uintmax_t count;
} hashMap;


/* function prototypes */
keyValuePair *new_keyValuePair (char *key, void *value, size_t value_size);
void free_keyValuePair (keyValuePair *pair);

hashMap *new_hashMap (void);
void free_hashMap (hashMap *hash_map);

void hash_extend (hashMap *hash_map);
void hash_append (hashMap *hash_map, char *key, void *value, size_t value_size);
void hash_pop (hashMap *hash_map);
bool hash_search_index (hashMap *hash_map, char *key, void *return_index);
bool hash_search (hashMap *hash_map, char *key, void *return_value);
void hash_set (hashMap *hash_map, char *key, void *value, size_t value_size);

#endif