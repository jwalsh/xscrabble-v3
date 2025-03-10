/**
 * XScrabble - User Interface Definitions
 */

#ifndef XSCRABBLE_UI_H
#define XSCRABBLE_UI_H

#include <X11/Intrinsic.h>
#include <stdbool.h>

/* UI component IDs */
typedef enum {
    UI_BOARD,
    UI_RACK,
    UI_SCORE,
    UI_STATUS,
    UI_BUTTON_FINISH,
    UI_BUTTON_CHANGE,
    UI_BUTTON_PASS,
    UI_BUTTON_EVALUATE,
    UI_BUTTON_REVERT,
    UI_BUTTON_JUGGLE,
    UI_BUTTON_QUIT
} UIComponentID;

/* Function prototypes */
bool ui_init(Widget parent);
void ui_cleanup(void);
void ui_update(void);
void ui_update_component(UIComponentID component);

#endif /* XSCRABBLE_UI_H */
