#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#define TABLE_SIZE 10
#define NAME_LEN 10

uint64_t hash_key(const void *key, size_t len) {
    const char *keyStr = key;
    unsigned int hashVal = 0;
    for(unsigned int i = 0; i < len; i++) {
        hashVal += keyStr[i];
    }
    return hashVal;
}

int main(void) {
    HashMap *map = NULL;
    if(init_hashmap(TABLE_SIZE, hash_key, &map) == ALLOC_ERR) return -1; // Allocation error
    char *str = "Hello world";
    char *obj = "Testing";

    //print_hashmap(map);
    ExitCode result = hashmap_insert(map, str, strlen(str) + 1, obj, strlen(obj) + 1);
    if(result == ALLOC_ERR) return -1;
    else if(result == DUPLICATE_KEY) return 0;
    print_hashmap(map);
    char *test = hashmap_lookup(map, str, strlen(str) + 1);

    printf("Test: %s\n", test);

    hashmap_delete(map, str, strlen(str) + 1);
    print_hashmap(map);
    return 0;
}