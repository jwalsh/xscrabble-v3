/**
 * XScrabble - Board Structure Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

/* The game board */
static BoardCell board[BOARD_SIZE][BOARD_SIZE];

/* Special cell coordinates */
static const int triple_word_cells[][2] = {
    {0, 0}, {0, 7}, {0, 14}, 
    {7, 0}, {7, 14}, 
    {14, 0}, {14, 7}, {14, 14}
};

static const int double_letter_cells[][2] = {
    {0, 3}, {0, 11}, {2, 6}, {2, 8}, {3, 0}, {3, 7}, {3, 14},
    {6, 2}, {6, 6}, {6, 8}, {6, 12}, {7, 3}, {7, 11}, {8, 2},
    {8, 6}, {8, 8}, {8, 12}, {11, 0}, {11, 7}, {11, 14},
    {12, 6}, {12, 8}, {14, 3}, {14, 11}
};

static const int double_word_cells[][2] = {
    {1, 1}, {1, 13}, {2, 2}, {2, 12}, {3, 3}, {3, 11}, {4, 4}, {4, 10},
    {10, 4}, {10, 10}, {11, 3}, {11, 11}, {12, 2}, {12, 12}, {13, 1}, {13, 13}
};

static const int triple_letter_cells[][2] = {
    {1, 5}, {1, 9}, {5, 1}, {5, 5}, {5, 9}, {5, 13},
    {9, 1}, {9, 5}, {9, 9}, {9, 13}, {13, 5}, {13, 9}
};

/* Helper function to check if a cell is in a list of coordinates */
static bool is_cell_in_list(int row, int col, const int list[][2], int list_size)
{
    for (int i = 0; i < list_size; i++) {
        if (list[i][0] == row && list[i][1] == col) {
            return true;
        }
    }
    return false;
}

/* Initialize the board */
bool board_init(void)
{
    /* Initialize all cells as normal */
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col].type = CELL_NORMAL;
            board[row][col].letter = '\0';
            board[row][col].is_fixed = false;
        }
    }
    
    /* Set triple word score cells */
    for (int i = 0; i < sizeof(triple_word_cells) / sizeof(triple_word_cells[0]); i++) {
        int row = triple_word_cells[i][0];
        int col = triple_word_cells[i][1];
        board[row][col].type = CELL_TRIPLE_WORD;
    }
    
    /* Set double letter score cells */
    for (int i = 0; i < sizeof(double_letter_cells) / sizeof(double_letter_cells[0]); i++) {
        int row = double_letter_cells[i][0];
        int col = double_letter_cells[i][1];
        board[row][col].type = CELL_DOUBLE_LETTER;
    }
    
    /* Set double word score cells */
    for (int i = 0; i < sizeof(double_word_cells) / sizeof(double_word_cells[0]); i++) {
        int row = double_word_cells[i][0];
        int col = double_word_cells[i][1];
        board[row][col].type = CELL_DOUBLE_WORD;
    }
    
    /* Set triple letter score cells */
    for (int i = 0; i < sizeof(triple_letter_cells) / sizeof(triple_letter_cells[0]); i++) {
        int row = triple_letter_cells[i][0];
        int col = triple_letter_cells[i][1];
        board[row][col].type = CELL_TRIPLE_LETTER;
    }
    
    return true;
}

/* Clean up board resources */
void board_cleanup(void)
{
    /* No dynamic resources to clean up in current implementation */
}

/* Get a pointer to a board cell */
BoardCell* board_get_cell(int row, int col)
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return NULL;
    }
    return &board[row][col];
}

/* Get the type of a cell */
CellType board_get_cell_type(int row, int col)
{
    BoardCell* cell = board_get_cell(row, col);
    if (cell) {
        return cell->type;
    }
    return CELL_NORMAL;
}

/* Place a tile on the board */
bool board_place_tile(int row, int col, char letter)
{
    BoardCell* cell = board_get_cell(row, col);
    if (!cell || cell->is_fixed) {
        return false;
    }
    
    cell->letter = letter;
    return true;
}

/* Remove a tile from the board */
bool board_remove_tile(int row, int col)
{
    BoardCell* cell = board_get_cell(row, col);
    if (!cell || cell->is_fixed || cell->letter == '\0') {
        return false;
    }
    
    cell->letter = '\0';
    return true;
}

/* Commit word to the board (make tiles fixed) */
void board_commit_word(void)
{
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].letter != '\0' && !board[row][col].is_fixed) {
                board[row][col].is_fixed = true;
            }
        }
    }
}

/* Revert uncommitted word on the board */
void board_revert_word(void)
{
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].letter != '\0' && !board[row][col].is_fixed) {
                board[row][col].letter = '\0';
            }
        }
    }
}
