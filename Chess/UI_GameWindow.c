#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_GameWindow.h"
#include "UI_Button.h"
#include "ChessGame.h"
#include "ChessErrorHandler.h"

#define BASE_X 281
#define BASE_Y 48
#define BASE_H 63
#define BASE_W 63
#define SQUARE_AXIS 61

#define OTHER_X 30
#define OTHER_Y 50
#define OTHER_H 53
#define OTHER_W 190
#define OTHER_Y_DELTA 60

#define HIGHLIGHT_DELTA 5

const static int OTHER_BUTTONS_NUM = 6;

static int getNumberOfPieces(ChessGame* game) {
	int count = 0;
	for (int i = 0; i < CHESS_N_ROWS; i++) {
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			if (game->gameBoard.position[i][j].type != CHESS_PIECE_EMPTY)
				count++;
		}
	}
	return count;
}

static int getNumberOfWidgets(GameWindowData* data) {
	return data->numOfHighlightMoves + data->numOfPieces + OTHER_BUTTONS_NUM;
}

static char* getPiecePicturePath(ChessPiece piece) {
	int player = piece.player;
	switch (piece.type) {
	case CHESS_PIECE_PAWN:
		return player == CHESS_WHITE_PLAYER ?
				UI_PIC_WHITE_PAWN : UI_PIC_BLACK_PAWN;
	case CHESS_PIECE_BISHOP:
		return player == CHESS_WHITE_PLAYER ?
				UI_PIC_WHITE_BISHOP : UI_PIC_BLACK_BISHOP;
	case CHESS_PIECE_KNIGHT:
		return player == CHESS_WHITE_PLAYER ?
				UI_PIC_WHITE_KNIGHT : UI_PIC_BLACK_KNIGHT;
	case CHESS_PIECE_ROOK:
		return player == CHESS_WHITE_PLAYER ?
				UI_PIC_WHITE_ROOK : UI_PIC_BLACK_ROOK;
	case CHESS_PIECE_QUEEN:
		return player == CHESS_WHITE_PLAYER ?
				UI_PIC_WHITE_QUEEN : UI_PIC_BLACK_QUEEN;
	case CHESS_PIECE_KING:
		return player == CHESS_WHITE_PLAYER ?
				UI_PIC_WHITE_KING : UI_PIC_BLACK_KING;
	default:
		return NULL ;
	}
}

static ChessPiecePosition coordinatesToPiecePosition(int x, int y) {
	ChessPiecePosition pos;
	pos.row = CHESS_N_ROWS - 1 + (BASE_Y - y) / SQUARE_AXIS;
	pos.column = (x - BASE_X) / SQUARE_AXIS;
	return pos;
}

static SDL_Rect piecePositionToRect(ChessPiecePosition pos) {
	SDL_Rect rect = { .x = BASE_X + SQUARE_AXIS * pos.column, .y = BASE_Y
			+ SQUARE_AXIS * (CHESS_N_ROWS - (1 + pos.row)), .h = BASE_H, .w =
			BASE_W };
	return rect;
}

static void setWidgetByPosition(Window* window, ChessPiecePosition position,
		int widgetID) {
	if (chessGameIsValidPosition(position)) {
		SDL_Rect rect = piecePositionToRect(position);
		buttonSetLocation((Button*) window->widgets[widgetID]->data, rect);
	}
}

static void handlePieceDragMode(Window* window) {
	GameWindowData* data = gameWindowGetData(window);
	SDL_Event event;
	ChessPiecePosition pos;
	while (event.type != SDL_MOUSEBUTTONUP) {
		pos = coordinatesToPiecePosition(event.button.x, event.button.y);
		setWidgetByPosition(window, pos, data->dragPieceWidgetID);
		windowDraw(window);
		SDL_WaitEvent(&event);
	}
	data->targetPos = pos;
}

static UI_EVENT handleEventPieceClick(Window* window, SDL_Event* event,
		int widgetID) {
	SDL_MouseButtonEvent mouseEvent = event->button;
	GameWindowData* data = gameWindowGetData(window);
	ChessPiecePosition pos = coordinatesToPiecePosition(mouseEvent.x,
			mouseEvent.y);
	switch (mouseEvent.button) {
	case SDL_BUTTON_LEFT:
		data->sourcePos = pos;
		data->dragPieceWidgetID = widgetID;
		handlePieceDragMode(window);
		return UI_BUTTON_EVENT_PIECE_DRAG;
	case SDL_BUTTON_RIGHT:
		data->sourcePos = pos;
		return UI_BUTTON_EVENT_GET_MOVES;
	default:
		break;
	}
	return UI_EVENT_NONE;
}

