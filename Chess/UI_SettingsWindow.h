#ifndef UI_SETTINGSWINDOW_H_
#define UI_SETTINGSWINDOW_H_

#include "UI_Widget.h"
#include "UI_Window.h"

//TODO: move to settings header when liran give it to me.

typedef enum {
	CHESS_DIFFICULTY_AMATEUR = 1,
	CHESS_DIFFICULTY_EASY = 2,
	CHESS_DIFFICULTY_MODERATE = 3,
	CHESS_DIFFICULTY_HARD = 4,
	CHESS_DIFFICULTY_EXPERT = 5
} CHESS_DIFFICULTY;

Window* settingsWindowCreate();


#endif
