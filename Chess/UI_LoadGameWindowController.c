#include <stdio.h>
#include <stdlib.h>
#include "UI_LoadGameWindow.h"
#include "UI_LoadGameWindowController.h"
#include "UI_GameWindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"

const static char* saveFileTemplate = "./save%d";
const static int saveFileMaxChars = 128;
const static int MAX_SAVES = 5;

typedef struct load_game_controller_data_t {
	int numOfSaves;
	int currentPage;
	GameSettings* previousGameSettings;
	GameSettings* loadedGameSettings;
	UI_MODE previousMode;
} LoadGameWindowControllerData;

static int getNumberOfSaves() {
	return MAX_SAVES;
}

static LoadGameWindowControllerData* getLoadGameWindowControllerData(
		WindowController* controller) {
	return (LoadGameWindowControllerData*) controller->data;
}

static LoadGameWindowControllerData* createLoadGameWindowControllerData(
		GameSettings* settings, UI_MODE previousMode) {
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
	case UI_MAIN_MODE:
		controller = mainWindowControllerCreate();
		break;
	case UI_GAME_MODE:
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
		return handleEventBack(controllerPtr);
	case UI_BUTTON_EVENT_NEXT_PAGE:
		return handleEventChangePage(*controllerPtr, true);
	case UI_BUTTON_EVENT_PREVIOUS_PAGE:
		return handleEventChangePage(*controllerPtr, false);
	default:
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
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
