#ifndef CHESSGAMECOMMON_H_
#define CHESSGAMECOMMON_H_
#include <stdbool.h>

/**
 * ChessGameCommon contain common game types and common functions to be used by all other modules.
 */

//Definitions
#define CHESS_N_ROWS 8
#define CHESS_N_COLUMNS 8
#define CHESS_WHITE_PLAYER 1
#define CHESS_BLACK_PLAYER 0
#define CHESS_NON_PLAYER -1
#define CHESS_START_ROW_CHAR 'A'
#define CHESS_START_COLUMN_CHAR '1'

/**
 * Enum used for pieces types
 */
typedef enum chess_piece_type_t {
	CHESS_PIECE_PAWN,
	CHESS_PIECE_BISHOP,
	CHESS_PIECE_KNIGHT,
	CHESS_PIECE_ROOK,
	CHESS_PIECE_QUEEN,
	CHESS_PIECE_KING,
	CHESS_PIECE_EMPTY, // Defines an empty space on the board.
} CHESS_PIECE_TYPE;

/**
 * Type for piece position
 */
typedef struct chess_piece_position_t {
	int row;
	int column;
} ChessPiecePosition;

/**
 * Struct used for general information about a chess piece.
 * Without any context to a specific game.
 */
typedef struct chess_game_piece_t {
	CHESS_PIECE_TYPE type;
	int player;
	char representation;
} ChessPiece;

/**
 * Type for the chess board
 */
typedef struct chess_game_board_t {
	ChessPiece position[CHESS_N_ROWS][CHESS_N_COLUMNS];
} ChessBoard;

/**
 *	Checks if the given chess position is valid.
 *	@param pos - the position to validate
 *	@return
 *	Returns true if the position is valid.
 *	If pos is NULL or the position is not valid, returns false.
 */
bool chessGameIsValidPosition(ChessPiecePosition pos);

/**
 *	Gets the ChessPiece in the specified position.
 *	@param board - the game board.
 *	@param pos - the position in which the piece we want to return
 *	@return
 *	Returns the piece in the given pos.
 *	If Board is NULL or pos is NULL or pos is not a valid position on board,
 *	returns undefined value.
 */
ChessPiece chessGameGetPieceByPosition(ChessBoard* board,
		ChessPiecePosition pos);

/**
 *	Checks equality between two given chess positions.
 *	@param pos1 - the first position.
 *	@param pos2 - the second position.
 *	@return
 *	Returns true if both positions are valid and equals, false otherwise.
 */
bool chessGameIsPositionEquals(ChessPiecePosition pos1, ChessPiecePosition pos2);

#endif
