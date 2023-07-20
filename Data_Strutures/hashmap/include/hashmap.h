#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>

typedef enum {
    ALLOC_ERR = -1,
    SUCCESS,
    DUPLICATE_KEY
} ExitCode;

typedef uint64_t hash_func(const void* key, size_t keySize);
typedef struct _HashMap HashMap;

ExitCode init_hashmap(uint64_t size, hash_func hasher, HashMap **map);
void destroy_hashmap(HashMap *map);
void print_hashmap(HashMap *map);
ExitCode hashmap_insert(HashMap *map, const void *key, size_t keySize, void *object, size_t objectSize);
void *hashmap_lookup(HashMap *map, const void *key, size_t keySize);
void *hashmap_delete(HashMap *map, const void *key, size_t keySize);

#endif