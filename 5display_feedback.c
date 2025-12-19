#include <stdio.h>
#include "wordle.h"

/* Print a single letter in a colored box according to feedback:
 *  G -> green background
 *  Y -> yellow background
 *  B -> gray background
 * Uses black text on colored background for readability.
 */
static void print_colored_box(char f, char c) {
    if (f == 'G') {
        /* black text on green background */
        printf("\033[30;42m %c \033[0m ", c);
    } else if (f == 'Y') {
        /* black text on yellow background */
        printf("\033[30;43m %c \033[0m ", c);
    } else {
        /* black text on bright black (gray) background */
        printf("\033[37;100m %c \033[0m ", c);
    }
}

void display_feedback(const char *feedback, const char *guess) {
    for (int i = 0; i < WORD_LEN; i++) {
        char c = guess[i];
        char f = feedback[i];
        /* If guess or feedback shorter, print placeholder */
        if (c == '\0') {
            printf("   ");
        } else {
            print_colored_box(f, c);
        }
    }
}