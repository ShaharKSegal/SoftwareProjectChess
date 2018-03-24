#ifndef CHESSGAME_H_
#define CHESSGAME_H_
#include "ChessGameCommon.h"
#include "ArrayList.h"

/**
 * ChessGame Summary:
 *
 * A container that represents a classic chess game, a two players or one player.
 * The container supports the following functions.
 *
 * chessGameCreate           - Creates a new game board
 * chessGameCopy             - Copies a game board
 * chessGameDestroy          - Frees all memory resources associated with a game
 * chessGameSetMove          - Sets a move on a game board
 * chessGameGetMoves         - Gets all valid moves by a specified piece.
 * chessGameIsValidMove      - Checks if a move is valid
 * chessGameUndoMove         - Undoes previous move made by the last player
 * chessGamePrintBoard       - Prints the current board
 * chessGameGetCurrentPlayer - Returns the current player
 *
 */

typedef struct chess_game_t {
	ChessBoard gameBoard;
	char currentPlayer;
	int historySize;
	unsigned int maxDepth;
	ArrayList* history;
	ChessPiecePosition whiteKingPosition;
	ChessPiecePosition blackKingPosition;
	bool isCheck;
} ChessGame;

/**
 * Type used for returning error codes from game functions
 */
typedef enum chess_game_message_t {
	CHESS_GAME_NONE,
	CHESS_GAME_INVALID_POSITION,
	CHESS_GAME_NO_PIECE_FOUND,
	CHESS_GAME_INVALID_MOVE,
	CHESS_GAME_MOVE_THREATEN_KING,
	CHESS_GAME_UNRESOLVED_THREATENED_KING,
	CHESS_GAME_EMPTY_HISTORY,
	CHESS_GAME_DRAW,
	CHESS_GAME_CHECK,
	CHESS_GAME_CHECKMATE,
	CHESS_GAME_SUCCESS,
} CHESS_GAME_MESSAGE;

/**
 * Creates a new game.
 *
 * @return
 * NULL if a memory allocation failure occurs.
 * Otherwise, a new game instance is returned.
 */
ChessGame* chessGameCreate();

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
ChessGame* chessGameCopy(ChessGame* src);

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void chessGameDestroy(ChessGame* src);

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
CHESS_GAME_MESSAGE chessGameSetMove(ChessGame* game,
		ChessPiecePosition cur_pos, ChessPiecePosition next_pos);

/**
 * Gets all valid moves for the specified position.
 *
 * @param game - The game. Assumes not NULL.
 * @param pos - The piece's position on board. Assumes not NULL.
 *
 * @return
 * ArrayList with all possible moves
 */
ArrayList* chessGameGetMoves(ChessGame* game, ChessPiecePosition pos);

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
CHESS_GAME_MESSAGE chessGameUndoMove(ChessGame* game);

/**
 * On success, the function prints the board game,
 * by the defined representation of each piece.
 *
 * @param game - Assumes not NULL.
 */
// TODO: move to different location since it's only for console.
void chessGamePrintBoard(ChessGame* game);

/**
 * Returns the current player of the specified game.
 * @param game - Assume not null
 * @return
 * game->currentPlayer
 */
short chessGameGetCurrentPlayer(ChessGame* src);

/**
 * Checks if the current state is checkmate, draw or none of them.
 * @param game - the source game
 * @return
 * 	CHESS_GAME_DRAW 		- if the game is draw.
 *	CHESS_GAME_CHECKMATE	- if their's a checkmate.
 *	CHESS_GAME_NONE			- if none of the above is true.
 */
CHESS_GAME_MESSAGE chessGameGetCurrentState(ChessGame* game);

#endif
