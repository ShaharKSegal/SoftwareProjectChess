#ifndef CHESSGAMEMOVE_H_
#define CHESSGAMEMOVE_H_
#include "ChessGameCommon.h"
#include "ArrayList.h"

/**
 * ChessGameMove contain all the chess pieces moves for ChessPiece type to use.
 * Including a dummy empty one.
 */

// Basic definitions for movement behaviour.
#define WHITE_PAWN_ROW 1
#define BLACK_PAWN_ROW 6

bool chessMoveIsValidMove(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos);

ArrayList* chessMoveGetMoves(ChessBoard* board, ChessPiecePosition pos);

#endif /* CHESSGAMEMOVE_H_ */
