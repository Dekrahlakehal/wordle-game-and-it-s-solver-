#include <stdlib.h>
#include <time.h>
#include <string.h> 
char *pick_random_word(char **dictionary, int size) {
    if (size <= 0 || dictionary == NULL) {
        return NULL;
    }
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));   
        initialized = 1;
    }
    int index = rand() % size;
    char *word = dictionary[index];
    char *copy = malloc(strlen(word) + 1); 
    if (copy == NULL) {
        return NULL; 
    }
    strcpy(copy, word);
    return copy;
}