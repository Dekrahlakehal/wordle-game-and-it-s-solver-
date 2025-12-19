#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "wordle.h"

// convert uppercase letters in a string to lowercase in-place
void tolowerCase(char str[]){
    for (int i = 0; str[i] != '\0'; i++){
        if (isupper((unsigned char)str[i])){
            str[i] = tolower((unsigned char)str[i]);
        }
    }
}

// Validate guess: must be WORD_LEN letters and exist in dictionary
int isValidGuess(char guess[], char dictionary[][WORD_BUFFER], int numWords) {
    tolowerCase(guess);

    if (strlen(guess) != WORD_LEN) {
        return 0;
    }

    for (int i = 0; i < WORD_LEN; i++) {
        if (!isalpha((unsigned char)guess[i])) {
            return 0;
        }
    }

    for (int i = 0; i < numWords; i++) {
        if (strcmp(guess, dictionary[i]) == 0) {
            return 1;
        }
    }

    return 0;
}