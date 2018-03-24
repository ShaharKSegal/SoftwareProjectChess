#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_MainWindow.h"
#include "UI_Button.h"
#include "ChessErrorHandler.h"

const static int NEW_GAME_X = 285;
const static int NEW_GAME_Y = 100;
const static int NEW_GAME_H = 100;
const static int NEW_GAME_W = 250;
const static int MAIN_LOAD_X = 285;
const static int MAIN_LOAD_Y = 250;
const static int MAIN_LOAD_H = 100;
const static int MAIN_LOAD_W = 250;
const static int EXIT_X = 285;
const static int EXIT_Y = 400;
const static int EXIT_H = 100;
const static int EXIT_W = 250;
const static int WIDGETS_NUM = 3;

static UI_EVENT mainWindowHandleEvent(Window* window, SDL_Event* event) {
	if (window == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	for (int i = 0; i < window->numOfWidgets; i++) {
		UI_EVENT eventResult = window->widgets[i]->handleEvent(
				window->widgets[i], event);
		if (eventResult != UI_EVENT_NONE)
			return eventResult;
	}
	return UI_EVENT_NONE;
}

static Widget** mainWindowWidgetsCreate(SDL_Renderer* renderer) {
	if (renderer == NULL )
		return NULL ;
	Widget** widgets = (Widget**) calloc(WIDGETS_NUM, sizeof(Widget*));
	if (widgets == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	// TODO: Change to new format with aux
	SDL_Rect newGameR = { .x = NEW_GAME_X, .y = NEW_GAME_Y, .h = NEW_GAME_H,
			.w = NEW_GAME_W };
	SDL_Rect loadR = { .x = MAIN_LOAD_X, .y = MAIN_LOAD_Y, .h = MAIN_LOAD_H,
			.w = MAIN_LOAD_W };
	SDL_Rect exitR = { .x = EXIT_X, .y = EXIT_Y, .h = EXIT_H, .w = EXIT_W };
	widgets[0] = buttonCreate(renderer, newGameR, UI_PIC_NEW_GAME,
			UI_PIC_NEW_GAME, UI_BUTTON_EVENT_NEW_GAME, UI_EVENT_NONE, true);
	widgets[1] = buttonCreate(renderer, loadR, UI_PIC_LOAD, UI_PIC_LOAD,
			UI_BUTTON_EVENT_LOAD, UI_EVENT_NONE, true);
	widgets[2] = buttonCreate(renderer, exitR, UI_PIC_EXIT, UI_PIC_EXIT,
			UI_BUTTON_EVENT_EXIT, UI_EVENT_NONE, true);
	if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL ) {
		widgetDestroy(widgets[0]);
		widgetDestroy(widgets[1]);
		widgetDestroy(widgets[2]);
		free(widgets);
		return NULL ;
	}
	return widgets;
}

Window* mainWindowCreate() {
	Window* window = windowBaseCreate(UI_PIC_DEFAULT_MENU);
	if (window == NULL )
		return NULL ;
	window->numOfWidgets = WIDGETS_NUM;
	window->handleEvent = mainWindowHandleEvent;
	window->widgets = mainWindowWidgetsCreate(window->renderer);
	if (window->widgets == NULL ) {
		windowDestroy(window);
		return NULL ;
	}
	return window;
}
