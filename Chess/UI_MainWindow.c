#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_MainWindow.h"
#include "UI_Button.h"
#include "ChessErrorHandler.h"

/**
 * Constants for the widgets location.
 */
static const int NEW_GAME_X = 285;
static const int NEW_GAME_Y = 100;
static const int NEW_GAME_H = 100;
static const int NEW_GAME_W = 250;
static const int MAIN_LOAD_X = 285;
static const int MAIN_LOAD_Y = 250;
static const int MAIN_LOAD_H = 100;
static const int MAIN_LOAD_W = 250;
static const int EXIT_X = 285;
static const int EXIT_Y = 400;
static const int EXIT_H = 100;
static const int EXIT_W = 250;
static const int WIDGETS_NUM = 3;

/**
 * An event handler for the main menu. Invoke event handler of each widget to
 * determine if the SDL_Event is relevant to one of them.
 * @returns
 * UI_EVENT, based on the button which was pressed. If none of the widgets
 * were clicked, returns UI_EVENT_NONE.
 */
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

/**
 * Creates a widget array with the new game, load and exit buttons.
 * Can have memory failures
 * @returns
 * Widget array, or NULL if an error occurred.
 */
static Widget** mainWindowWidgetsCreate(SDL_Renderer* renderer) {
	if (renderer == NULL )
		return NULL ;
	Widget** widgets = (Widget**) calloc(WIDGETS_NUM, sizeof(Widget*));
	if (widgets == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	SDL_Rect newGameR = { .x = NEW_GAME_X, .y = NEW_GAME_Y, .h = NEW_GAME_H,
			.w = NEW_GAME_W };
	SDL_Rect loadR = { .x = MAIN_LOAD_X, .y = MAIN_LOAD_Y, .h = MAIN_LOAD_H,
			.w = MAIN_LOAD_W };
	SDL_Rect exitR = { .x = EXIT_X, .y = EXIT_Y, .h = EXIT_H, .w = EXIT_W };
	int i = 0;
	if (!createButtonInWidgetArray(widgets, i, renderer, newGameR,
			UI_PIC_NEW_GAME, UI_PIC_NEW_GAME, UI_BUTTON_EVENT_NEW_GAME,
			UI_EVENT_NONE, true))
		return false;
	i++;
	if (!createButtonInWidgetArray(widgets, i, renderer, loadR, UI_PIC_LOAD,
			UI_PIC_LOAD, UI_BUTTON_EVENT_LOAD, UI_EVENT_NONE, true))
		return false;
	i++;
	if (!createButtonInWidgetArray(widgets, i, renderer, exitR, UI_PIC_EXIT,
			UI_PIC_EXIT, UI_BUTTON_EVENT_EXIT, UI_EVENT_NONE, true))
		return false;
	i++;
	return widgets;
}

/**
 * Creates a Window type with widgets and operations of the main menu
 * Can have memory failures or SDL errors
 * @returns
 * NULL if some error occurred, otherwise a Window* of the main menu.
 */
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
