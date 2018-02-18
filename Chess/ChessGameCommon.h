#ifndef CHESSGAMECOMMON_H_
#define CHESSGAMECOMMON_H_
#include <stdbool.h>

/**
 * ChessGameCommon contain common game types to be used by all other modules.
 *
 */

//Definitions
#define CHESS_GAME_N_ROWS 8
#define CHESS_GAME_N_COLUMNS 8
#define CHESS_GAME_WHITE_PLAYER 0
#define CHESS_GAME_BLACK_PLAYER 1
#define CHESS_GAME_START_ROW_CHAR 'A'
#define CHESS_GAME_START_COLUMN_CHAR '1'

/**
 * Type used for pieces types
 */
typedef enum chess_game_piece_type_t {
	CHESS_GAME_PAWN,
	CHESS_GAME_BISHOP,
	CHESS_GAME_KNIGHT,
	CHESS_GAME_ROOK,
	CHESS_GAME_QUEEN,
	CHESS_GAME_KING,
	CHESS_GAME_EMPTY, // Defines an empty space on the board.
} CHESS_GAME_PIECE_TYPE;

/**
 * Type for piece position
 */
typedef char ChessPiecePosition[2];

/**
 * Forward declaration of the board and piece types
 */
typedef struct chess_game_piece_t ChessPiece;
typedef struct chess_game_board_t ChessGameBoard;

/**
 * Struct used for general information about a chess piece.
 * Without any context to a specific game.
 */
struct chess_game_piece_t {
	CHESS_GAME_PIECE_TYPE type;
	int player;
	char consoleRepresentation;
	bool (*isValidMove)(ChessGameBoard, ChessPiecePosition, ChessPiecePosition);
	ChessPiecePosition* (*getMoves)(ChessGameBoard, ChessPiecePosition);
};

/**
 * Type for board game
 */
typedef struct chess_game_board_t {
	ChessPiece position[CHESS_GAME_N_ROWS][CHESS_GAME_N_COLUMNS];
} ChessGameBoard;



#endif
