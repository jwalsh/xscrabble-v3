/**
 * XScrabble - Game Logic Definitions
 */

#ifndef XSCRABBLE_GAME_H
#define XSCRABBLE_GAME_H

#include <stdbool.h>

/* Game state structure */
typedef struct {
    char current_player[32];
    int scores[2];
    int tiles_left;
    char current_word[16];
    char player_rack[7];
} GameState;

/* Function prototypes */
bool game_init(void);
void game_cleanup(void);
GameState* game_get_state(void);
bool game_place_tile(int row, int col, char letter);
bool game_remove_tile(int row, int col);
int game_evaluate_move(void);
bool game_finish_turn(void);
void game_pass_turn(void);
bool game_change_letters(void);
void game_revert_move(void);
bool game_shuffle_rack(void);

#endif /* XSCRABBLE_GAME_H */
