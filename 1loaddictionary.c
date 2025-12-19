#include <stdio.h>
#include "wordle.h"

int loadDictionary(const char *filename, char dictionary[][WORD_BUFFER]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    int count = 0;
    /* read up to WORD_LEN characters per line */
    while (count < MAX_WORDS && fscanf(file, "%5s", dictionary[count]) == 1) {
        count++;
    }

    fclose(file);
    return count;
}

