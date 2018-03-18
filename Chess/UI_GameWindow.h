#ifndef UI_GAMEWINDOW_H_
#define UI_GAMEWINDOW_H_

#include "UI_Window.h"
#include "ChessGame.h"

typedef struct game_window_data_t {
	ChessGame* game;
	ChessPiecePosition sourcePos;
	ChessPiecePosition targetPos;
	int dragPieceWidgetID;
	int numOfPieces;
	int numOfHighlightMoves;
} GameWindowData;

Window* gameWindowCreate(ChessGame* game);

bool gameWindowAddHighlightMoves(Window* window, ArrayList* moves);

bool gameWindowRemoveHighlightMoves(Window* window);

bool gameWindowRefreshWidgets(Window* window);

GameWindowData* gameWindowGetData(Window* window);

#endif /* UI_GAMEWINDOW_H_ */
