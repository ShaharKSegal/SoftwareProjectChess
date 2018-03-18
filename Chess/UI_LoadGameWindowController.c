#include <stdio.h>
#include <stdlib.h>
#include "UI_LoadGameWindow.h"
#include "UI_LoadGameWindowController.h"
#include "UI_GameWindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"

const static char* saveFileTemplate = "./save%d";
const static int saveFileMaxChars = 128;

typedef struct load_game_controller_data_t {
	GameSettings* gameSettings;
	UI_MODE previousMode;
} LoadGameWindowControllerData;

static LoadGameWindowControllerData* createLoadGameWindowControllerData(
		GameSettings* settings, UI_MODE previousMode) {
	LoadGameWindowControllerData* data = malloc(
			sizeof(LoadGameWindowControllerData));
	if (data == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	data->gameSettings = settings;
	data->previousMode = previousMode;
	return data;
}

static void destroyLoadGameWindowControllerData(
		LoadGameWindowControllerData* data) {
	if (data == NULL )
		return;
	GameSettingsDestroy(data->gameSettings);
	free(data);
}

static void loadGameWindowControllerDestroy(WindowController* controller) {
	if (controller == NULL || controller->window == NULL )
		return;
	windowDestroy(controller->window);
	destroyLoadGameWindowControllerData(controller->data);
	free(controller);
}

static bool handleEventBack(WindowController** controllerPtr) {
	LoadGameWindowControllerData* controllerData =
			(LoadGameWindowControllerData*) (*controllerPtr)->data;
	UI_MODE previousMode = controllerData->previousMode;
	GameSettings* settings = gameSettingsCopy(controllerData->gameSettings);
	if (settings == NULL )
		return false;
	windowControllerDestroy(*controllerPtr);
	if (previousMode == UI_MAIN_MODE)
		*controllerPtr = mainWindowControllerCreate();
	else if (previousMode == UI_GAME_MODE)
		*controllerPtr = gameWindowControllerCreate(settings);
	return (*controllerPtr) != NULL ;
}

static UI_CONTROLLER_EVENT loadGameWindowControllerHandleEvent(
		WindowController** controllerPtr, SDL_Event* event) {
	UI_EVENT uiEvent = windowHandleEvent((*controllerPtr)->window, event);
	char saveFile[saveFileMaxChars];
	int slot =
			((LoadGameWindowData*) ((*controllerPtr)->window->data))->activeSlot;
	if (uiEvent == UI_EVENT_NONE)
			return UI_CONTROLLER_EVENT_NONE;
	switch (uiEvent) {
	case UI_BUTTON_EVENT_ACTIVATED_SLOT:
		sprintf(saveFile, saveFileTemplate, slot);
		//TODO: Load file and change settings.
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_LOAD:
		//TODO: Change to game controller (create).
		windowControllerDestroy(*controllerPtr);
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	case UI_BUTTON_EVENT_BACK:
		return handleEventBack(controllerPtr) ?
				UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_ERROR;
	default:
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

static int getNumberOfSaves() {
	// TODO: implement generic
	return 5;
}

WindowController* loadGameWindowControllerCreate(GameSettings* settings,
		UI_MODE previousMode) {
	LoadGameWindowControllerData* data = createLoadGameWindowControllerData(
			settings, previousMode);
	if (data == NULL )
		return NULL ;
	Window* window = loadGameWindowCreate(getNumberOfSaves());
	if (window == NULL )
		return NULL ;
	WindowController* controller = windowControllerCreate(window, data,
			loadGameWindowControllerHandleEvent,
			loadGameWindowControllerDestroy);
	if (controller == NULL ) {
		windowDestroy(window);
		free(data);
		return NULL ;
	}
	return controller;
}