static bool addOtherWidgets(SDL_Renderer* renderer, Widget** widgets) {
	int i = 0;
	SDL_Rect rect = { .x = OTHER_X, .y = OTHER_Y, .h = OTHER_H, .w = OTHER_W };
	if (!createButtonInWidgetArray(widgets, i, renderer, rect, UI_PIC_RESTART,
			UI_PIC_RESTART, UI_BUTTON_EVENT_RESTART, UI_BUTTON_EVENT_RESTART,
			true))
		return false;
	i++;
	rect.y += OTHER_Y_DELTA;
	if (!createButtonInWidgetArray(widgets, i, renderer, rect, UI_PIC_SAVE,
			UI_PIC_SAVE, UI_BUTTON_EVENT_SAVE, UI_BUTTON_EVENT_SAVE, true))
		return false;
	i++;
	rect.y += OTHER_Y_DELTA;
	if (!createButtonInWidgetArray(widgets, i, renderer, rect, UI_PIC_LOAD_GAME,
			UI_PIC_LOAD_GAME, UI_BUTTON_EVENT_LOAD, UI_BUTTON_EVENT_LOAD, true))
		return false;
	i++;
	rect.y += OTHER_Y_DELTA;
	if (!createButtonInWidgetArray(widgets, i, renderer, rect, UI_PIC_UNDO,
			UI_PIC_UNDO, UI_BUTTON_EVENT_UNDO, UI_BUTTON_EVENT_UNDO, true))
		return false;
	i++;
	rect.y += OTHER_Y_DELTA;
	if (!createButtonInWidgetArray(widgets, i, renderer, rect,
			UI_PIC_BACK_TO_MAIN, UI_PIC_BACK_TO_MAIN,
			UI_BUTTON_EVENT_MAIN_MENU_SCREEN, UI_BUTTON_EVENT_MAIN_MENU_SCREEN,
			true))
		return false;
	i++;
	rect.y += OTHER_Y_DELTA;
	if (!createButtonInWidgetArray(widgets, i, renderer, rect, UI_PIC_EXIT_GAME,
			UI_PIC_EXIT_GAME, UI_BUTTON_EVENT_EXIT, UI_BUTTON_EVENT_EXIT, true))
		return false;
	return true;
}

static Widget** gameWindowWidgetsCreate(SDL_Renderer* renderer,
		GameWindowData* data) {
	int numOfWidgets = getNumberOfWidgets(data);
	Widget** widgets = calloc(numOfWidgets, sizeof(Widget*));
	if (widgets == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	if (!addOtherWidgets(renderer, widgets))
		return NULL ;
	int widgetID = OTHER_BUTTONS_NUM;
	for (int i = 0; i < CHESS_N_ROWS; i++) {
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			char* picPath = getPiecePicturePath(
					data->game->gameBoard.position[i][j]);
			if (picPath != NULL ) {
				ChessPiecePosition* pos = malloc(sizeof(ChessPiecePosition));
				if (pos == NULL ) {
					hadMemoryFailure();
					widgetListDestory(widgets, widgetID);
					return NULL ;
				}
				*pos = (ChessPiecePosition ) { .row = i, .column = j };
				SDL_Rect rect = piecePositionToRect(*pos);
				if (!createButtonInWidgetArray(widgets, widgetID, renderer,
						rect, picPath, picPath,
						UI_BUTTON_EVENT_CHESS_PIECE_CLICK,
						UI_BUTTON_EVENT_CHESS_PIECE_CLICK, true))
					return NULL ;
				widgetID++;
			}
		}
	}
	return widgets;
}

static GameWindowData* createGameWindowData(ChessGame* game) {
	GameWindowData* data = malloc(sizeof(GameWindowData));
	if (data == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	data->game = game;
	data->numOfPieces = getNumberOfPieces(game);
	data->numOfHighlightMoves = 0;
	data->dragPieceWidgetID = -1;
	return data;
}

static void gameWindowDraw(Window* window) {
	if (window == NULL )
		return;
	SDL_Renderer* renderer = window->renderer;
	//Draw window
	SDL_Rect rec = { .x = 0, .y = 0, .w = 800, .h = 600 };
	if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) == -1) {
		hadSDLError();
		return;
	}
	if (SDL_RenderClear(renderer) == -1) {
		hadSDLError();
		return;
	}
	if (SDL_RenderCopy(renderer, window->bgTexture, NULL, &rec) == -1) {
		hadSDLError();
		return;
	}
	GameWindowData* data = gameWindowGetData(window);
	int dragWidgetID = data->dragPieceWidgetID;
	//Draw highlighted moves
	for (int i = data->numOfPieces + OTHER_BUTTONS_NUM;
			i < window->numOfWidgets; i++) {
		(window->widgets[i])->drawWidget(window->widgets[i]);
		if (getHadSDLError())
			return;
	}
	//Draw basic window buttons + chess pieces
	for (int i = 0; i < window->numOfWidgets - data->numOfHighlightMoves; i++) {
		if (window->widgets[i] == NULL || i == dragWidgetID)
			continue;
		(window->widgets[i])->drawWidget(window->widgets[i]);
		if (getHadSDLError())
			return;
	}
	if (dragWidgetID >= 0)
		(window->widgets[dragWidgetID])->drawWidget(
				window->widgets[dragWidgetID]);
	if (getHadSDLError())
		return;
	SDL_RenderPresent(renderer);
}

