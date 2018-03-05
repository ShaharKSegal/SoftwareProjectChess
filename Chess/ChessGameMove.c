#include <stdio.h>
#include <stdlib.h>
#include "ChessErrorHandler.h"
#include "ArrayList.h"
#include "ChessGameMove.h"

/**
 * Declaration of ArrayList maximum size for each piece
 */
const static int pawnArrayMaxSize = 4;
const static int bishopArrayMaxSize = 13;
const static int knightArrayMaxSize = 8;
const static int rookArrayMaxSize = 14;
const static int queenArrayMaxSize = 27;
const static int kingArrayMaxSize = 8;

//TODO: document all this shit.

static int getOpponent(ChessPiece piece) {
	return piece.player == CHESS_WHITE_PLAYER ?
			CHESS_BLACK_PLAYER : CHESS_WHITE_PLAYER;
}

static bool isEmptyPosition(ChessBoard* board, ChessPiecePosition pos) {
	return chessGameGetPieceByPosition(board, pos).type == CHESS_PIECE_EMPTY;
}

static void createAndAddMove(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos, ChessPiecePosition newPos) {
	ChessMove move = { .previousPosition = pos, .currentPosition = newPos,
			.capturedPiece = chessGameGetPieceByPosition(board, newPos),
			.isThreatened = false };
	arrayListAddLast(arr, move);
}

static bool tryMoveToPosition(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos, int vDirection, int hDirection) {
	while (!chessGameIsPositionEquals(pos, newPos)) {
		pos.row += vDirection;
		pos.column += hDirection;
		if (!isEmptyPosition(board, pos)
				&& !chessGameIsPositionEquals(pos, newPos))
			return false;
	}
	return true;
}

static void tryAddRelativePosition(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos, int rowAdd, int colAdd) {
	ChessPiecePosition newPos = (ChessPiecePosition ) { .row = pos.row + rowAdd,
					.column = pos.column + colAdd };
	if (chessMoveIsValidMove(board, pos, newPos))
		createAndAddMove(arr, board, pos, newPos);
}

static void addMovesInDirection(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos, int vDirection, int hDirection) {
	int player = chessGameGetPieceByPosition(board, pos).player;
	ChessPiecePosition newPos = pos;
	do {
		newPos.row += vDirection;
		newPos.column += hDirection;
		if (chessGameIsValidPosition(newPos)
				&& player != chessGameGetPieceByPosition(board, newPos).player)
			createAndAddMove(arr, board, pos, newPos);
	} while (chessGameIsValidPosition(newPos) && isEmptyPosition(board, newPos));
}

static int getArraySizeByPieceType(ChessPiece piece) {
	int size = 0;
	switch (piece.type) {
	case CHESS_PIECE_PAWN:
		size = pawnArrayMaxSize;
		break;
	case CHESS_PIECE_BISHOP:
		size = bishopArrayMaxSize;
		break;
	case CHESS_PIECE_KNIGHT:
		size = knightArrayMaxSize;
		break;
	case CHESS_PIECE_ROOK:
		size = rookArrayMaxSize;
		break;
	case CHESS_PIECE_QUEEN:
		size = queenArrayMaxSize;
		break;
	case CHESS_PIECE_KING:
		size = kingArrayMaxSize;
		break;
	default:
		break;
	}
	return size;
}

static bool isValidMovePawn(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos, int rowDiff, int colDiff) {
	ChessPiece piece = chessGameGetPieceByPosition(board, pos);
	ChessPiece newPosPiece = chessGameGetPieceByPosition(board, newPos);
	rowDiff *= piece.player == CHESS_WHITE_PLAYER ? 1 : -1;
	if (rowDiff == 1) {
		if (!colDiff && isEmptyPosition(board, newPos)) // regular move
			return true;
		else if (colDiff * colDiff == 1
				&& newPosPiece.player == getOpponent(piece)) // capturing
			return true;
	}
	if (rowDiff == 2 && !colDiff && isEmptyPosition(board, newPos))
		return true;
	return false;
}

static bool isValidMoveBishop(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos, int rowDiff, int colDiff) {
	// Checks if the direction is diagonal
	if (rowDiff * rowDiff != colDiff * colDiff)
		return false;

	// Check diagonal direction
	int vDirection = rowDiff > 0 ? 1 : -1;
	int hDirection = colDiff > 0 ? 1 : -1;
	return tryMoveToPosition(board, pos, newPos, vDirection, hDirection);
}

static bool isValidMoveKnight(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos, int rowDiff, int colDiff) {
	// Checks if the move is L shaped.
	// Meaning abs(rowDiff) = (1 or 2) and abs(colDiff) = (2 or 1) respectively.
	if (rowDiff * rowDiff == 1) {
		if (colDiff == 2 || colDiff == -2)
			return true;
	} else if (colDiff * colDiff == 1) {
		if (rowDiff == 2 || rowDiff == -2)
			return true;
	}
	return false;
}

static bool isValidMoveRook(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos, int rowDiff, int colDiff) {
	// Checks if the direction is horizontal/vertical.
	// Using logical NXOR trick to see if exactly one axis diff == 0
	if (!(!rowDiff != !colDiff))
		return false;

	// Get diagonal direction
	int vDirection = 0, hDirection = 0;
	if (rowDiff)
		vDirection = rowDiff > 0 ? 1 : -1;
	else
		hDirection = colDiff > 0 ? 1 : -1;
	return tryMoveToPosition(board, pos, newPos, vDirection, hDirection);
}

