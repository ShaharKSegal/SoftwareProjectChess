#ifndef UI_SETTINGSWINDOW_H_
#define UI_SETTINGSWINDOW_H_

#include "UI_Widget.h"
#include "UI_Window.h"

/**
 * Summary: This is an implementation of the settings window, which creates
 * a Window type with the relevant widgets and operations regarding the settings menu.
 */

//TODO: move to settings header when liran give it to me.

typedef enum {
	CHESS_DIFFICULTY_AMATEUR = 1,
	CHESS_DIFFICULTY_EASY = 2,
	CHESS_DIFFICULTY_MODERATE = 3,
	CHESS_DIFFICULTY_HARD = 4,
	CHESS_DIFFICULTY_EXPERT = 5
} CHESS_DIFFICULTY;


/**
 * Creates a Window type with widgets and operations of the settings menu
 * Can have memory failures or SDL errors
 * @returns
 * NULL if some error occurred, otherwise a Window* of the settings menu.
 */
Window* settingsWindowCreate();


#endif
