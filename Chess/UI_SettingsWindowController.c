#include <stdio.h>
#include <stdlib.h>
#include "UI_SettingsWindow.h"
#include "UI_SettingsWindowController.h"
#include "UI_GameWindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"

/**
 * An event handler for the controller.
 * Update the GameSettings (controller->data) according to the UI_EVENT if needed.
 * Switched controllerPtr content if the back or start buttons were pressed.
 * Translates the UI_EVENT into UI_CONTROLLER_EVENT.
 * @returns
 * If there's no event relevant, returns UI_CONTROLLER_EVENT_NONE.
 * If an error occurred (like memory failure), returns UI_CONTROLLER_EVENT_ERROR.
 * Otherwise returns UI_CONTROLLER_EVENT_INVOKE_DRAW.
 */
static UI_CONTROLLER_EVENT settingsWindowControllerHandleEvent(
		WindowController** controllerPtr, SDL_Event* event) {
	UI_EVENT uiEvent = windowHandleEvent((*controllerPtr)->window, event);
	GameSettings* settings = (GameSettings*)(*controllerPtr)->data;
	if (uiEvent == UI_EVENT_NONE)
		return UI_CONTROLLER_EVENT_NONE;
	switch (uiEvent) {
	case UI_BUTTON_EVENT_START_GAME:
		settings = gameSettingsCopy(settings);
		if (settings == NULL)
			return UI_CONTROLLER_EVENT_ERROR;
		windowControllerDestroy(*controllerPtr);
		*controllerPtr = gameWindowControllerCreate(settings);
		return *controllerPtr != NULL ?
				UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_ERROR;
	case UI_BUTTON_EVENT_BACK:
		windowControllerDestroy(*controllerPtr);
		*controllerPtr = mainWindowControllerCreate();
		return *controllerPtr != NULL ?
				UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_ERROR;
	case UI_BUTTON_EVENT_ONE_PLAYER_MODE:
		gameSettingsChangeGameMode(settings, ONE_PLAYER);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_TWO_PLAYER_MODE:
		gameSettingsChangeGameMode(settings, TWO_PLAYERS);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_AMATEUR_MODE:
		gameSettingsChangeDifficulty(settings, DIFFICULTY_LEVEL_1_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_EASY_MODE:
		gameSettingsChangeDifficulty(settings, DIFFICULTY_LEVEL_2_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_MODERATE_MODE:
		gameSettingsChangeDifficulty(settings, DIFFICULTY_LEVEL_3_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_HARD_MODE:
		gameSettingsChangeDifficulty(settings, DIFFICULTY_LEVEL_4_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_EXPERT_MODE:
		gameSettingsChangeDifficulty(settings, DIFFICULTY_LEVEL_5_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_WHITE_COLOR_MODE:
		gameSettingsChangeUserColor(settings, CHESS_WHITE_PLAYER);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_BLACK_COLOR_MODE:
		gameSettingsChangeUserColor(settings, CHESS_BLACK_PLAYER);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	default:

		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

/**
 * The destroy function for the controller.
 * Calls window destroy and settings destroy and frees the controller afterwards.
 */
static void settingsWindowControllerDestroy(WindowController* controller) {
	if (controller == NULL || controller->window == NULL )
		return;
	windowDestroy(controller->window);
	gameSettingsDestroy((GameSettings*) controller->data);
	free(controller);
}

/**
 * Creates a WindowController type with the relevant Window type.
 * Adds a GameSettings type as it's data.
 * Can have memory failures or SDL errors
 * @returns
 * NULL if some error occurred, otherwise a WindowController* of the settings menu.
 */
WindowController* settingsWindowControllerCreate() {
	GameSettings* settings = gameSettingsCreate();
	if (settings == NULL )
		return NULL ;
	Window* window = settingsWindowCreate();
	if (window == NULL ) {
		gameSettingsDestroy(settings);
		return NULL ;
	}
	WindowController* controller = windowControllerCreate(window, settings,
			settingsWindowControllerHandleEvent,
			settingsWindowControllerDestroy);
	if (controller == NULL ) {
		windowDestroy(window);
		gameSettingsDestroy(settings);
		return NULL ;
	}
	return controller;
}
