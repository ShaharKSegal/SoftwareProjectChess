#include <stdio.h>
#include <stdlib.h>
#include "ChessErrorHandler.h"
#include "ArrayList.h"
#include "ChessGame.h"

/**
 * Definitions for game pieces initial position.
 */
#define WHITE_PAWN_ROW 1
#define BLACK_PAWN_ROW 6
#define WHITE_OTHER_ROW 0
#define BLACK_OTHER_ROW 7
#define BISHOP_COLUMN_L 2
#define BISHOP_COLUMN_R 5
#define KNIGHT_COLUMN_L 1
#define KNIGHT_COLUMN_R 6
#define ROOK_COLUMN_L 0
#define ROOK_COLUMN_R 7
#define QUEEN_COLUMN 3
#define KING_COLUMN 4

/**
 * Definitions for game pieces symbols.
 */
#define WHITE_PAWN_SYMBOL 'm'
#define BLACK_PAWN_SYMBOL 'M'
#define WHITE_BISHOP_SYMBOL 'b'
#define BLACK_BISHOP_SYMBOL 'B'
#define WHITE_KNIGHT_SYMBOL 'n'
#define BLACK_KNIGHT_SYMBOL 'N'
#define WHITE_ROOK_SYMBOL 'r'
#define BLACK_ROOK_SYMBOL 'R'
#define WHITE_QUEEN_SYMBOL 'q'
#define BLACK_QUEEN_SYMBOL 'Q'
#define WHITE_KING_SYMBOL 'k'
#define BLACK_KING_SYMBOL 'K'
#define EMPTY_ENTRY_SYMBOL '_'

/**
 * Definitions for print game
 */
#define PRINT_GAME_LINE_SIZE 21
#define PRINT_GAME_ZERO_CHAR '0'
#define PRINT_GAME_VERTICAL_BAR '|'
#define PRINT_GAME_WHITESPACE ' '
#define PRINT_GAME_BEFORE_LAST_LINE "  -----------------  \n"
#define PRINT_GAME_LAST_LINE "   A B C D E F G H   \n"

/**
 * Declaration of chess pieces. These are constants.
 */
const static ChessPiece WHITE_PAWN = { .type = CHESS_GAME_PAWN, .player =
		CHESS_GAME_WHITE_PLAYER, .consoleRepresentation = WHITE_PAWN_SYMBOL };
const static ChessPiece BLACK_PAWN = { .type = CHESS_GAME_PAWN, .player =
		CHESS_GAME_BLACK_PLAYER, .consoleRepresentation = BLACK_PAWN_SYMBOL };
const static ChessPiece WHITE_BISHOP = { .type = CHESS_GAME_BISHOP, .player =
		CHESS_GAME_WHITE_PLAYER, .consoleRepresentation = WHITE_BISHOP_SYMBOL };
const static ChessPiece BLACK_BISHOP = { .type = CHESS_GAME_BISHOP, .player =
		CHESS_GAME_BLACK_PLAYER, .consoleRepresentation = BLACK_BISHOP_SYMBOL };
const static ChessPiece WHITE_KNIGHT = { .type = CHESS_GAME_KNIGHT, .player =
		CHESS_GAME_WHITE_PLAYER, .consoleRepresentation = WHITE_KNIGHT_SYMBOL };
const static ChessPiece BLACK_KNIGHT = { .type = CHESS_GAME_KNIGHT, .player =
		CHESS_GAME_BLACK_PLAYER, .consoleRepresentation = BLACK_KNIGHT_SYMBOL };
const static ChessPiece WHITE_ROOK = { .type = CHESS_GAME_ROOK, .player =
		CHESS_GAME_WHITE_PLAYER, .consoleRepresentation = WHITE_ROOK_SYMBOL };
const static ChessPiece BLACK_ROOK = { .type = CHESS_GAME_ROOK, .player =
		CHESS_GAME_BLACK_PLAYER, .consoleRepresentation = BLACK_ROOK_SYMBOL };
const static ChessPiece WHITE_QUEEN = { .type = CHESS_GAME_QUEEN, .player =
		CHESS_GAME_WHITE_PLAYER, .consoleRepresentation = WHITE_QUEEN_SYMBOL };
const static ChessPiece BLACK_QUEEN = { .type = CHESS_GAME_QUEEN, .player =
		CHESS_GAME_BLACK_PLAYER, .consoleRepresentation = BLACK_QUEEN_SYMBOL };
const static ChessPiece WHITE_KING = { .type = CHESS_GAME_KING, .player =
		CHESS_GAME_WHITE_PLAYER, .consoleRepresentation = WHITE_KING_SYMBOL };
const static ChessPiece BLACK_KING = { .type = CHESS_GAME_KING, .player =
		CHESS_GAME_BLACK_PLAYER, .consoleRepresentation = BLACK_KING_SYMBOL };
