#include <stdio.h>
#include <string.h>
#include "wordle.h"

void getFeedback(const char *secret, const char *guess, char *feedback) {
    int len = strlen(secret);
    int used[WORD_LEN]; // mark letters used for yellow

    for (int i = 0; i < len; i++) used[i] = 0;

    // First pass: mark Greens
    for (int i = 0; i < len; i++) {
        if (guess[i] == secret[i]) {
            feedback[i] = 'G';
            used[i] = 1;
        } else {
            feedback[i] = 'B';
        }
    }

    // Second pass: mark Yellows
    for (int i = 0; i < len; i++) {
        if (feedback[i] == 'G') continue;

        for (int j = 0; j < len; j++) {
            if (!used[j] && guess[i] == secret[j]) {
                feedback[i] = 'Y';
                used[j] = 1;
                break;
            }
        }
    }

    feedback[len] = '\0';
}
