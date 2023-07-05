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

#include "hashmap.h"

/* libraries */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "config.h"

/* keyValuePair functions */
/* create new keyValuePair */
keyValuePair *new_keyValuePair (wsInt key, wsInt value)
{
    keyValuePair *new_pair;

    /* allocate new_pair */
    /* we are using a keyValuePair POINTER, to avoid the compiler trying to free local variables */
    new_pair = (keyValuePair *)malloc (sizeof (keyValuePair));

    /* assign the Pair's Key */
    new_pair->key = key;

    /* assigne the Pair's Value */
    new_pair->value = value;

    return new_pair;
}

/* free keyValuePair */
void free_keyValuePair (keyValuePair *pair)
{
    /* guard clauses */
    /* only work with valid pointers */
    assert (pair != NULL);

    /* free pair */
    free (pair);

}


/* hashMap functions */
/* create new hashMap */
hashMap *new_hashMap (void)
{
    hashMap *new_hash;

    /* allocate hashMap */
    new_hash = (hashMap *)malloc (sizeof (hashMap));

    /* set starting size information */
    new_hash->size = 2;
    new_hash->count = 0;

    /* allocate items */
    new_hash->item_list = (keyValuePair **)malloc (new_hash->size * sizeof (keyValuePair *));
    assert (new_hash->item_list != NULL);

    return new_hash;
}

/* free hashMap */
void free_hashMap (hashMap *hash_map)
{
    uintmax_t i = 0;

    /* guard statements */
    /* only work with valid pointers */
    assert (hash_map != NULL);
    assert (hash_map->item_list != NULL);

    /* dispose of each keyValuePair */
    for (; i < hash_map->count; i++)
        free_keyValuePair (hash_map->item_list[i]);

    /* dispose of item_list */
    free (hash_map->item_list);

    /* free hash_map */
    free (hash_map);
}

/* extend the allocated length of a hashMap */
void hash_extend (hashMap *hash_map)
{
    /* guard statements */
    /* dont work with NULLs */
    assert (hash_map != NULL);
    assert (hash_map->item_list != NULL);

    /* double the allocated length, and realloc the list */
    hash_map->size *= 2;
    hash_map->item_list = realloc (hash_map->item_list, hash_map->size * sizeof (keyValuePair));
}

/* append a new item to the end of a hashmap*/
void hash_append (hashMap *hash_map, wsInt key, wsInt value)
{
    uintmax_t new_index = hash_map->count;

    /* guard clauses */
    /* dont work with nulls */
    assert (hash_map != NULL);
    assert (hash_map->item_list != NULL);
    /* YELL VIOLENTLY if the size is 0, DONT WORK WITH IT, JUST CEASE */
    assert (hash_map->size != 0);


    /* first, see if their is enough allocated space for new item */
    /* incase user touches count (NOT ADVISED), keep extending until adequately sized */
    /* also dont run, if size is 0, otherwise youll enter an infinate loop */
    while (hash_map->count >= hash_map->size && hash_map->size != 0)
        hash_extend (hash_map);

    /* next, create a new keyValuePair */
    hash_map->item_list[new_index] = new_keyValuePair (key, value);


    /* increment hash_map->count */
    hash_map->count++;
}

/* destroy the last element of a hashmap */
void hash_pop (hashMap *hash_map)
{
    uintmax_t item_index;

    /* guard clasuses */
    /* dont work with NULLS */
    assert (hash_map != NULL);
    assert (hash_map->item_list != NULL);
    /* dont work with empty hashMaps */
    assert (hash_map->count != 0);

    /* reduce count */
    hash_map->count--;

    /* set item_index to the item we want to remove */
    item_index = hash_map->count;

    /* free the indexed item */
    free_keyValuePair (hash_map->item_list[item_index]);
}

/* search a hashMap for a specific key, and put the keyValuePair's index in a var */
/* if a matching key is found, return true. otherwise, return false */
bool hash_search_index (hashMap *hash_map, wsInt key, uintmax_t *return_index)
{
    uintmax_t i = 0;
    keyValuePair *item;
    bool match_found = false;

    /* function parameter guard clauses */
    /* dont work with NULLs */
    assert (hash_map != NULL);
    assert (hash_map->item_list != NULL);

    /* check each element in the hashMap, until one has a matching key*/
    /* stop looping through when either, i is equal to count, or we find a match */
    for (; i < hash_map->count && !match_found; i++)
    {
        /* pointer to the current item */
        item = hash_map->item_list[i];

        /* item guard clauses */
        assert (item != NULL);

        /* compare the item's key and the param key */
        /* if the keys match, set return variables to adequate values */
        if (item->key == key)
        {
            /* set return bool to true, showing we found a match */
            match_found = true;

            /* copy item's index into the return_index var */
            (*return_index) = i;
        }
    }

    /* return true if match is found, false if not */
    return match_found;
}

/* search a hashMap for a specific key, and puts the key's value into a variable */
/* if a matching key is found, return true. otherwise, return false */
bool hash_search (hashMap *hash_map, wsInt key, wsInt *return_value)
{
    uintmax_t match_index;
    keyValuePair *match;
    bool match_found = false;

    /* function parameter guard clauses */
    /* dont work with NULLs */
    assert (hash_map != NULL);
    assert (hash_map->item_list != NULL);

    /* search for the provided key in the hashmap */
    match_found = hash_search_index  (hash_map, key, &match_index);
    /* if a match is found, copy the value over to the return value */
    if (match_found)
    {
        match = hash_map->item_list[match_index];
        /* copy value from match->value into return_value */
        (*return_value) = match->value;
    }

    /* return true if match is found, false if not */
    return match_found;
}


/* set an element in the hashMap */
void hash_set (hashMap *hash_map, wsInt key, wsInt value)
{
    bool key_exists;
    uintmax_t key_index;
    keyValuePair *item;

    /* guard clauses taken care of by other functions */

    /* check if element exsists with the current key */
    key_exists = hash_search_index (hash_map, key, &key_index);

    /* if key doesnt exist, append a new item, and exit function */
    if (!key_exists)
    {
        hash_append (hash_map, key, value);
        return;
    }

    /* if key does exist update the value */
    item = hash_map->item_list[key_index];

    /* and finally copy the new value over */
    item->value = value;
}
