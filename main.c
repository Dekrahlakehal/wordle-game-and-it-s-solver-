#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordle.h"

int main() {
    char dictionary[MAX_WORDS][WORD_BUFFER];
    char *dict_ptrs[MAX_WORDS];  // Array of pointers for pick_random_word
    char feedback[WORD_BUFFER];
    char guess[WORD_BUFFER];
    char *secret_word;
    int num_words;
    int attempts = 0;
    int max_attempts = 6;
    int won = 0;

    // Load dictionary from file
    num_words = loadDictionary("word.txt", dictionary);
    if (num_words == 0) {
        printf("\033[1;91mError! Could not load dictionary.\n\033[0m");
        return 1;
    }

    // Create array of pointers to dictionary words
    for (int i = 0; i < num_words; i++) {
        dict_ptrs[i] = dictionary[i];
    }

    /* Welcome (pink) and one-time legend */
    printf("\033[1;35m=== Welcome to Wordle! ===\n\033[0m");
    printf("You have %d attempts to guess the %d-letter word.\n\n", max_attempts, WORD_LEN);
    printf("Legend (shown once): (\033[1;32m● Green\033[0m = correct position, \033[1;33m● Yellow\033[0m = wrong position, \033[1;90m● Gray\033[0m = not in word)\n\n");

    /* persistent small board to show past guesses */
    char past_guesses[6][WORD_BUFFER];
    char past_feedbacks[6][WORD_BUFFER];
    for (int i = 0; i < max_attempts; i++) {
        past_guesses[i][0] = '\0';
        past_feedbacks[i][0] = '\0';
    }
    int printed_legend = 1; /* we already printed it above */
    /* keyboard state: 'U' unknown, 'G' green, 'Y' yellow, 'B' gray */
    char keyboard_state[26];
    for (int i = 0; i < 26; ++i) keyboard_state[i] = 'U';

    // Pick a random word
    secret_word = pick_random_word(dict_ptrs, num_words);
    if (secret_word == NULL) {
        printf("\033[1;91mError! Could not select a word.\n\033[0m");
        return 1;
    }

    // Game loop
    while (attempts < max_attempts && !won) {
        /* Clear screen and re-render compact UI (legend shown only once) */
        printf("\033[2J\033[H");
        /* header */
        printf("\033[1;35m=== Wordle ===\033[0m\n");
        printf("Attempts: %d/%d\n", attempts, max_attempts);
        if (printed_legend) {
            /* show brief note that legend was shown once */
            printf("(Legend shown at start)\n\n");
        } else {
            printf("\n");
        }

        /* Render board */
        printf("Previous guesses:\n");
        for (int i = 0; i < max_attempts; i++) {
            printf("%d: ", i + 1);
            if (past_guesses[i][0] == '\0') {
                for (int k = 0; k < WORD_LEN; k++) printf("[   ] ");
                printf("\n");
            } else {
                display_feedback(past_feedbacks[i], past_guesses[i]);
                printf("\n");
            }
        }

        /* Render keyboard (QWERTY) */
        const char *row1 = "qwertyuiop";
        const char *row2 = "asdfghjkl";
        const char *row3 = "zxcvbnm";
        printf("\nKeyboard:\n");
        for (int r = 0; row1[r]; ++r) {
            char ch = row1[r];
            char s = keyboard_state[ch - 'a'];
            if (s == 'G') printf("\033[30;42m %c \033[0m ", ch);
            else if (s == 'Y') printf("\033[30;43m %c \033[0m ", ch);
            else if (s == 'B') printf("\033[37;100m %c \033[0m ", ch);
            else printf("[ %c ] ", ch);
        }
        printf("\n");
        for (int r = 0; row2[r]; ++r) {
            char ch = row2[r];
            char s = keyboard_state[ch - 'a'];
            if (s == 'G') printf("\033[30;42m %c \033[0m ", ch);
            else if (s == 'Y') printf("\033[30;43m %c \033[0m ", ch);
            else if (s == 'B') printf("\033[37;100m %c \033[0m ", ch);
            else printf("[ %c ] ", ch);
        }
        printf("\n");
        for (int r = 0; row3[r]; ++r) {
            char ch = row3[r];
            char s = keyboard_state[ch - 'a'];
            if (s == 'G') printf("\033[30;42m %c \033[0m ", ch);
            else if (s == 'Y') printf("\033[30;43m %c \033[0m ", ch);
            else if (s == 'B') printf("\033[37;100m %c \033[0m ", ch);
            else printf("[ %c ] ", ch);
        }
        printf("\n\n");

        printf("\nEnter your guess (attempt %d/%d): ", attempts + 1, max_attempts);
        
        // Get user input
        if (fgets(guess, sizeof(guess), stdin) == NULL) {
            printf("\033[1;91mError reading input.\n\033[0m");
            continue;
        }

        // Remove newline character if present
        if (guess[strlen(guess) - 1] == '\n') {
            guess[strlen(guess) - 1] = '\0';
        }

        // Validate the guess
        if (!isValidGuess(guess, dictionary, num_words)) {
            printf("\033[1;91mError! Invalid guess. Must be a 5-letter word in the dictionary.\n\033[0m\n");
            continue;
        }

        // Get feedback
        getFeedback(secret_word, guess, feedback);

        // Store and show in board
        strncpy(past_guesses[attempts], guess, WORD_BUFFER - 1);
        past_guesses[attempts][WORD_BUFFER - 1] = '\0';
        strncpy(past_feedbacks[attempts], feedback, WORD_BUFFER - 1);
        past_feedbacks[attempts][WORD_BUFFER - 1] = '\0';

        /* Update keyboard state: precedence G > Y > B */
        for (int i = 0; i < WORD_LEN; ++i) {
            char c = guess[i];
            if (c < 'a' || c > 'z') continue;
            int idx = c - 'a';
            char f = feedback[i];
            if (f == 'G') keyboard_state[idx] = 'G';
            else if (f == 'Y') {
                if (keyboard_state[idx] != 'G') keyboard_state[idx] = 'Y';
            } else { /* B */
                if (keyboard_state[idx] != 'G' && keyboard_state[idx] != 'Y') keyboard_state[idx] = 'B';
            }
        }

        // Display feedback for this guess
        printf("Feedback: ");
        display_feedback(feedback, guess);
        printf("\n\n");

        // Check if the guess is correct
        if (strcmp(guess, secret_word) == 0) {
            printf("\033[1;32m🎉 Congratulations! You guessed the word: %s\n\033[0m", secret_word);
            won = 1;
        } else {
            attempts++;
        }
    }

    // Game over
    if (!won) {
        printf("\033[1;91m❌ Game Over! The word was: %s\n\033[0m", secret_word);
    }

    // Free allocated memory
    free(secret_word);

    return 0;
}
