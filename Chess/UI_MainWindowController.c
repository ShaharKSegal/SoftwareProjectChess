#include <stdio.h>
#include <stdlib.h>
#include "UI_MainWindow.h"
#include "UI_MainWindowController.h"
#include "UI_LoadGameWindowController.h"
#include "UI_SettingsWindowController.h"
#include "ChessErrorHandler.h"

static UI_CONTROLLER_EVENT mainWindowControllerHandleEvent(
		WindowController** controllerPtr, SDL_Event* event) {
	UI_EVENT uiEvent = windowHandleEvent((*controllerPtr)->window, event);
	if (uiEvent == UI_EVENT_NONE)
		return UI_CONTROLLER_EVENT_NONE;
	switch (uiEvent) {
	case UI_BUTTON_EVENT_EXIT:
		return UI_CONTROLLER_EVENT_QUTT;
	case UI_BUTTON_EVENT_NEW_GAME:
		windowControllerDestroy(*controllerPtr);
		*controllerPtr = settingsWindowControllerCreate();
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_LOAD:
		windowControllerDestroy(*controllerPtr);
		*controllerPtr = loadGameWindowControllerCreate(NULL, UI_MAIN_MODE);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	default:
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

WindowController* mainWindowControllerCreate() {
	Window* window = mainWindowCreate();
	if (window == NULL )
		return NULL ;
	WindowController* controller = windowControllerCreate(window, NULL,
			mainWindowControllerHandleEvent, NULL );
	if (controller == NULL ) {
		windowDestroy(window);
		return NULL ;
	}
	return controller;
}
