#ifndef UI_LOADGAMEWINDOW_H_
#define UI_LOADGAMEWINDOW_H_

#include "UI_Widget.h"
#include "UI_Window.h"
#include "ChessGame.h"

typedef struct load_game_data_t {
	int numOfGames;
	int activeSlot;
} LoadGameWindowData;

Window* loadGameWindowCreate(int numOfGames);


#endif
