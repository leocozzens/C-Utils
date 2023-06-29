#include <hashmap.h>
#include <time.h>

#define NAME_LEN 10

uint64_t hash_string(const void *key, size_t) {
    const char *keyStr = key;
    unsigned int len = strlen(keyStr);
    unsigned int hashVal = 0;
    for(unsigned int i = 0; i < len; i++) {
        hashVal += keyStr[i];
    }
    return hashVal;
}

int main(void) {
    HashMap *map = NULL;
    if(init_hashmap(TABLE_SIZE, hash_string, &map)) return 1; // Allocation error
    char *str = "Hello world";
    char *obj = "Testing";

    //print_hashmap(map);
    hashmap_insert(map, str, strlen(str) + 1, obj);
    print_hashmap(map);
    char *test = hashmap_lookup(map, str, strlen(str) + 1);

    printf("Test: %s\n", test);
    return 0;
}