static UI_EVENT gameWindowHandleEvent(Window* window, SDL_Event* event) {
	if (window == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	for (int i = 0; i < window->numOfWidgets; i++) {
		UI_EVENT eventResult = window->widgets[i]->handleEvent(
				window->widgets[i], event);
		if (eventResult == UI_BUTTON_EVENT_CHESS_PIECE_CLICK) {
			return handleEventPieceClick(window, event, i);
		} else if (eventResult != UI_EVENT_NONE)
			return eventResult;
	}
	return UI_EVENT_NONE;
}

Window* gameWindowCreate(ChessGame* game) {
	GameWindowData* data = createGameWindowData(game);
	if (data == NULL ) {
		return NULL ;
	}
	Window* window = windowBaseCreate(UI_PIC_GAME_MENU);
	if (window == NULL )
		return NULL ;
	window->data = data;
	window->numOfWidgets = getNumberOfWidgets(data);
	window->drawWindow = gameWindowDraw;
	window->handleEvent = gameWindowHandleEvent;
	window->widgets = gameWindowWidgetsCreate(window->renderer, data);
	if (window->widgets == NULL ) {
		windowDestroy(window);
		return NULL ;
	}
	return window;
}

GameWindowData* gameWindowGetData(Window* window) {
	return (GameWindowData*) window->data;
}

bool gameWindowAddHighlightMoves(Window* window, ArrayList* moves) {
	int numOfMoves = moves->actualSize;
	int prevNumOfWidgets = window->numOfWidgets;
	Widget** widgets = realloc(window->widgets,
			sizeof(Widget) * (numOfMoves + prevNumOfWidgets));
	if (widgets == NULL ) {
		hadMemoryFailure();
		return false;
	}

	for (int i = 0; i < numOfMoves; i++) {
		ChessMove move = arrayListGetAt(moves, i);
		SDL_Rect rect = piecePositionToRect(move.currentPosition);
		const char* movePicPath = UI_PIC_POSSIBLE_MOVE;
		if (move.capturedPiece.type != CHESS_PIECE_EMPTY) {
			if (move.isThreatened)
				movePicPath = UI_PIC_THREATENED_CAPTURE_MOVE;
			else
				movePicPath = UI_PIC_CAPTURE_MOVE;
		}
		else if (move.isThreatened) {
			movePicPath = UI_PIC_THREATENED_MOVE;
		}
		rect.x += HIGHLIGHT_DELTA;
		rect.y += HIGHLIGHT_DELTA;
		rect.h += -2 * HIGHLIGHT_DELTA;
		rect.w += -2 * HIGHLIGHT_DELTA;
		if (!createButtonInWidgetArray(widgets, prevNumOfWidgets + i,
				window->renderer, rect, movePicPath, movePicPath, UI_EVENT_NONE,
				UI_EVENT_NONE, false))
			return false;
	}
	gameWindowGetData(window)->numOfHighlightMoves = numOfMoves;
	window->numOfWidgets = prevNumOfWidgets + numOfMoves;
	window->widgets = widgets;
	return true;
}

bool gameWindowRemoveHighlightMoves(Window* window) {
	int numOfMoves = gameWindowGetData(window)->numOfHighlightMoves;
	int prevNumOfWidgets = window->numOfWidgets - numOfMoves;
	for (int i = prevNumOfWidgets; i < window->numOfWidgets; i++)
		widgetDestroy(window->widgets[i]);
	Widget** widgets = realloc(window->widgets,
			sizeof(Widget*) * (prevNumOfWidgets));
	if (widgets == NULL ) {
		hadMemoryFailure();
		return false;
	}
	gameWindowGetData(window)->numOfHighlightMoves = 0;
	window->numOfWidgets = prevNumOfWidgets;
	window->widgets = widgets;
	return true;
}

bool gameWindowRefreshWidgets(Window* window) {
	GameWindowData* data = createGameWindowData(
			gameWindowGetData(window)->game);
	if (data == NULL ) {
		hadMemoryFailure();
		return false;
	}
	Widget** widgets = gameWindowWidgetsCreate(window->renderer, data);
	if (widgets == NULL ) {
		hadMemoryFailure();
		free(data);
		return false;
	}
	free(window->data);
	widgetListDestory(window->widgets, window->numOfWidgets);
	window->data = data;
	window->numOfWidgets = getNumberOfWidgets(data);
	window->widgets = widgets;
	return true;
}
