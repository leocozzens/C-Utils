#include <hashmap.h>

typedef struct Instance {
    void *key;
    size_t keySize;
    void *object;
    struct Instance *nextInstance;
} Instance;

typedef struct _HashMap {
    uint64_t tableSize;
    hash_func *hasher;
    Instance **table;
} HashMap;

_Bool init_hashmap(uint64_t size, hash_func hasher, HashMap **map) {
    *map = malloc(sizeof(HashMap));
    if(map == NULL) return 1;
    (*map)->tableSize = size;
    (*map)->hasher = hasher;
    (*map)->table = malloc(sizeof(Instance*) * (*map)->tableSize);
    if((*map)->table == NULL) return 1;

    for(uint64_t i = 0; i < (*map)->tableSize; i++) {
        (*map)->table[i] = NULL;
    }
    return 0;
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

_Bool hashmap_insert(HashMap *map, const void *key, size_t keySize, void *object) {
    if(map == NULL || key == NULL || object == NULL) return 1;
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    // TODO: Check if same key is used

    Instance *newInst = malloc(sizeof(Instance));
    if(newInst == NULL) return 1;
    newInst->object = object;
    newInst->key = malloc(keySize);
    if(newInst->key == NULL) return 1;
    memcpy(newInst->key, key, keySize);

    newInst->nextInstance = map->table[index];
    map->table[index] = newInst;
    return 0;
}

void *hashmap_lookup(HashMap *map, const void *key, size_t keySize) {
    if(map == NULL || key == NULL) return NULL; // TODO: Add assert here
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    Instance *tempInst = map->table[index];
    while(tempInst != NULL && (memcmp(tempInst->key, key, keySize) != 0)) {
        tempInst = tempInst->nextInstance;
    }
    if(tempInst == NULL) return NULL;
    return tempInst->object;
}

void *hashmap_delete(HashMap *map, const void *key, size_t keySize) {
    if(map == NULL || key == NULL) return NULL; // TODO: Add assert here
    uint64_t index = (map->hasher(key, keySize) % map->tableSize);

    Instance *tempInst = map->table[index];
    Instance *prevInst = NULL;
    while(tempInst != NULL && (memcmp(tempInst->key, key, keySize) != 0)) {
        prevInst = tempInst;
        tempInst = tempInst->nextInstance;
    }
    if(tempInst == NULL) return NULL;
    if(prevInst == NULL) {
        map->table[index] = tempInst->nextInstance;
    }
    else {
        prevInst->nextInstance = tempInst->nextInstance;
    }
    void *result = tempInst->object;
    free(tempInst);
    return tempInst->object;
}
