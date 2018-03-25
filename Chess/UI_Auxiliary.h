#ifndef UI_AUXILIARY_H_
#define UI_AUXILIARY_H_

#include <stdlib.h>
#include "ChessGame.h"
#include "UI_Widget.h"

/**
 * Basic window settings
 */
#define UI_WINDOW_TITLE "Chess"
#define UI_WINDOW_W 800
#define UI_WINDOW_H 600

/**
 * Pictures relative path
 */
#define UI_PIC_MAX_PATH 2048

#define UI_PIC_DEFAULT_MENU "./pictures/chessMainbg.bmp"
#define UI_PIC_SETTINGS_MENU "./pictures/chessSettings.bmp"
#define UI_PIC_GAME_MENU "./pictures/game.bmp"

#define UI_PIC_NEW_GAME "./pictures/newGame.bmp"
#define UI_PIC_LOAD "./pictures/Load.bmp"
#define UI_PIC_EXIT "./pictures/exit.bmp"

#define UI_PIC_ONE_PLAYER_ACTIVE "./pictures/onePlayerActive.bmp"
#define UI_PIC_ONE_PLAYER_INACTIVE "./pictures/onePlayerInActive.bmp"
#define UI_PIC_TWO_PLAYER_ACTIVE "./pictures/twoPlayerActive.bmp"
#define UI_PIC_TWO_PLAYER_INACTIVE "./pictures/twoPlayerInActive.bmp"

#define UI_PIC_WHITE_COLOR_ACTIVE "./pictures/whiteColorActive.bmp"
#define UI_PIC_WHITE_COLOR_INACTIVE "./pictures/whiteColorInactive.bmp"
#define UI_PIC_BLACK_COLOR_ACTIVE "./pictures/blackColorActive.bmp"
#define UI_PIC_BLACK_COLOR_INACTIVE "./pictures/blackColorInactive.bmp"

#define UI_PIC_DIFFICULTY_AMATEUR_ACTIVE "./pictures/noobActive.bmp"
#define UI_PIC_DIFFICULTY_AMATEUR_INACTIVE "./pictures/noobInActive.bmp"
#define UI_PIC_DIFFICULTY_EASY_ACTIVE "./pictures/easyActive.bmp"
#define UI_PIC_DIFFICULTY_EASY_INACTIVE "./pictures/easyInActive.bmp"
#define UI_PIC_DIFFICULTY_MODERATE_ACTIVE "./pictures/moderateActive.bmp"
#define UI_PIC_DIFFICULTY_MODERATE_INACTIVE "./pictures/moderateInActive.bmp"
#define UI_PIC_DIFFICULTY_HARD_ACTIVE "./pictures/hardActive.bmp"
#define UI_PIC_DIFFICULTY_HARD_INACTIVE "./pictures/hardInActive.bmp"
#define UI_PIC_DIFFICULTY_EXPERT_ACTIVE "./pictures/expertActive.bmp"
#define UI_PIC_DIFFICULTY_EXPERT_INACTIVE "./pictures/expertInActive.bmp"

#define UI_PIC_START_ACTIVE "./pictures/start.bmp"
#define UI_PIC_START_INACTIVE "./pictures/startInactive.bmp"
#define UI_PIC_BACK "./pictures/back.bmp"

#define UI_PIC_LOAD_ACTIVE "./pictures/loadActive.bmp"
#define UI_PIC_LOAD_INACTIVE "./pictures/loadinActive.bmp"
#define UI_PIC_SAVE_ACTIVE "./pictures/saveActive.bmp"
#define UI_PIC_SAVE_INACTIVE "./pictures/saveInactive.bmp"
#define UI_PIC_NEXT_PAGE_ACTIVE "./pictures/nextActive.bmp"
#define UI_PIC_NEXT_PAGE_INACTIVE "./pictures/nextInactive.bmp"
#define UI_PIC_PREV_PAGE_ACTIVE "./pictures/previousActive.bmp"
#define UI_PIC_PREV_PAGE_INACTIVE "./pictures/previousInactive.bmp"
#define UI_PIC_SLOT_ACTIVE_TEMPLATE "./pictures/GameSlot%dActive.bmp"
#define UI_PIC_SLOT_INACTIVE_TEMPLATE "./pictures/GameSlot%dinActive.bmp"

#define UI_PIC_BLACK_PAWN "./pictures/blackPawn.bmp"
#define UI_PIC_WHITE_PAWN "./pictures/whitePawn.bmp"
#define UI_PIC_BLACK_KNIGHT "./pictures/blackKnight.bmp"
#define UI_PIC_WHITE_KNIGHT "./pictures/whiteKnight.bmp"
#define UI_PIC_BLACK_BISHOP "./pictures/blackBishop.bmp"
#define UI_PIC_WHITE_BISHOP "./pictures/whiteBishop.bmp"
#define UI_PIC_BLACK_ROOK "./pictures/blackRook.bmp"
#define UI_PIC_WHITE_ROOK "./pictures/whiteRook.bmp"
#define UI_PIC_BLACK_QUEEN "./pictures/blackQueen.bmp"
#define UI_PIC_WHITE_QUEEN "./pictures/whiteQueen.bmp"
#define UI_PIC_BLACK_KING "./pictures/blackKing.bmp"
#define UI_PIC_WHITE_KING "./pictures/whiteKing.bmp"

#define UI_PIC_POSSIBLE_MOVE "./pictures/possibleMove.bmp"
#define UI_PIC_THREATENED_MOVE "./pictures/threatenedLoc.bmp"
#define UI_PIC_CAPTURE_MOVE "./pictures/opponentLoc.bmp"
#define UI_PIC_THREATENED_CAPTURE_MOVE "./pictures/threatenedAndOpponentLoc.bmp"

#define UI_PIC_RESTART "./pictures/restart.bmp"
#define UI_PIC_SAVE "./pictures/save.bmp"
#define UI_PIC_LOAD_GAME "./pictures/loadForGame.bmp"
#define UI_PIC_UNDO "./pictures/undo.bmp"
#define UI_PIC_BACK_TO_MAIN "./pictures/mainMenuForGame.bmp"
#define UI_PIC_EXIT_GAME "./pictures/exitForGame.bmp"

/**
 * Adds a new button to the widgets array in the given index.
 * Handles failures and frees memory in the widget array.
 * NOTE: assumes the given index is the end of the actual size of the array.
 */
bool createButtonInWidgetArray(Widget** widgets, int index,
		SDL_Renderer* renderer, SDL_Rect location, const char* activeImage,
		const char* inactiveImage, UI_EVENT eventActive, UI_EVENT eventInactive,
		bool isActive);

/**
 * Popup message box for unsaved changes, asking the user approval.
 * @return:
 * UI_MSGBOX_EVENT_YES - The controller will change to the save screen.
 * UI_MSGBOX_EVENT_NO - Ignore unsaved changes and continue.
 * UI_MSGBOX_EVENT_CANCEL - Go back to the window as if nothing happened.
 */
UI_EVENT unsavedChangesPopup();

/**
 * Popup message box for the game state. Indicating check, checkmate or draw.
 * @return:
 * returns what chessGameGetCurrentState returns.
 * See chessGameGetCurrentState in ChessGame.h for details
 */
CHESS_GAME_MESSAGE chessGameStatePopup(ChessGame* game);

#endif
