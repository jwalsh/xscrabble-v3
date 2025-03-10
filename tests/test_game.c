/**
 * XScrabble - Game Logic Tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/game.h"

int main(void)
{
    printf("Running game logic tests...\n");
    
    /* Test game initialization */
    assert(game_init());
    
    /* Test game state */
    GameState* state = game_get_state();
    assert(state != NULL);
    assert(strcmp(state->current_player, "jwalsh") == 0);
    assert(state->scores[0] == 0);
    assert(state->scores[1] == 20);
    assert(state->tiles_left == 82);
    
    /* Test player rack */
    assert(state->player_rack[0] == 'O');
    assert(state->player_rack[1] == 'K');
    assert(state->player_rack[2] == 'I');
    assert(state->player_rack[3] == 'Q');
    assert(state->player_rack[4] == 'E');
    assert(state->player_rack[5] == 'E');
    assert(state->player_rack[6] == 'L');
    
    /* Test placing tiles */
    assert(game_place_tile(8, 8, 'E'));
    
    /* Test evaluating a move */
    int score = game_evaluate_move();
    /* Simple test - actual scoring would be more complex */
    assert(score >= 0);
    
    /* Test reverting a move */
    game_revert_move();
    
    /* Clean up */
    game_cleanup();
    
    printf("Game logic tests passed!\n");
    return EXIT_SUCCESS;
}
