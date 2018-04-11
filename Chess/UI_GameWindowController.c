#include <stdio.h>
#include <stdlib.h>
#include "UI_Auxiliary.h"
#include "UI_GameWindow.h"
#include "UI_GameWindowController.h"
#include "UI_LoadGameWindowController.h"
#include "UI_MainWindowController.h"
#include "Minimax.h"
#include "ChessErrorHandler.h"

typedef struct chess_game_controller_data_t {
	GameSettings* gameSettings;
	bool unsavedChanges;
	bool hasHighlights;

} GameWindowControllerData;

static GameWindowControllerData* getGameWindowControllerData(
		WindowController* controller) {
	return (GameWindowControllerData*) controller->data;
}

static void setUnsavedChanges(WindowController* controller, bool value) {
	getGameWindowControllerData(controller)->unsavedChanges = value;
}

static GameWindowControllerData* createGameWindowControllerData(
		GameSettings* settings) {
	GameWindowControllerData* data = malloc(sizeof(GameWindowControllerData));
	if (data == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	data->gameSettings = settings;
	data->unsavedChanges = false;
	data->hasHighlights = false;
	return data;
}

static void destroyGameWindowControllerData(GameWindowControllerData* data) {
	if (data == NULL)
		return;
	gameSettingsDestroy(data->gameSettings);
	free(data);
}

static void gameWindowControllerDestroy(WindowController* controller) {
	if (controller == NULL || controller->window == NULL)
		return;
	windowDestroy(controller->window);
	destroyGameWindowControllerData(controller->data);
	free(controller);
}

static UI_CONTROLLER_EVENT handleEventRestart(WindowController* controller) {
	GameSettings* settings =
			getGameWindowControllerData(controller)->gameSettings;
	ChessGame* game = chessGameCreate();
	if (game == NULL)
		return UI_CONTROLLER_EVENT_ERROR;
	Window* window = controller->window;
	GameWindowData* winData = gameWindowGetData(controller->window);
	winData->game = game;
	bool res = gameWindowRefreshWidgets(window);
	if (!res) {
		chessGameDestroy(game);
		return UI_CONTROLLER_EVENT_ERROR;
	}
	chessGameDestroy(settings->chessGame);
	settings->chessGame = game;
	setUnsavedChanges(controller, false);
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT handleEventLoad(WindowController** controllerPtr) {
	GameWindowControllerData* data = getGameWindowControllerData(
			*controllerPtr);
	GameSettings* settings = gameSettingsCopy(data->gameSettings);
	if (settings == NULL)
		return UI_CONTROLLER_EVENT_ERROR;
	WindowController* controller = loadGameWindowControllerCreate(settings,
			UI_GAME_CONTROLLER);
	if (controller == NULL)
		return UI_CONTROLLER_EVENT_ERROR;
	windowControllerDestroy(*controllerPtr);
	*controllerPtr = controller;
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT handleEventSave(WindowController** controllerPtr,
		UI_CONTROLLER nextMode) {
	GameWindowControllerData* data = getGameWindowControllerData(
			*controllerPtr);
	GameSettings* settings = gameSettingsCopy(data->gameSettings);
	if (settings == NULL)
		return UI_CONTROLLER_EVENT_ERROR;
	WindowController* controller = saveGameWindowControllerCreate(settings,
			UI_GAME_CONTROLLER, nextMode);
	if (controller == NULL)
		return UI_CONTROLLER_EVENT_ERROR;
	windowControllerDestroy(*controllerPtr);
	*controllerPtr = controller;
	setUnsavedChanges(*controllerPtr, false);
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT handleEventUndo(WindowController* controller) {
	ChessGame* game =
			getGameWindowControllerData(controller)->gameSettings->chessGame;
	CHESS_GAME_MESSAGE msg = chessGameUndoMove(game);
	if (msg == CHESS_GAME_SUCCESS) {
		bool res = gameWindowRefreshWidgets(controller->window);
		if (!res)
			return UI_CONTROLLER_EVENT_ERROR;
		setUnsavedChanges(controller, true);
	}
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT handleEventBackToMainMenu(
		WindowController** controllerPtr) {
	if (!getGameWindowControllerData(*controllerPtr)->unsavedChanges) {
		windowControllerDestroy(*controllerPtr);
		*controllerPtr = mainWindowControllerCreate();
	} else {
		UI_EVENT event = unsavedChangesPopup();
		UI_CONTROLLER_EVENT res;
		switch (event) {
		case UI_EVENT_ERROR:
			return UI_CONTROLLER_EVENT_ERROR;
		case UI_MSGBOX_EVENT_YES:
			res = handleEventSave(controllerPtr, UI_MAIN_CONTROLLER);
			break;
		case UI_MSGBOX_EVENT_NO:
			windowControllerDestroy(*controllerPtr);
			*controllerPtr = mainWindowControllerCreate();
			break;
		default: //Include UI_MSGBOX_EVENT_CANCEL, UI_EVENT_NONE.
			res = UI_CONTROLLER_EVENT_INVOKE_DRAW;
			break;
		}
		if (res == UI_CONTROLLER_EVENT_ERROR)
			return res;
	}
	return (*controllerPtr) != NULL ?
			UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_ERROR;
}

static UI_CONTROLLER_EVENT handleEventExit(WindowController** controllerPtr) {
	if (!getGameWindowControllerData(*controllerPtr)->unsavedChanges)
		return UI_CONTROLLER_EVENT_QUTT;
	UI_EVENT event = unsavedChangesPopup();
	switch (event) {
	case UI_EVENT_ERROR:
		return UI_CONTROLLER_EVENT_ERROR;
	case UI_MSGBOX_EVENT_YES:
		return handleEventSave(controllerPtr, UI_QUIT_CONTROLLER);
		break;
	case UI_MSGBOX_EVENT_NO:
		return UI_CONTROLLER_EVENT_QUTT;
	default: //Include UI_MSGBOX_EVENT_CANCEL, UI_EVENT_NONE.
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

static UI_CONTROLLER_EVENT handleEventGetMoves(WindowController* controller) {
	GameWindowControllerData* data = getGameWindowControllerData(controller);
	ChessGame* game = data->gameSettings->chessGame;
	GameWindowData* winData = gameWindowGetData(controller->window);
	ArrayList* moves = chessGameGetMoves(game, winData->sourcePos);
	if (moves == NULL) {
		if (getHadMemoryFailure())
			return UI_CONTROLLER_EVENT_ERROR;
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
	if (!gameWindowAddHighlightMoves(controller->window, moves))
		return UI_CONTROLLER_EVENT_ERROR;
	data->hasHighlights = true;
	return UI_CONTROLLER_EVENT_INVOKE_DRAW;
}

static UI_CONTROLLER_EVENT handleEventPieceDrag(WindowController* controller) {
	GameWindowControllerData* data = getGameWindowControllerData(controller);
	ChessPiecePosition sourcePos =
			gameWindowGetData(controller->window)->sourcePos;
	ChessPiecePosition targetPos =
			gameWindowGetData(controller->window)->targetPos;
	ChessGame* game = data->gameSettings->chessGame;
	CHESS_GAME_MESSAGE msg = chessGameSetMove(game, sourcePos, targetPos);
	if (msg == CHESS_GAME_SUCCESS) {
		setUnsavedChanges(controller, true);
		msg = chessGameStatePopup(game);
		if (msg == CHESS_GAME_NONE || msg == CHESS_GAME_CHECK) {
			if (data->gameSettings->gameMode == ONE_PLAYER) {
				ChessMove move = chessGameMinimax(data->gameSettings);
				if (getHadCriticalError())
					return UI_CONTROLLER_EVENT_ERROR;
				chessGameSetMove(game, move.previousPosition,
						move.currentPosition);
				msg = chessGameStatePopup(game);
			}
		}
	}
	bool res = gameWindowRefreshWidgets(controller->window);
	return res ? UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_ERROR;
}

static UI_CONTROLLER_EVENT gameWindowControllerHandleEvent(
		WindowController** controllerPtr, SDL_Event* event) {
	GameWindowControllerData* data = getGameWindowControllerData(
			*controllerPtr);
	UI_EVENT uiEvent = windowHandleEvent((*controllerPtr)->window, event);
	bool hadhighLights = data->hasHighlights;
	if (uiEvent == UI_EVENT_NONE)
		return hadhighLights ?
				UI_CONTROLLER_EVENT_INVOKE_DRAW : UI_CONTROLLER_EVENT_NONE;
	if (hadhighLights)
		gameWindowRemoveHighlightMoves((*controllerPtr)->window);
	switch (uiEvent) {
	case UI_BUTTON_EVENT_RESTART:
		return handleEventRestart(*controllerPtr);
	case UI_BUTTON_EVENT_LOAD:
		return handleEventLoad(controllerPtr);
	case UI_BUTTON_EVENT_SAVE:
		return handleEventSave(controllerPtr, UI_GAME_CONTROLLER);
	case UI_BUTTON_EVENT_UNDO:
		return handleEventUndo(*controllerPtr);
	case UI_BUTTON_EVENT_MAIN_MENU_SCREEN:
		return handleEventBackToMainMenu(controllerPtr);
	case UI_BUTTON_EVENT_EXIT:
		return handleEventExit(controllerPtr);
	case UI_BUTTON_EVENT_GET_MOVES:
		return handleEventGetMoves(*controllerPtr);
	case UI_BUTTON_EVENT_PIECE_DRAG:
		return handleEventPieceDrag(*controllerPtr);
	default:
		return UI_CONTROLLER_EVENT_INVOKE_DRAW;
	}
}

WindowController* gameWindowControllerCreate(GameSettings* settings) {
	GameWindowControllerData* data = createGameWindowControllerData(settings);
	if (data == NULL)
		return NULL;
	Window* window = gameWindowCreate((ChessGame*) settings->chessGame);
	if (window == NULL)
		return NULL;
	WindowController* controller = windowControllerCreate(window, data,
			gameWindowControllerHandleEvent, gameWindowControllerDestroy);
	if (controller == NULL) {
		windowDestroy(window);
		destroyGameWindowControllerData(data);
		return NULL;
	}
	return controller;
}