const static ChessPiece EMPTY_ENTRY = { .type = CHESS_GAME_EMPTY, .player = -1,
		.consoleRepresentation = EMPTY_ENTRY_SYMBOL };

ChessPiece chessGameGetPieceByPosition(ChessGame* game, ChessPiecePosition pos) {
	return game->gameBoard.position[(int) pos[0]][(int) pos[1]];
}

void chessGameSetPieceInPosition(ChessGame* game, ChessPiecePosition pos,
		ChessPiece piece) {
	game->gameBoard.position[(int) pos[0]][(int) pos[1]] = piece;
}

/**
 * Creates a new board for a new game.
 *
 * @return
 * NULL if a memory allocation failure occurred.
 * Otherwise, a new board instance is returned.
 */
ChessGameBoard chessBoardCreate() {
	ChessGameBoard board;
	ChessPiece piece;
	for (int i = 0; i < CHESS_GAME_N_ROWS; i++)
		for (int j = 0; j < CHESS_GAME_N_COLUMNS; j++) {
			bool isWhite = i == WHITE_OTHER_ROW;
			switch (i) {
			case WHITE_PAWN_ROW:
				piece = WHITE_PAWN;
				break;
			case BLACK_PAWN_ROW:
				piece = BLACK_PAWN;
				break;
			case WHITE_OTHER_ROW:
			case BLACK_OTHER_ROW:
				switch (j) {
				case BISHOP_COLUMN_L:
				case BISHOP_COLUMN_R:
					piece = isWhite ? WHITE_BISHOP : BLACK_BISHOP;
					break;
				case KNIGHT_COLUMN_L:
				case KNIGHT_COLUMN_R:
					piece = isWhite ? WHITE_KNIGHT : BLACK_KNIGHT;
					break;
				case ROOK_COLUMN_L:
				case ROOK_COLUMN_R:
					piece = isWhite ? WHITE_ROOK : BLACK_ROOK;
					break;
				case QUEEN_COLUMN:
					piece = isWhite ? WHITE_QUEEN : BLACK_QUEEN;
					break;
				case KING_COLUMN:
					piece = isWhite ? WHITE_KING : BLACK_KING;
					break;
				}
				break;
			default:
				piece = EMPTY_ENTRY;
			}
			board.position[i][j] = piece;
		}
	return board;
}

/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @historySize - The total number of moves to undo,
 *                a player can undo at most historySizeMoves turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instance is returned.
 */
