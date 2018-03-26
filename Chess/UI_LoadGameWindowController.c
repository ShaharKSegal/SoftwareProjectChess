#include <stdio.h>
#include <stdlib.h>
#include "ChessErrorHandler.h"
#include "LoadGame.h"
#include "SaveGame.h"
#include "UI_LoadGameWindow.h"
#include "UI_LoadGameWindowController.h"
#include "UI_GameWindowController.h"
#include "UI_MainWindowController.h"

#define SAVE_FILE_MAX_PATH 128

static const char* saveFileTemplate = "./save%d";
static const int MAX_SAVES = 5;

typedef struct load_game_controller_data_t {
	int numOfSaves;
	int currentPage;
	char filePath[SAVE_FILE_MAX_PATH];
	GameSettings* previousGameSettings;
	GameSettings* loadedGameSettings;
	UI_CONTROLLER previousMode;
	UI_CONTROLLER currentMode;
	UI_CONTROLLER nextMode;
} LoadGameWindowControllerData;

static int getNumberOfSaves() {
	return MAX_SAVES;
}

static LoadGameWindowControllerData* getLoadGameWindowControllerData(
		WindowController* controller) {
	return (LoadGameWindowControllerData*) controller->data;
}

static LoadGameWindowControllerData* createLoadGameWindowControllerData(
		GameSettings* settings, UI_CONTROLLER previousMode,
		UI_CONTROLLER currentMode, UI_CONTROLLER nextMode) {
	if (currentMode != UI_LOAD_CONTROLLER && currentMode != UI_SAVE_CONTROLLER)
		return NULL ;
	if (previousMode != UI_GAME_CONTROLLER
			&& previousMode != UI_MAIN_CONTROLLER)
		return NULL ;
	LoadGameWindowControllerData* data = malloc(
			sizeof(LoadGameWindowControllerData));
	if (data == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	data->numOfSaves = getNumberOfSaves();
	data->currentPage = 0;
	data->previousGameSettings = settings;
	data->loadedGameSettings = NULL;
	data->previousMode = previousMode;
	data->currentMode = currentMode;
	data->nextMode = nextMode;
	return data;
}

static void destroyLoadGameWindowControllerData(
		LoadGameWindowControllerData* data) {
	if (data == NULL )
		return;
	GameSettingsDestroy(data->loadedGameSettings);
	free(data);
}

static void loadGameWindowControllerDestroy(WindowController* controller) {
	if (controller == NULL || controller->window == NULL )
		return;
	windowDestroy(controller->window);
	destroyLoadGameWindowControllerData(controller->data);
	free(controller);
}

static UI_CONTROLLER_EVENT handleEventBack(WindowController** controllerPtr) {
	LoadGameWindowControllerData* controllerData =
			(LoadGameWindowControllerData*) (*controllerPtr)->data;
	GameSettings* settings = NULL;
	WindowController* controller = NULL;
	switch (controllerData->previousMode) {
	case UI_MAIN_CONTROLLER:
		controller = mainWindowControllerCreate();
		break;
	case UI_GAME_CONTROLLER:
		settings = gameSettingsCopy(controllerData->previousGameSettings);
		if (settings == NULL )
			return false;
		controller = gameWindowControllerCreate(settings);
		break;
	default:
		return UI_CONTROLLER_EVENT_ERROR;
	}
	windowControllerDestroy(*controllerPtr);
	*controllerPtr = controller;
	return (*controllerPtr) != NULL ?
			UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_ERROR;
}

static UI_CONTROLLER_EVENT handleEventChangePage(WindowController* controller,
		bool isNext) {
	LoadGameWindowControllerData* data = getLoadGameWindowControllerData(
			controller);
	if (isNext)
		data->currentPage++;
	else
		data->currentPage--;
	bool res = loadGameWindowChangePage(controller->window, data->currentPage);
	return res ? UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_ERROR;
}

static UI_CONTROLLER_EVENT handleEventActivateSlot(WindowController* controller) {
	LoadGameWindowControllerData* data = getLoadGameWindowControllerData(
			controller);
	int slot = ((LoadGameWindowData*) (controller->window->data))->activeSlot;
	sprintf(data->filePath, saveFileTemplate, slot);
	if (data->currentMode == UI_LOAD_CONTROLLER) {
		GameSettings* settings = gameSettingsCreateAndLoad(data->filePath);
		if (settings == NULL )
			return UI_CONTROLLER_EVENT_MINOR_ERROR;
		data->loadedGameSettings = settings;
	}
	loadGameActivateSlot(controller->window);
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT handleEventLoad(WindowController** controllerPtr) {
	LoadGameWindowControllerData* data = getLoadGameWindowControllerData(
			*controllerPtr);
	GameSettings* settings = gameSettingsCopy(data->loadedGameSettings);
		if (settings == NULL )
			return UI_CONTROLLER_EVENT_ERROR;
	WindowController* controller = gameWindowControllerCreate(
			settings);
	if (controller == NULL )
		return UI_CONTROLLER_EVENT_ERROR;
	windowControllerDestroy(*controllerPtr);
	*controllerPtr = controller;
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT handleEventSave(WindowController** controllerPtr) {
	WindowController* controller = NULL;
	GameSettings* settings = NULL;
	LoadGameWindowControllerData* data = getLoadGameWindowControllerData(
			*controllerPtr);
	GAME_SETTINGS_MESSAGE msg = chessGameSave(data->filePath,
			data->previousGameSettings);
	if (msg == GAME_SETTINGS_SAVE_GAME_FAIL) {
		return UI_CONTROLLER_EVENT_MINOR_ERROR;
	}
	switch (data->nextMode) {
	case UI_GAME_CONTROLLER:
		settings = gameSettingsCopy(data->previousGameSettings);
		if (settings == NULL )
				return UI_CONTROLLER_EVENT_ERROR;
		controller = gameWindowControllerCreate(settings);
		break;
	case UI_MAIN_CONTROLLER:
		controller = mainWindowControllerCreate();
		break;
	case UI_QUIT_CONTROLLER:
		return UI_CONTROLLER_EVENT_QUTT;
	default:
		break;
	}
	if (controller == NULL )
		return UI_CONTROLLER_EVENT_ERROR;
	windowControllerDestroy(*controllerPtr);
	*controllerPtr = controller;
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT loadGameWindowControllerHandleEvent(
		WindowController** controllerPtr, SDL_Event* event) {
	UI_EVENT uiEvent = windowHandleEvent((*controllerPtr)->window, event);
	if (uiEvent == UI_EVENT_NONE)
		return UI_CONTROLLER_EVENT_NONE;
	switch (uiEvent) {
	case UI_BUTTON_EVENT_ACTIVATED_SLOT:
		return handleEventActivateSlot(*controllerPtr);
	case UI_BUTTON_EVENT_LOAD:
		return handleEventLoad(controllerPtr);
	case UI_BUTTON_EVENT_SAVE:
		return handleEventSave(controllerPtr);
	case UI_BUTTON_EVENT_BACK:
		return handleEventBack(controllerPtr);
	case UI_BUTTON_EVENT_NEXT_PAGE:
		return handleEventChangePage(*controllerPtr, true);
	case UI_BUTTON_EVENT_PREVIOUS_PAGE:
		return handleEventChangePage(*controllerPtr, false);
	default:
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

WindowController* createLoadOrSaveGameWindowController(
		GameSettings* previousSettings, UI_CONTROLLER previousMode,
		UI_CONTROLLER currentMode, UI_CONTROLLER nextMode) {
	LoadGameWindowControllerData* data = createLoadGameWindowControllerData(
			previousSettings, previousMode, currentMode, nextMode);
	if (data == NULL )
		return NULL ;
	Window* window = loadGameWindowCreate(currentMode == UI_SAVE_CONTROLLER,
			getNumberOfSaves());
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

WindowController* loadGameWindowControllerCreate(GameSettings* settings,
		UI_CONTROLLER previousMode) {
	return createLoadOrSaveGameWindowController(settings, previousMode,
			UI_LOAD_CONTROLLER, UI_GAME_CONTROLLER);
}

WindowController* saveGameWindowControllerCreate(GameSettings* settings,
		UI_CONTROLLER previousMode, UI_CONTROLLER nextMode) {
	return createLoadOrSaveGameWindowController(settings, previousMode,
			UI_SAVE_CONTROLLER, nextMode);
}
