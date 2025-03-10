/**
 * XScrabble - Modern Scrabble Implementation for X11
 * 
 * Main entry point for the application
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>

#include "game.h"
#include "board.h"
#include "ui.h"
#include "config.h"

/* Global variables */
XtAppContext app_context;
Widget       top_level;

int main(int argc, char *argv[])
{
    /* Initialize X toolkit */
    top_level = XtAppInitialize(
        &app_context,
        "XScrabble",
        NULL, 0,
        &argc, argv,
        NULL,
        NULL, 0
    );
    
    /* Initialize game state */
    if (!game_init()) {
        fprintf(stderr, "Failed to initialize game\n");
        return EXIT_FAILURE;
    }
    
    /* Initialize UI components */
    if (!ui_init(top_level)) {
        fprintf(stderr, "Failed to initialize UI\n");
        game_cleanup();
        return EXIT_FAILURE;
    }
    
    /* Display main window */
    XtRealizeWidget(top_level);
    
    /* Start main event loop */
    XtAppMainLoop(app_context);
    
    /* This point is never reached, but for completeness */
    game_cleanup();
    return EXIT_SUCCESS;
}
