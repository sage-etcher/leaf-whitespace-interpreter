/*
    This file is part of LeafWSI.

    LeafWSI - a C based, cross-platform, whitespace lang interpretter.
    Copyright (C) 2023  Sage I. Hendricks

    LeafWSI is free software: you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version.

    LeafWSI is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License 
    along with LeafWSI. If not, see <https://www.gnu.org/licenses/>. 
*/

/* 
Contact Information:
    Email   sage.codes@email.com
    Github  sage-etcher
    Discord sage4424
*/

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