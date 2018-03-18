#include <stdlib.h>
#include "UI_Auxiliary.h"
#include "UI_LoadGameWindow.h"
#include "UI_Button.h"
#include "ChessErrorHandler.h"

#define FIRSTGAME_X 350
#define FIRSTGAME_Y 50
#define FIRSTGAME_H 50
#define FIRSTGAME_W 150
#define GAME_DELTA 60
#define LOAD_BACK_X 250
#define LOAD_BACK_Y 400
#define LOAD_BACK_H 50
#define LOAD_BACK_W 100
#define LOAD_X 450
#define LOAD_Y 400
#define LOAD_H 50
#define LOAD_W 100

const static int OTHER_BUTTONS_NUM = 2;

static LoadGameWindowData* createLoadGameWindowData(int numOfGames) {
	LoadGameWindowData* data = malloc(sizeof(LoadGameWindowData));
	if (data == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	data->numOfGames = numOfGames;
	data->activeSlot = -1;
	return data;
}

static Widget** createLoadGameWindowWidgets(SDL_Renderer* renderer,
		int numOfGames) {
	int numOfWidgets = OTHER_BUTTONS_NUM + numOfGames;
	if (renderer == NULL )
		return NULL ;
	Widget** widgets = (Widget**) calloc(sizeof(Widget*), numOfWidgets);
	if (widgets == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}

	SDL_Rect loadR = { .x = LOAD_X, .y = LOAD_Y, .h = LOAD_H, .w = LOAD_W };
	SDL_Rect backR = { .x = LOAD_BACK_X, .y = LOAD_BACK_Y, .h = LOAD_BACK_H,
			.w = LOAD_BACK_W };
	SDL_Rect slotR = { .x = FIRSTGAME_X, .y = FIRSTGAME_Y, .h = FIRSTGAME_H,
			.w = FIRSTGAME_W };

	if (!createButtonInWidgetArray(widgets, 0, renderer, loadR,
			UI_PIC_LOAD_ACTIVE, UI_PIC_LOAD_INACTIVE,
			UI_BUTTON_EVENT_LOAD, UI_EVENT_NONE, false))
		return NULL ;
	if (!createButtonInWidgetArray(widgets, 1, renderer, backR, UI_PIC_BACK,
			UI_PIC_BACK, UI_BUTTON_EVENT_BACK, UI_BUTTON_EVENT_BACK, true))
		return NULL ;
	for (int i = 0; i < numOfGames; i++) {
		if (!createButtonInWidgetArray(widgets, OTHER_BUTTONS_NUM + i, renderer,
				slotR, UI_PIC_SLOT_ACTIVE,
				UI_PIC_SLOT_INACTIVE, UI_EVENT_NONE,
				UI_BUTTON_EVENT_ACTIVATED_SLOT, false))
			return NULL ;
		slotR.y += GAME_DELTA;
	}
	return widgets;
}

static void activateGameSlot(Window* window, int gameSlot) {
	Button* loadButton = (Button*) window->widgets[0]->data;
	buttonSetActive(loadButton, true);
	for (int i = OTHER_BUTTONS_NUM; i < window->numOfWidgets; i++) {
		Button* slotButton = (Button*) window->widgets[i]->data;
		buttonSetActive(slotButton, gameSlot == i);
	}
}

static UI_EVENT loadGameWindowHandleEvent(Window* window, SDL_Event* event) {
	if (window == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	for (int i = 0; i < window->numOfWidgets; i++) {
		UI_EVENT eventResult = window->widgets[i]->handleEvent(
				window->widgets[i], event);
		if (eventResult != UI_EVENT_NONE) {
			if (eventResult == UI_BUTTON_EVENT_ACTIVATED_SLOT)
				activateGameSlot(window, i);
			return eventResult;
		}
	}
	return UI_EVENT_NONE;
}

Window* loadGameWindowCreate(int numOfGames) {
	int numOfWidgets = OTHER_BUTTONS_NUM + numOfGames;
	LoadGameWindowData* data = createLoadGameWindowData(numOfGames);
	if (data == NULL )
		return NULL ;
	Window* window = windowBaseCreate(UI_PIC_DEFAULT_MENU);
	if (window == NULL )
		return NULL ;
	window->numOfWidgets = numOfWidgets;
	window->handleEvent = loadGameWindowHandleEvent;
	window->data = data;
	window->widgets = createLoadGameWindowWidgets(window->renderer, numOfGames);
	if (window->widgets == NULL ) {
		windowDestroy(window);
		return NULL ;
	}
	return window;
}