ChessGame* chessGameCreate(int historySize) {
	if (historySize <= 0)
		return NULL ;
	ArrayList* list = arrayListCreate(historySize);
	if (list == NULL )
		return NULL ;
	ChessGame* game = malloc(sizeof(ChessGame));
	if (game == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	game->historySize = historySize;
	game->history = list;
	game->currentPlayer = CHESS_GAME_WHITE_PLAYER;
	game->gameBoard = chessBoardCreate();
	return game;
}

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
ChessGame* chessGameCopy(ChessGame* src) {
	if (src == NULL )
		return NULL ;
	ChessGame* game = chessGameCreate(src->historySize);
	if (game == NULL )
		return NULL ;
	ArrayList* history = arrayListCopy(src->history);
	if (history == NULL )
		return NULL ;
	arrayListDestroy(game->history);
	game->history = history;
	for (int i = 0; i < CHESS_GAME_N_ROWS; i++)
		for (int j = 0; j < CHESS_GAME_N_COLUMNS; j++)
			game->gameBoard.position[i][j] = src->gameBoard.position[i][j];
	game->currentPlayer = src->currentPlayer;
	game->maxDepth = src->maxDepth;
	return game;
}

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void chessGameDestroy(ChessGame* src) {
	if (src == NULL )
		return;
	arrayListDestroy(src->history);
	free(src);
}

/**
 * Changes src->currentPlayer to the other player if src is not NULL.
 */
void chessGameChangePlayer(ChessGame* src) {
	char currentPlayer = chessGameGetCurrentPlayer(src);
	if (!currentPlayer)
		return;
	if (currentPlayer == CHESS_GAME_WHITE_PLAYER)
		src->currentPlayer = CHESS_GAME_BLACK_PLAYER;
	else
		src->currentPlayer = CHESS_GAME_WHITE_PLAYER;
}

/**
 * Checks if a piece can be put in the specified position.
 *
 * @param game - The source game. Assumes not NULL.
 * @param cur_pos - The piece's position on board. Assumes not NULL.
 * @param next_pos - The specified position. Assumes not NULL.
 *
 * @return
 * CHESS_GAME_INVALID_POSITION - if cur_pos or next_pos are out-of-range.
 * CHESS_GAME_INVALID_MOVE - if the given next_pos is illegal for this piece.
 * CHESS_GAME_MOVE_THREATEN_KING - if the move will cause your king to be threatened.
 * CHESS_GAME_UNRESOLVED_THREATENED_KING - if the move doesn't resolve the threatened king.
 * CHESS_GAME_SUCCESS - otherwise
 */
CHESS_GAME_MESSAGE chessGameIsValidMove(ChessGame* game,
		ChessPiecePosition cur_pos, ChessPiecePosition next_pos) {
	if (cur_pos[0] < 0|| cur_pos[0] > CHESS_GAME_N_ROWS
			|| cur_pos[1] < 0 || cur_pos[1] > CHESS_GAME_N_COLUMNS
			|| next_pos[0] < 0 || next_pos[0] > CHESS_GAME_N_ROWS
			|| next_pos[1] < 0 || next_pos[1] > CHESS_GAME_N_COLUMNS)
		return CHESS_GAME_INVALID_POSITION;
	if (!chessGameGetPieceByPosition(game, cur_pos)
			.isValidMove(game->gameBoard, cur_pos, next_pos))
		return CHESS_GAME_INVALID_MOVE;
	// TODO: Add CHESS_GAME_MOVE_THREATEN_KING logic.
	// TODO: Add CHESS_GAME_UNRESOLVED_THREATENED_KING logic.
	return CHESS_GAME_SUCCESS;
}

/**
 * Sets the next move in a given game by specifying ChessPiecePosition.
 *
 * @param game - The source game. Assumes not NULL.
 * @param cur_pos - The piece's position on board. Assumes not NULL.
 * @param next_pos - The specified position. Assumes not NULL.
 *
 * @return
 * CHESS_GAME_INVALID_POSITION - if cur_pos or next_pos are out-of-range.
 * CHESS_GAME_INVALID_MOVE - if the given next_pos is illegal for this piece.
 * CHESS_GAME_MOVE_THREATEN_KING - if the move will cause your king to be threatened.
 * CHESS_GAME_UNRESOLVED_THREATENED_KING - if the move doesn't resolve the threatened king.
 * CHESS_GAME_SUCCESS - otherwise
 */
CHESS_GAME_MESSAGE chessGameSetMove(ChessGame* game, ChessPiecePosition cur_pos,
		ChessPiecePosition next_pos) {
	CHESS_GAME_MESSAGE res = chessGameIsValidMove(game, cur_pos, next_pos);
	if (res != CHESS_GAME_SUCCESS)
		return res;
	ArrayList* history = game->history;
	chessGameChangePlayer(game);
	if (arrayListIsFull(history))
		arrayListRemoveFirst(history);
	ChessMove move = {
			.previousPosition = cur_pos,
			.currentPosition = next_pos,
			.capturedPiece = chessGameGetPieceByPosition(game, next_pos)
	};
	arrayListAddLast(history, move);
	ChessPiece piece = chessGameGetPieceByPosition(game, cur_pos);
	chessGameSetPieceInPosition(game, next_pos, piece);
	return res;
}

/**
 * Undo the last move on the board and changes the current player's turn.
 * If the user invoked this command more than historySize times in a row, an error occurs.
 *
 * @param src - The source game, assumes not NULL.
 * @return
 * CHESS_GAME_EMPTY_HISTORY    - if the user invoked this function more then
 *                               historySize in a row.
 * CHESS_GAME_SUCCESS          - On success. The last move on the board is removed,
 * 								 and the current player is changed.
 */
CHESS_GAME_MESSAGE chessGameUndoMove(ChessGame* game) {
	ArrayList* history = game->history;
	if (arrayListIsEmpty(history))
		return CHESS_GAME_EMPTY_HISTORY;
	ChessMove move = arrayListGetLast(history);
	ChessPiece currPiece = chessGameGetPieceByPosition(game, move.currentPosition);
	chessGameSetPieceInPosition(game, move.previousPosition, currPiece);
	chessGameSetPieceInPosition(game, move.currentPosition, move.capturedPiece);
	arrayListRemoveLast(history);
	chessGameChangePlayer(game);
	return CHESS_GAME_SUCCESS;
}

/**
 * On success, the function prints the board game,
 * by the defined representation of each piece.
 *
 * @param game - Assumes not NULL.
 */
void chessGamePrintBoard(ChessGame* game) {
	char line[PRINT_GAME_LINE_SIZE + 1];
	line[1] = line[PRINT_GAME_LINE_SIZE - 1] = PRINT_GAME_VERTICAL_BAR;
	line[PRINT_GAME_LINE_SIZE - 2] = PRINT_GAME_WHITESPACE;
	line[PRINT_GAME_LINE_SIZE] = 0;
	for (int i = CHESS_GAME_N_ROWS - 1; i >= 0; i--) {
		line[0] = PRINT_GAME_ZERO_CHAR + (char) (i + 1);
		for (int j = 0; j < CHESS_GAME_N_COLUMNS; j++) {
			line[2 * j + 2] = PRINT_GAME_WHITESPACE;
			line[2 * j + 3] =
					game->gameBoard.position[i][j].consoleRepresentation;
		}
		printf("%s\n", line);
	}
	printf(PRINT_GAME_BEFORE_LAST_LINE);
	printf(PRINT_GAME_LAST_LINE);
}

/**
 * Returns the current player of the specified game.
 * @param game - Assume not null
 * @return
 * game->currentPlayer
 */
char chessGameGetCurrentPlayer(ChessGame* game) {
	return game->currentPlayer;
}

bool chessCheckHorizontalWin(ChessGame* src, int last_col, int last_row,
		char last_player) {
	int counter = 0;
	int min_col =
			(last_col - CHESS_GAME_SPAN + 1) < 0 ?
					0 : last_col - CHESS_GAME_SPAN + 1;
	int max_col =
			(last_col + CHESS_GAME_SPAN) > CHESS_GAME_N_COLUMNS ?
					CHESS_GAME_N_COLUMNS : last_col + CHESS_GAME_SPAN;

//check horizontal win
	for (int j = min_col; j < max_col; j++) {
		if (src->gameBoard[last_row][j] == last_player)
			counter++;
		else
			counter = 0;
		if (counter == CHESS_GAME_SPAN)
			return true;
	}
	return false;
}

bool chessCheckVerticalWin(ChessGame* src, int last_col, int last_row,
		char last_player) {
	int counter = 0;
	if (last_row >= (CHESS_GAME_SPAN - 1))
		for (int i = last_row; i > last_row - CHESS_GAME_SPAN; i--) {
			if (src->gameBoard[i][last_col] == last_player)
				counter++;
			else
				counter = 0;
			if (counter == CHESS_GAME_SPAN)
				return true;
		}
	return false;
}

bool chessCheckDiagUpWin(ChessGame* src, int last_col, int last_row,
		char last_player) {
	int counter = 0;
	int min_val = min(last_row, last_col, CHESS_GAME_SPAN - 1);
	int i = last_row - min_val;
	int j = last_col - min_val;
	while (i < CHESS_GAME_N_ROWS && j < CHESS_GAME_N_COLUMNS) {
		if (src->gameBoard[i][j] == last_player) {
			if (++counter == CHESS_GAME_SPAN)
				return true;
		} else
			counter = 0;
		i++;
		j++;
	}
	return false;
}

bool chessCheckDiagDownWin(ChessGame* src, int last_col, int last_row,
		char last_player) {
	int counter = 0;
	int min_val = min(CHESS_GAME_N_ROWS - last_row - 1, last_col,
			CHESS_GAME_SPAN - 1);
	int i = last_row + min_val;
	int j = last_col - min_val;
	while (i >= 0 && j < CHESS_GAME_N_COLUMNS) {
		if (src->gameBoard[i][j] == last_player) {
			if (++counter == CHESS_GAME_SPAN)
				return true;
		} else
			counter = 0;
		i--;
		j++;
	}
	return false;
}

/**
 * Checks if there's a winner in the specified game status. The function returns either
 * CHESS_GAME_PLAYER_1_SYMBOL or CHESS_GAME_PLAYER_2_SYMBOL in case there's a winner, where
 * the value returned is the symbol of the winner. If the game is over and there's a tie
 * then the value CHESS_GAME_TIE_SYMBOL is returned. in any other case the null characters
 * is returned.
 * @param src - the source game
 * @return
 * CHESS_GAME_PLAYER_1_SYMBOL - if player 1 won
 * CHESS_GAME_PLAYER_2_SYMBOL - if player 2 won
 * CHESS_GAME_TIE_SYMBOL - If the game is over and there's a tie
 * null character - otherwise
 */
char chessCheckWinner(ChessGame* src) {
	if (src == NULL )
		return 0;
	if (spArrayListIsEmpty(src->history))
		return 0;
	int last_col = spArrayListGetLast(src->history);
	int last_row = src->tops[last_col] - 1;
	char last_player = src->gameBoard[last_row][last_col];

//check horizontal up win
	if (chessCheckHorizontalWin(src, last_col, last_row, last_player))
		return last_player;

//check vertical win
	if (chessCheckVerticalWin(src, last_col, last_row, last_player))
		return last_player;

//check diagonal up win
	if (chessCheckDiagUpWin(src, last_col, last_row, last_player))
		return last_player;

//check diagonal down win
	if (chessCheckDiagDownWin(src, last_col, last_row, last_player))
		return last_player;

//check for tie or otherwise
	for (int i = 0; i < CHESS_GAME_N_COLUMNS; i++)
		if (src->tops[i] != CHESS_GAME_N_ROWS)
			return 0;
	return CHESS_GAME_TIE_SYMBOL;

}
