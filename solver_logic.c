// New solver implementation with a Solver object and helper functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"
#include "wordle.h"

struct Solver {
    char candidates[MAX_WORDS][WORD_BUFFER];
    int count;
};

/* Internal: score words by unique-letter frequency */
static void compute_letter_freq(const Solver *s, int freq[26]) {
    for (int i = 0; i < 26; ++i) freq[i] = 0;
    for (int i = 0; i < s->count; ++i) {
        int seen[26] = {0};
        for (int j = 0; j < WORD_LEN; ++j) {
            char c = s->candidates[i][j];
            if (c < 'a' || c > 'z') continue;
            int idx = c - 'a';
            if (!seen[idx]) { freq[idx]++; seen[idx] = 1; }
        }
    }
}

/* Create solver from file */
Solver *solver_create_from_file(const char *filename) {
    Solver *s = malloc(sizeof(Solver));
    if (!s) return NULL;
    int loaded = loadDictionary(filename, s->candidates);
    if (loaded <= 0) { free(s); return NULL; }
    s->count = loaded;
    return s;
}

void solver_destroy(Solver *s) {
    free(s);
}

int solver_candidate_count(const Solver *s) {
    return s ? s->count : 0;
}

/* Pick best using internal freq scoring */
char *solver_select_next(Solver *s) {
    if (!s || s->count <= 0) return NULL;
    int freq[26]; compute_letter_freq(s, freq);
    long best_score = -1; int best_idx = 0;
    for (int i = 0; i < s->count; ++i) {
        int seen[26] = {0}; long score = 0;
        for (int j = 0; j < WORD_LEN; ++j) {
            char c = s->candidates[i][j]; if (c < 'a' || c > 'z') continue;
            int idx = c - 'a'; if (!seen[idx]) { score += freq[idx]; seen[idx] = 1; }
        }
        if (score > best_score) { best_score = score; best_idx = i; }
    }
    char *copy = malloc(WORD_BUFFER);
    if (!copy) return NULL;
    strncpy(copy, s->candidates[best_idx], WORD_BUFFER-1); copy[WORD_BUFFER-1] = '\0';
    return copy;
}

/* Score a given word against current candidate letter frequencies. Higher is better. */
int score_word(const Solver *s, const char *word) {
    if (!s || !word) return 0;
    int freq[26]; compute_letter_freq(s, freq);
    int seen[26] = {0}; int score = 0;
    for (int j = 0; j < WORD_LEN; ++j) {
        char c = word[j]; if (c < 'a' || c > 'z') continue; int idx = c - 'a';
        if (!seen[idx]) { score += freq[idx]; seen[idx] = 1; }
    }
    return score;
}

/* solver_loop - run the solver until solved or out of attempts. If secret==NULL pick random.
 * returns attempts used on success, -1 on failure.
 */
int solver_loop(Solver *s, const char *secret_in, int verbose) {
    if (!s) return -1;
    char *secret = NULL;
    char *dict_ptrs[MAX_WORDS];
    for (int i = 0; i < s->count; ++i) dict_ptrs[i] = s->candidates[i];
    if (secret_in) {
        secret = malloc(WORD_BUFFER); strncpy(secret, secret_in, WORD_BUFFER-1); secret[WORD_BUFFER-1]='\0';
    } else {
        secret = pick_random_word(dict_ptrs, s->count);
        if (!secret) return -1;
    }

    int attempts = 0;
    while (attempts < 6 && s->count > 0) {
        if (verbose) { printf("[Solver] Step %d - remaining candidates: %d\n", attempts+1, s->count); }
        char *guess = solver_select_next(s);
        if (!guess) break;
        char feedback[WORD_BUFFER];
        getFeedback(secret, guess, feedback);
        if (verbose) {
            printf("[Solver] Guess: %s  Feedback: ", guess); display_feedback(feedback, guess); printf("\n");
        }
        if (strcmp(guess, secret) == 0) { free(guess); if (!secret_in) free(secret); return attempts+1; }
        solver_filter_possible(s, guess, feedback);
        free(guess);
        attempts++;
    }
    if (!secret_in) free(secret);
    return -1;
}

/* local feedback computation (same as game) */
static void compute_feedback_local(const char *secret, const char *guess, char *out) {
    int len = WORD_LEN; int used[WORD_LEN]; for (int i=0;i<len;i++) used[i]=0;
    for (int i=0;i<len;i++) { if (guess[i]==secret[i]) { out[i]='G'; used[i]=1; } else out[i]='B'; }
    for (int i=0;i<len;i++) { if (out[i]=='G') continue; for (int j=0;j<len;j++) if(!used[j] && guess[i]==secret[j]) { out[i]='Y'; used[j]=1; break; } }
    out[len]='\0';
}

void solver_filter_possible(Solver *s, const char *guess, const char *feedback) {
    if (!s || s->count<=0) return;
    int write = 0; char ftmp[WORD_BUFFER];
    for (int i=0;i<s->count;i++) {
        compute_feedback_local(s->candidates[i], guess, ftmp);
        if (strcmp(ftmp, feedback)==0) {
            if (write!=i) { strncpy(s->candidates[write], s->candidates[i], WORD_BUFFER-1); s->candidates[write][WORD_BUFFER-1]='\0'; }
            write++;
        }
    }
    s->count = write;
}

void solver_print_status(const Solver *s, int n) {
    if (!s) return;
    printf("Remaining candidates: %d\n", s->count);
    int show = (n < s->count) ? n : s->count;
    for (int i=0;i<show;i++) printf("  %s\n", s->candidates[i]);
    if (s->count > show) printf("  ... (+%d more)\n", s->count - show);
}

/* Backwards-compatible helpers (wrap the solver functions) */
char *pick_best_guess(char candidates[][WORD_BUFFER], int count) {
    Solver tmp; tmp.count = count; for (int i=0;i<count;i++) strncpy(tmp.candidates[i], candidates[i], WORD_BUFFER-1);
    return solver_select_next(&tmp);
}

void filter_candidates(const char *guess, const char *feedback, char candidates[][WORD_BUFFER], int *count) {
    Solver tmp; tmp.count = *count; for (int i=0;i<*count;i++) strncpy(tmp.candidates[i], candidates[i], WORD_BUFFER-1);
    solver_filter_possible(&tmp, guess, feedback);
    /* copy back */
    for (int i=0;i<tmp.count;i++) strncpy(candidates[i], tmp.candidates[i], WORD_BUFFER-1);
    *count = tmp.count;
}

/* Convenience wrappers requested in header */
Solver *initialize_solver(const char *filename) { return solver_create_from_file(filename); }
void destroy_solver(Solver *s) { solver_destroy(s); }
char *select_next_guess(Solver *s) { return solver_select_next(s); }
void filter_possible_words(Solver *s, const char *guess, const char *feedback) { solver_filter_possible(s, guess, feedback); }

char *solver_pick_random_secret(Solver *s) {
    if (!s || s->count <= 0) return NULL;
    char *ptrs[MAX_WORDS];
    for (int i = 0; i < s->count; ++i) ptrs[i] = s->candidates[i];
    return pick_random_word(ptrs, s->count);
}
