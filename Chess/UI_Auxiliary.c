#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_Button.h"
#include "ChessErrorHandler.h"

const static char* TITLE_GAME_STATE = "Game State";
const static char* TITLE_WARNING = "Warning";
const static char* MESSAGE_UNSAVED =
		"Unsaved changes detected, would you like to save the game?";
const static char* MESSAGE_CHECK = "Check!";
const static char* MESSAGE_CHECKMATE = "Checkmate!";
const static char* MESSAGE_DRAW = "Draw!";
const static SDL_MessageBoxButtonData BUTTONS[] = { { 0, UI_MSGBOX_EVENT_NO,
		"no" }, { 0, UI_MSGBOX_EVENT_YES, "yes" }, { 0, UI_MSGBOX_EVENT_CANCEL,
		"cancel" }, };
const static SDL_MessageBoxColorScheme COLOR_SCHEME = { { { 255, 0, 0 }, { 0,
		255, 0 }, { 255, 255, 0 }, { 0, 0, 255 }, { 255, 0, 255 } } };

/**
 * Adds a new button to the widgets array in the given index.
 * Handles failures and frees memory in the widget array.
 * NOTE: assumes the given index is the end of the actual size of the array.
 */
bool createButtonInWidgetArray(Widget** widgets, int index,
		SDL_Renderer* renderer, SDL_Rect location, const char* activeImage,
		const char* inactiveImage, UI_EVENT eventActive, UI_EVENT eventInactive,
		bool isActive) {
	widgets[index] = buttonCreate(renderer, location, activeImage,
			inactiveImage, eventActive, eventInactive, isActive);
	if (widgets[index] == NULL ) {
		widgetListDestory(widgets, index);
		return false;
	}
	return true;
}

/**
 * Popup message box for unsaved changes, asking the user approval.
 * @return:
 * UI_MSGBOX_EVENT_YES - The controller will change to the save screen.
 * UI_MSGBOX_EVENT_NO - Ignore unsaved changes and continue.
 * UI_MSGBOX_EVENT_CANCEL - Go back to the window as if nothing happened.
 */
UI_EVENT unsavedChangesPopup() {
	const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_WARNING, NULL,
			TITLE_WARNING, MESSAGE_UNSAVED, SDL_arraysize(BUTTONS), BUTTONS,
			&COLOR_SCHEME };
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		hadSDLError();
		return UI_EVENT_ERROR;
	}
	if (buttonid == -1)
		return UI_EVENT_NONE;
	return (UI_EVENT) buttonid;
}

/**
 * Popup message box for the game state. Indicating check, checkmate or draw.
 * @return:
 * See chessGameGeCurrentState in ChessGame.h for details
 */
CHESS_GAME_MESSAGE chessGameStatePopup(ChessGame* game) {
	if (game == NULL )
		return CHESS_GAME_NONE;
	CHESS_GAME_MESSAGE gameMsg = chessGameGetCurrentState(game);
	const char* msg = NULL;
	switch (gameMsg) {
	case CHESS_GAME_CHECK:
		msg = MESSAGE_CHECK;
		break;
	case CHESS_GAME_CHECKMATE:
		msg = MESSAGE_CHECKMATE;
		break;
	case CHESS_GAME_DRAW:
		msg = MESSAGE_DRAW;
		break;
	default:
		break;
	}
	if (msg != NULL )
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, TITLE_GAME_STATE,
				msg, NULL );
	return gameMsg;
}
