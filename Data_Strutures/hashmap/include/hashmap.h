#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_USER 256
#define TABLE_SIZE 10
#define ALLOCATION_ATTEMPTS 5
#define SIZE_INCREASE_FACTOR 2

#define CHECK_LAST(_iterator, _attempts) if((_iterator + 1) == _attempts) { \
                fprintf(stderr, "ERROR: Memory allocation error\n"); \
                exit(1); \
                }

typedef uint64_t hash_func(const void*, size_t);
typedef struct _HashMap HashMap;

_Bool init_hashmap(uint64_t size, hash_func hasher, HashMap **map);
void destroy_hashmap(HashMap *map);
void print_hashmap(HashMap *map);
_Bool hashmap_insert(HashMap *map, const void *key, size_t keyLength, void *object);
void *hashmap_lookup(HashMap *map, const void *key, size_t keyLength);
void *hashmap_delete(HashMap *map, const void *key, size_t keyLength);

#endif