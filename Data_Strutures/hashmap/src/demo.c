#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <hashmap.h>

#define REQUIRED_ARGS 3

#define TABLE_SIZE 1<<15
#define WORD_SIZE 256

uint64_t hash_key(const void *key, size_t len) {
    const char *keyStr = key;
    unsigned int hashVal = 0;
    for(unsigned int i = 0; i < len; i++) {
        hashVal += keyStr[i];
    }
    return hashVal;
}

char **make_guess_list(int guessCount, uint32_t wordSize);
void free_guess_list(char ***guessList, int guessCount);
void enter_wordlist(char *wordList, HashMap *map, char **guessList, uint32_t guessCount);
void print_guesses(char **guessList, uint32_t guessCount, HashMap *map);
void delete_guesses(char **guessList, uint32_t guessCount, HashMap *map);

int main(int argc, char **argv) {
    if(argc != REQUIRED_ARGS)  {
        fprintf(stderr, "Usage: %s [WORDLIST] [MAP_CHECKS]\n", argv[0]);
        return EXIT_FAILURE;
    }
    srand(time(NULL));

    HashMap *map = NULL;
    if(init_hashmap(TABLE_SIZE, hash_key, &map) == ALLOC_ERR) return ALLOC_ERR;

    uint32_t guessCount = strtoul(argv[2], NULL, 10);
    char **guessList = make_guess_list(guessCount, WORD_SIZE);
    if(guessList == NULL) return ALLOC_ERR;

    enter_wordlist(argv[1], map, guessList, guessCount);
    print_guesses(guessList, guessCount, map);

    delete_guesses(guessList, guessCount, map);
    print_guesses(guessList, guessCount, map);

    free_guess_list(&guessList, guessCount);

    hashmap_kill(&map);
 
    return EXIT_SUCCESS;
}

char **make_guess_list(int guessCount, uint32_t wordSize) {
    char **guessList = malloc(guessCount * sizeof(char*));
    if(guessList == NULL) return NULL;
    for(int i = 0; i < guessCount; i++) {
        guessList[i] = malloc(wordSize);
        if(guessList[i] == NULL) return NULL;
    }
    return guessList;
}

void free_guess_list(char ***guessList, int guessCount) {
    if(guessList == NULL) return;
    for(int i = 0; i < guessCount; i++) {
        free((*guessList)[i]);
    }
    free(*guessList);
    *guessList = NULL;
}

void enter_wordlist(char *wordList, HashMap *map, char **guessList, uint32_t guessCount) {
    FILE *wordlist = fopen(wordList, "r");
    char buffer[WORD_SIZE];
    uint32_t wordCount = 0;
    while(fgets(buffer, WORD_SIZE, wordlist)) {
        char *endLine = strchr(buffer, '\n');
        if(endLine != NULL) *endLine = '\0';

        ExitCode result = hashmap_insert(map, buffer, strlen(buffer) + 1, buffer, strlen(buffer) + 1);
        if(result == ALLOC_ERR) exit(ALLOC_ERR);
        else if(result != SUCCESS) exit(EXIT_FAILURE);

        if(wordCount < (guessCount)) strncpy(guessList[wordCount], buffer, WORD_SIZE);
        else if((rand() % wordCount) == 0) strncpy(guessList[rand() % guessCount], buffer, WORD_SIZE);
        wordCount++;
    }
    if(wordCount < guessCount) {
        fprintf(stderr, "ERROR: Guess count larger than total wordcount\n");
        exit(EXIT_FAILURE);
    }
    printf("%d words parsed and logged in map\n", wordCount);
    fclose(wordlist);
}

void print_guesses(char **guessList, uint32_t guessCount, HashMap *map) {
    for(int i = 0; i < guessCount; i++) {
        char *guessResult =  hashmap_lookup(map, guessList[i], strlen(guessList[i]) + 1, NULL);
        if(guessResult == NULL) guessResult = "Could not be found";
        printf("%s: %s\n---------------\n", guessList[i], guessResult);
    }
}

void delete_guesses(char **guessList, uint32_t guessCount, HashMap *map) {
    for(int i = 0; i < guessCount; i++) {
        size_t objectSize;
        char *deletedObj = hashmap_delete(map, guessList[i], strlen(guessList[i]) + 1, &objectSize);

        char *outStr = malloc(objectSize);
        memcpy(outStr, deletedObj, objectSize);
        outStr[objectSize - 1] = '\0';

        if(deletedObj == NULL) strcpy(outStr, "Could not delete entry");
        free(deletedObj);

        printf("deleted: %s\n", outStr);
        free(outStr);
    }
    printf("---------------\n");
}