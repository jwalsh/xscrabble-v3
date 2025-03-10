/**
 * XScrabble - Board Structure Definitions
 */

#ifndef XSCRABBLE_BOARD_H
#define XSCRABBLE_BOARD_H

#include <stdbool.h>

/* Board dimensions */
#define BOARD_SIZE 15

/* Special cell types */
typedef enum {
    CELL_NORMAL,
    CELL_DOUBLE_LETTER,
    CELL_TRIPLE_LETTER,
    CELL_DOUBLE_WORD,
    CELL_TRIPLE_WORD
} CellType;

/* Board cell structure */
typedef struct {
    CellType type;
    char letter;
    bool is_fixed;
} BoardCell;

/* Function prototypes */
bool board_init(void);
void board_cleanup(void);
BoardCell* board_get_cell(int row, int col);
CellType board_get_cell_type(int row, int col);
bool board_place_tile(int row, int col, char letter);
bool board_remove_tile(int row, int col);
void board_commit_word(void);
void board_revert_word(void);

#endif /* XSCRABBLE_BOARD_H */
