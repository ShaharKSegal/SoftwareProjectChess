#include <stdio.h>
#include <stdlib.h>
#include "ChessGameCommon.h"

/**
 *	Checks if the given chess position is valid.
 *	@param pos - the position to validate
 *	@return
 *	Returns true if the position is valid.
 *	If pos is NULL or the position is not valid, returns false.
 */
bool chessGameIsValidPosition(ChessPiecePosition pos) {
	if (pos.row < 0 || pos.row > CHESS_N_ROWS)
		return false;
	if (pos.column < 0 || pos.column > CHESS_N_COLUMNS)
		return false;
	return true;
}

/**
 *	Gets the ChessPiece in the specified position.
 *	@param board - the game board.
 *	@param pos - the position in which the piece we want to return
 *	@return
 *	Returns the piece in the given pos.
 *	If board is NULL or pos is not a valid position on board,
 *	returns undefined value.
 */
ChessPiece chessGameGetPieceByPosition(ChessBoard* board,
		ChessPiecePosition pos) {
	ChessPiece piece;
	if (board != NULL && chessGameIsValidPosition(pos))
		piece = board->position[pos.row][pos.column];
	return piece;
}

/**
 *	Checks equality between two given chess positions.
 *	@param pos1 - the first position.
 *	@param pos2 - the second position.
 *	@return
 *	Returns true if both positions are valid and equals, false otherwise.
 */bool chessGameIsPositionEquals(ChessPiecePosition pos1,
		ChessPiecePosition pos2) {
	return chessGameIsValidPosition(pos1) && chessGameIsValidPosition(pos2)
			&& pos1.row == pos2.row && pos1.column == pos2.column;
}
