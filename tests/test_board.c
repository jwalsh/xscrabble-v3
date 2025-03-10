/**
 * XScrabble - Board Tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/board.h"

int main(void)
{
    printf("Running board tests...\n");
    
    /* Test board initialization */
    assert(board_init());
    
    /* Test board cell types */
    assert(board_get_cell_type(0, 0) == CELL_TRIPLE_WORD);
    assert(board_get_cell_type(7, 7) == CELL_NORMAL);
    assert(board_get_cell_type(3, 0) == CELL_DOUBLE_LETTER);
    assert(board_get_cell_type(1, 1) == CELL_DOUBLE_WORD);
    assert(board_get_cell_type(5, 1) == CELL_TRIPLE_LETTER);
    
    /* Test board cell access */
    BoardCell* cell = board_get_cell(0, 0);
    assert(cell != NULL);
    assert(cell->type == CELL_TRIPLE_WORD);
    
    /* Test placing tiles */
    assert(board_place_tile(7, 7, 'X'));
    cell = board_get_cell(7, 7);
    assert(cell->letter == 'X');
    
    /* Test removing tiles */
    assert(board_remove_tile(7, 7));
    cell = board_get_cell(7, 7);
    assert(cell->letter == '\0');
    
    /* Test committing words */
    assert(board_place_tile(8, 8, 'Y'));
    board_commit_word();
    cell = board_get_cell(8, 8);
    assert(cell->is_fixed == true);
    
    /* Test reverting uncommitted tiles */
    assert(board_place_tile(9, 9, 'Z'));
    cell = board_get_cell(9, 9);
    assert(cell->letter == 'Z');
    assert(cell->is_fixed == false);
    
    board_revert_word();
    cell = board_get_cell(9, 9);
    assert(cell->letter == '\0');
    
    /* Clean up */
    board_cleanup();
    
    printf("Board tests passed!\n");
    return EXIT_SUCCESS;
}
