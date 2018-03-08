#ifndef CHESSGAMEMOVE_H_
#define CHESSGAMEMOVE_H_
#include "ChessGameCommon.h"
#include "ArrayList.h"

/**
 * ChessGameMove contain all the chess pieces moves for ChessPiece type to use.
 * This module doesn't check king threats, only if the piece can move from
 * position a to position b.
 */

// Basic definitions for movement behaviour.
#define WHITE_PAWN_ROW 1
#define BLACK_PAWN_ROW 6

/**
 * Validate movement from pos to newPos, split to cases by pos's piece type
 * NOTE: Doesn't check for king threats.
 */
bool chessMoveIsValidMove(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos);

/**
 * Get all valid moves of a piece in pos.
 * Returns NULL if arrayList fails or the position is empty\invalid.
 * NOTE: Doesn't check for king threats.
 * NOTE2: Doesn't update isThreatened field. It should be updated in ChessGame.
 */
ArrayList* chessMoveGetMoves(ChessBoard* board, ChessPiecePosition pos);

#endif /* CHESSGAMEMOVE_H_ */
