#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordle.h"
#include "solver.h"

int main(void) {
    Solver *s = solver_create_from_file("word.txt");
    if (!s) {
        printf("\033[1;91mError loading dictionary for solver\033[0m\n");
        return 1;
    }

    /* pick secret */
    char *secret = solver_pick_random_secret(s);
    if (!secret) { printf("\033[1;91mError picking secret\033[0m\n"); solver_destroy(s); return 1; }

    printf("\033[1;35m=== Wordle Solver Demo ===\033[0m\n");
    printf("Solver will attempt to find a secret chosen from the dictionary.\n\n");

    int attempts = 0;
    int solved = 0;

    while (attempts < 6 && solver_candidate_count(s) > 0 && !solved) {
        printf("--- Step %d ---\n", attempts + 1);
        solver_print_status(s, 8);

        char *guess = solver_select_next(s);
        if (!guess) break;

        char feedback[WORD_BUFFER];
        getFeedback(secret, guess, feedback);

        printf("Solver guess: %s   ", guess);
        printf("Feedback: "); display_feedback(feedback, guess); printf("\n\n");

        if (strcmp(guess, secret) == 0) {
            printf("\033[1;32mSolver found the word '%s' in %d attempts!\033[0m\n", secret, attempts + 1);
            solved = 1; free(guess); break;
        }

        solver_filter_possible(s, guess, feedback);
        free(guess);
        attempts++;
    }

    if (!solved) printf("\033[1;91mSolver did not find the word. Remaining candidates: %d\033[0m\n", solver_candidate_count(s));

    free(secret);
    solver_destroy(s);
    return 0;
}