static bool isValidMoveQueen(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos, int rowDiff, int colDiff) {
	return isValidMoveRook(board, pos, newPos, rowDiff, colDiff)
			|| isValidMoveBishop(board, pos, newPos, rowDiff, colDiff);
}

static bool isValidMoveKing(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos, int rowDiff, int colDiff) {
	// Checks if each axis step is at most 1
	return (rowDiff * rowDiff) <= 1 && (colDiff * colDiff) <= 1;
}

static void addMovesPawn(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos) {
	int vDir =
			chessGameGetPieceByPosition(board, pos).player == CHESS_WHITE_PLAYER ?
					1 : -1;
	// Check all possible pawn moves
	// regular move
	tryAddRelativePosition(arr, board, pos, vDir, 0);
	// capture on left and right side
	tryAddRelativePosition(arr, board, pos, vDir, -1);
	tryAddRelativePosition(arr, board, pos, vDir, 1);
	// special move at start
	tryAddRelativePosition(arr, board, pos, 2 * vDir, 0);
}

static void addMovesBishop(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos) {
	// Add all moves in the 4 diagonal directions
	addMovesInDirection(arr, board, pos, 1, 1);
	addMovesInDirection(arr, board, pos, -1, 1);
	addMovesInDirection(arr, board, pos, 1, -1);
	addMovesInDirection(arr, board, pos, -1, -1);
}

static void addMovesKnight(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos) {
	// Checks all possible knight moves (L shape)
	for (int rAdd = -2; rAdd <= 2; rAdd++) {
		if (rAdd == 0)
			continue;
		// if rowAdd = -1 or 1, colAdd = -2, else colAdd = -1
		int cAdd = rAdd * rAdd == 1 ? -2 : -1;
		tryAddRelativePosition(arr, board, pos, rAdd, cAdd);
		tryAddRelativePosition(arr, board, pos, rAdd, -cAdd);
	}
}

static void addMovesRook(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos) {
	// Add all moves in the 4 horizontal/vertical directions
	addMovesInDirection(arr, board, pos, 1, 0);
	addMovesInDirection(arr, board, pos, -1, 0);
	addMovesInDirection(arr, board, pos, 0, 1);
	addMovesInDirection(arr, board, pos, 0, -1);
}

static void addMovesQueen(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos) {
	addMovesBishop(arr, board, pos);
	addMovesRook(arr, board, pos);
}

static void addMovesKing(ArrayList* arr, ChessBoard* board,
		ChessPiecePosition pos) {
	// Checks all possible king moves (at most 1 step in each axis)
	for (int rAdd = -1; rAdd <= 1; rAdd++)
		for (int cAdd = -1; cAdd <= 1; cAdd++)
			tryAddRelativePosition(arr, board, pos, rAdd, cAdd);
}

bool chessMoveIsValidMove(ChessBoard* board, ChessPiecePosition pos,
		ChessPiecePosition newPos) {
	//Validate parameters
	if (board == NULL || !chessGameIsValidPosition(pos)
			|| !chessGameIsValidPosition(newPos))
		return false;

	ChessPiece piece = chessGameGetPieceByPosition(board, pos);
	ChessPiece newPosPiece = chessGameGetPieceByPosition(board, newPos);
	// Can't move to the same position or a position with a piece you own.
	if (chessGameIsPositionEquals(pos, newPos)
			|| piece.player == newPosPiece.player)
		return false;

	int rowDiff = newPos.row - pos.row;
	int colDiff = newPos.column - pos.column;
	bool res = false;
	switch (piece.type) {
	case CHESS_PIECE_PAWN:
		res = isValidMovePawn(board, pos, newPos, rowDiff, colDiff);
		break;
	case CHESS_PIECE_BISHOP:
		res = isValidMoveBishop(board, pos, newPos, rowDiff, colDiff);
		break;
	case CHESS_PIECE_KNIGHT:
		res = isValidMoveKnight(board, pos, newPos, rowDiff, colDiff);
		break;
	case CHESS_PIECE_ROOK:
		res = isValidMoveRook(board, pos, newPos, rowDiff, colDiff);
		break;
	case CHESS_PIECE_QUEEN:
		res = isValidMoveQueen(board, pos, newPos, rowDiff, colDiff);
		break;
	case CHESS_PIECE_KING:
		res = isValidMoveKing(board, pos, newPos, rowDiff, colDiff);
		break;
	default:
		break;
	}
	return res;
}

// NOTE: Doesn't update isThreatened field. It should be updated in ChessGame.
ArrayList* chessMoveGetMoves(ChessBoard* board, ChessPiecePosition pos) {
	//Validate parameters
	if (board == NULL || !chessGameIsValidPosition(pos))
		return false;

	ChessPiece piece = chessGameGetPieceByPosition(board, pos);
	ArrayList* arr = arrayListCreate(getArraySizeByPieceType(piece));
	if (arr == NULL )
		return NULL ;
	switch (piece.type) {
	case CHESS_PIECE_PAWN:
		addMovesPawn(arr, board, pos);
		break;
	case CHESS_PIECE_BISHOP:
		addMovesBishop(arr, board, pos);
		break;
	case CHESS_PIECE_KNIGHT:
		addMovesKnight(arr, board, pos);
		break;
	case CHESS_PIECE_ROOK:
		addMovesRook(arr, board, pos);
		break;
	case CHESS_PIECE_QUEEN:
		addMovesQueen(arr, board, pos);
		break;
	case CHESS_PIECE_KING:
		addMovesKing(arr, board, pos);
		break;
	default:
		break;
	}
	return arr;
}
