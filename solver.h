#ifndef SOLVER_H
#define SOLVER_H
#include "wordle.h"

/* Opaque solver type */
typedef struct Solver Solver;

/* Create a solver and load candidates from a dictionary file. Returns NULL on error */
Solver *solver_create_from_file(const char *filename);

/* Destroy solver and free resources */
void solver_destroy(Solver *s);

/* Number of remaining candidates */
int solver_candidate_count(const Solver *s);

/* Select the next guess (malloc'd string, caller must free) using internal scoring */
char *solver_select_next(Solver *s);

/* Filter possible words keeping only those consistent with guess/feedback */
void solver_filter_possible(Solver *s, const char *guess, const char *feedback);

/* Print a short status: remaining count and top candidates (up to n) */
void solver_print_status(const Solver *s, int n);

/* Pick a random secret from the solver's current candidate list (malloc'd) */
char *solver_pick_random_secret(Solver *s);

/* Score a word against current candidates */
int score_word(const Solver *s, const char *word);

/* Run the solver loop until found or attempts exhausted. If secret==NULL, a random secret from dictionary is used.
 * verbose controls printed output. Returns number of attempts on success, -1 on failure.
 */
int solver_loop(Solver *s, const char *secret, int verbose);

/* Backwards-compatible helpers (optional) */
char *pick_best_guess(char candidates[][WORD_BUFFER], int count);
void filter_candidates(const char *guess, const char *feedback, char candidates[][WORD_BUFFER], int *count);

/* Compatibility names requested by user */
Solver *initialize_solver(const char *filename);
void destroy_solver(Solver *s);
char *select_next_guess(Solver *s);
void filter_possible_words(Solver *s, const char *guess, const char *feedback);

#endif /* SOLVER_H */
