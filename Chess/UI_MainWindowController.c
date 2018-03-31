#include <stdio.h>
#include <stdlib.h>
#include "UI_MainWindow.h"
#include "UI_MainWindowController.h"
#include "UI_LoadGameWindowController.h"
#include "UI_SettingsWindowController.h"
#include "ChessErrorHandler.h"

/**
 * An event handler for the controller.
 * Translates the UI_EVENT into UI_CONTROLLER_EVENT.
 * @returns
 * If there's no event relevant, returns UI_CONTROLLER_EVENT_NONE.
 * If the exit button was pressed, returns UI_CONTROLLER_EVENT_QUTT.
 * Otherwise returns UI_CONTROLLER_EVENT_INVOKE_DRAW.
 */
static UI_CONTROLLER_EVENT mainWindowControllerHandleEvent(
		WindowController** controllerPtr, SDL_Event* event) {
	UI_EVENT uiEvent = windowHandleEvent((*controllerPtr)->window, event);
	WindowController* controller;
	if (uiEvent == UI_EVENT_NONE)
		return UI_CONTROLLER_EVENT_NONE;
	switch (uiEvent) {
	case UI_BUTTON_EVENT_EXIT:
		return UI_CONTROLLER_EVENT_QUTT;
	case UI_BUTTON_EVENT_NEW_GAME:
		controller = settingsWindowControllerCreate();
		if (controller == NULL )
			return UI_CONTROLLER_EVENT_ERROR;
		windowControllerDestroy(*controllerPtr);
		*controllerPtr = controller;
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_LOAD:
		controller = loadGameWindowControllerCreate(NULL, UI_MAIN_CONTROLLER);
		if (controller == NULL )
			return UI_CONTROLLER_EVENT_ERROR;
		windowControllerDestroy(*controllerPtr);
		*controllerPtr = controller;
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	default:
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

/**
 * Creates a WindowController type with the relevant Window type.
 * Can have memory failures or SDL errors
 * @returns
 * NULL if some error occurred, otherwise a WindowController* of the main menu.
 */
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
