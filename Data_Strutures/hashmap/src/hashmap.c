#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

typedef struct _Instance {
    void *key;
    size_t keySize;
    void *object;
    struct _Instance *nextInstance;
} Instance;

struct _HashMap {
    uint64_t tableSize;
    hash_func *hasher;
    Instance **table;
};


static _Bool check_instance_key(Instance *checkInst, const void *key, size_t keySize);

ExitCode init_hashmap(uint64_t size, hash_func hasher, HashMap **map) {
    *map = malloc(sizeof(HashMap));
    if(map == NULL) return ALLOC_ERR;
    (*map)->tableSize = size;
    (*map)->hasher = hasher;
    (*map)->table = malloc(sizeof(Instance*) * (*map)->tableSize);
    if((*map)->table == NULL) return ALLOC_ERR;

    for(uint64_t i = 0; i < (*map)->tableSize; i++) {
        (*map)->table[i] = NULL;
    }
    return SUCCESS;
}

void destroy_hashmap(HashMap *map) {
    free(map->table);
    free(map);
}

void print_hashmap(HashMap *map) {
    printf("Table:\n");
    for(uint64_t i = 0; i < map->tableSize; i++) {
        if(map->table[i] == NULL) {
            printf("%d: ------\n", i);
        }
        else {
            Instance *tempInst = map->table[i];
            while(tempInst != NULL) {
                printf("%d: %s\"%p\" -", i, ((char*)tempInst->key), tempInst->object);
                tempInst = tempInst->nextInstance;
            }
            printf("\n");
        }
    }
    printf("End table\n");
}

ExitCode hashmap_insert(HashMap *map, const void *key, size_t keySize, void *object, size_t objectSize) {
    if(map == NULL || key == NULL || object == NULL) return ALLOC_ERR;
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    if(check_instance_key(map->table[index], key, keySize)) return DUPLICATE_KEY;

    Instance *newInst = malloc(sizeof(Instance));
    if(newInst == NULL) return ALLOC_ERR;
    newInst->object = malloc(objectSize);
    if(newInst->object == NULL) return ALLOC_ERR;
    newInst->key = malloc(keySize);
    if(newInst->key == NULL) return ALLOC_ERR;
    memcpy(newInst->key, key, keySize);
    memcpy(newInst->object, object, objectSize);

    newInst->nextInstance = map->table[index];
    map->table[index] = newInst;
    return SUCCESS;
}

static _Bool check_instance_key(Instance *checkInst, const void *key, size_t keySize) {
    Instance *tempInst = checkInst;
    while(tempInst != NULL) {
        if(tempInst->keySize == keySize) {
            if(memcmp(checkInst->key, key, keySize) != 0) return 1;
        }
        tempInst = tempInst->nextInstance;
    }
    return 0;
}

void *hashmap_lookup(HashMap *map, const void *key, size_t keySize) {
    if(map == NULL || key == NULL) return NULL;
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    Instance *tempInst = map->table[index];
    while(tempInst != NULL && (memcmp(tempInst->key, key, keySize) != 0)) {
        tempInst = tempInst->nextInstance;
    }
    if(tempInst == NULL) return NULL;
    return tempInst->object;
}

void *hashmap_delete(HashMap *map, const void *key, size_t keySize) {
    if(map == NULL || key == NULL) return NULL;
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    Instance *tempInst = map->table[index];
    Instance *prevInst = NULL;
    while(tempInst != NULL && (memcmp(tempInst->key, key, keySize) != 0)) {
        prevInst = tempInst;
        tempInst = tempInst->nextInstance;
    }
    if(tempInst == NULL) return NULL;
    if(prevInst == NULL) map->table[index] = tempInst->nextInstance;
    else prevInst->nextInstance = tempInst->nextInstance;

    void *result = tempInst->object;
    free(tempInst->key);
    free(tempInst);
    return result;
}
