#ifndef UI_LOADGAMEWINDOW_H_
#define UI_LOADGAMEWINDOW_H_

#include "UI_Widget.h"
#include "UI_Window.h"
#include "ChessGame.h"

typedef struct load_game_data_t {
	int page;
	int maxNumOfSlots;
	char** slotsPicPath;
	int numOfPageSlots;
	int activeSlot;
} LoadGameWindowData;

Window* loadGameWindowCreate(int maxNumOfSlots);

bool loadGameWindowChangePage(Window* window, int page);

#endif