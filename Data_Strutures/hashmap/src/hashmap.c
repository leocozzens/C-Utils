#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

typedef struct Instance {
    void *key;
    size_t keySize;
    void *object;
    size_t objectSize;
    struct Instance *nextInstance;
} Instance;

struct _HashMap {
    uint64_t tableSize;
    uint64_t collisionCount; // This is purely for diagnostic purposes, remove if not testing performance
    hash_func hasher;
    Instance **table;
};


static _Bool check_if_dup(Instance *checkInst, const void *key, size_t keySize);

ExitCode hashmap_init(uint64_t size, hash_func hasher, HashMap **map) {
    *map = malloc(sizeof(HashMap));
    if(map == NULL) return ALLOC_ERR;
    (*map)->tableSize = size;
    (*map)->hasher = hasher;
    (*map)->collisionCount = 0;
    (*map)->table = malloc(sizeof(Instance*) * (*map)->tableSize);
    if((*map)->table == NULL) return ALLOC_ERR;

    for(uint64_t i = 0; i < (*map)->tableSize; i++) {
        (*map)->table[i] = NULL;
    }
    return SUCCESS;
}

ExitCode hashmap_insert(HashMap *map, const void *key, size_t keySize, void *object, size_t objectSize) {
    if(map == NULL || key == NULL || object == NULL) return ALLOC_ERR;
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);
    if(check_if_dup(map->table[index], key, keySize)) return DUPLICATE_KEY;
    if(map->table[index] != NULL) map->collisionCount++; // Diagnostic, remove in regular usage

    Instance *newInst = malloc(sizeof(Instance));

    if(newInst == NULL) return ALLOC_ERR;
    newInst->object = malloc(objectSize);
    if(newInst->object == NULL) {
        free(newInst);
        return ALLOC_ERR;
    }
    newInst->key = malloc(keySize);
    if(newInst->key == NULL) {
        free(newInst);
        free(newInst->object);
        return ALLOC_ERR;
    }

    memcpy(newInst->key, key, keySize);
    memcpy(newInst->object, object, objectSize);
    newInst->keySize = keySize;
    newInst->objectSize = objectSize;

    newInst->nextInstance = map->table[index];
    map->table[index] = newInst;
    return SUCCESS;
}

static _Bool check_if_dup(Instance *checkInst, const void *key, size_t keySize) {
    Instance *tempInst = checkInst;
    while(tempInst != NULL) {
        if(tempInst->keySize == keySize && memcmp(checkInst->key, key, keySize) == 0) return 1;
        tempInst = tempInst->nextInstance;
    }
    return 0;
}

void *hashmap_lookup(HashMap *map, const void *key, size_t keySize, size_t *objectSize) {
    if(map == NULL || key == NULL) return NULL;
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    Instance *tempInst = map->table[index];
    while(tempInst != NULL && !(tempInst->keySize == keySize && memcmp(tempInst->key, key, keySize) == 0)) {
        tempInst = tempInst->nextInstance;
    }
    if(tempInst == NULL) return NULL;
    if(objectSize != NULL) *objectSize = tempInst->objectSize;
    return tempInst->object;
}

void *hashmap_delete(HashMap *map, const void *key, size_t keySize, size_t *objectSize) {
    if(map == NULL || key == NULL) return NULL;
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    Instance *tempInst = map->table[index];
    Instance *prevInst = NULL;
    while(tempInst != NULL && !(tempInst->keySize == keySize && memcmp(tempInst->key, key, keySize) == 0)) {
        prevInst = tempInst;
        tempInst = tempInst->nextInstance;
    }
    if(tempInst == NULL) return NULL;
    if(prevInst == NULL) map->table[index] = tempInst->nextInstance;
    else prevInst->nextInstance = tempInst->nextInstance;

    void *result = tempInst->object;
    if(objectSize != NULL) *objectSize = tempInst->objectSize;
    free(tempInst->key);
    free(tempInst);
    return result;
}

uint64_t get_collisions(HashMap *map) { // Diagnostic remove in regulat usage
    return map->collisionCount;
}

void hashmap_kill(HashMap **map) {
    if(*map == NULL) return;
    for(uint64_t i = 0; i < (*map)->tableSize; i++) {
        if((*map)->table[i] == NULL) continue;
        Instance *tempInst = (*map)->table[i];
        do {
            Instance *nextInst = tempInst->nextInstance;
            free(tempInst->key);
            free(tempInst->object);
            free(tempInst);
            tempInst = nextInst;
        } while(tempInst != NULL);
    }
    free((*map)->table);
    free(*map);
    *map = NULL;
}