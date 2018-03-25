#include <stdio.h>
#include <stdlib.h>
#include "UI_SettingsWindow.h"
#include "UI_SettingsWindowController.h"
#include "UI_GameWindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"

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
		changeGameMode(settings, ONE_PLAYER);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_TWO_PLAYER_MODE:
		changeGameMode(settings, TWO_PLAYERS);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_AMATEUR_MODE:
		changeDifficulty(settings, DIFFICULTY_LEVEL_1_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_EASY_MODE:
		changeDifficulty(settings, DIFFICULTY_LEVEL_2_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_MODERATE_MODE:
		changeDifficulty(settings, DIFFICULTY_LEVEL_3_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_HARD_MODE:
		changeDifficulty(settings, DIFFICULTY_LEVEL_4_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_EXPERT_MODE:
		changeDifficulty(settings, DIFFICULTY_LEVEL_5_INT);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_WHITE_COLOR_MODE:
		changeUserColor(settings, CHESS_WHITE_PLAYER);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_BLACK_COLOR_MODE:
		changeUserColor(settings, CHESS_BLACK_PLAYER);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	default:

		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

static void settingsWindowControllerDestroy(WindowController* controller) {
	if (controller == NULL || controller->window == NULL )
		return;
	windowDestroy(controller->window);
	GameSettingsDestroy((GameSettings*) controller->data);
	free(controller);
}

WindowController* settingsWindowControllerCreate() {
	GameSettings* settings = GameSettingsCreate();
	if (settings == NULL )
		return NULL ;
	Window* window = settingsWindowCreate();
	if (window == NULL )
		return NULL ;
	WindowController* controller = windowControllerCreate(window, settings,
			settingsWindowControllerHandleEvent,
			settingsWindowControllerDestroy);
	if (controller == NULL ) {
		windowDestroy(window);
		GameSettingsDestroy(settings);
		return NULL ;
	}
	return controller;
}
