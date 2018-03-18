#include "unit_test_util.h"
#include "ChessGame.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HISTORY_SIZE 6
/*
 static bool ChessGameValidMoveTest() {
 ChessGame* res = ChessGameCreate(HISTORY_SIZE);
 ASSERT_TRUE(res!=NULL);
 int repeat = CHESS_N_ROWS;
 while (repeat-- > 0) {
 ASSERT_TRUE(ChessGameSetMove(res, 2) == CHESS_GAME_SUCCESS);

 }
 ASSERT_FALSE(ChessGameIsValidMove(res, 2));
 ASSERT_TRUE(ChessGameSetMove(res, 2) == CHESS_GAME_INVALID_MOVE);
 ChessGamePrintBoard(res);
 ChessGameDestroy(res);
 return true;
 }
 */
/*
 ChessGame* res = ChessGameCreate(HISTORY_SIZE);
 ASSERT_TRUE(res!=NULL);
 int repeat = 3;
 while (repeat-- > 0) {
 for (int i = 0; i < CHESS_N_COLUMNS; i++) {
 ASSERT_TRUE(ChessGameSetMove(res, i) == CHESS_GAME_SUCCESS);
 }
 }
 repeat = 20;
 while (repeat-- > 0) {
 ASSERT_TRUE(ChessGameUndoPrevMove(res) == CHESS_GAME_SUCCESS);
 }
 ASSERT_TRUE(ChessGameUndoPrevMove(res) == CHESS_GAME_EMPTY_HISTORY);
 ChessGamePrintBoard(res);
 ChessGameDestroy(res);
 return true;
 }
 */
/*
 static bool ChessGameUndoMoveTest() {
 ChessGame* res = ChessGameCreate(HISTORY_SIZE);
 ASSERT_TRUE(res!=NULL);
 int repeat = 2;
 while (repeat-- > 0) {
 for (int i = 0; i < CHESS_N_COLUMNS; i++) {
 ASSERT_TRUE(ChessGameSetMove(res, i) == CHESS_GAME_SUCCESS);
 }
 }
 repeat = 2;
 while (repeat-- > 0) {
 for (int i = 0; i < CHESS_N_COLUMNS; i++) {
 ASSERT_TRUE(ChessGameUndoPrevMove(res) == CHESS_GAME_SUCCESS);
 }
 }
 ChessGamePrintBoard(res);
 ChessGameDestroy(res);
 return true;
 }
 */

static bool ChessGameSetMoveTest() {
	ChessGame* res = chessGameCreate(HISTORY_SIZE);
	ASSERT_TRUE(res!=NULL);
	ChessPiecePosition pos;
	ChessPiecePosition pos_next;

	//Move pawn valid
	pos = (ChessPiecePosition ) { .row = 1, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 0 };
	chessGameSetMove(res, pos, pos_next);
	chessGamePrintBoard(res);

	//Move invalid positions
	pos = (ChessPiecePosition ) { .row = -1, .column = 0 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next)
					== CHESS_GAME_INVALID_POSITION);
	pos = (ChessPiecePosition ) { .row = 8, .column = 0 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next)
					== CHESS_GAME_INVALID_POSITION);
	pos = (ChessPiecePosition ) { .row = 0, .column = -1 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next)
					== CHESS_GAME_INVALID_POSITION);
	pos = (ChessPiecePosition ) { .row = 0, .column = 8 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next)
					== CHESS_GAME_INVALID_POSITION);
	pos = (ChessPiecePosition ) { .row = 0, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = -1, .column = 0 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next)
					== CHESS_GAME_INVALID_POSITION);

	//Move piece not of the current player
	pos = (ChessPiecePosition ) { .row = 3, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 4, .column = 0 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next) == CHESS_GAME_NO_PIECE_FOUND);
	pos = (ChessPiecePosition ) { .row = 5, .column = 5 };
	pos_next = (ChessPiecePosition ) { .row = 6, .column = 6 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next) == CHESS_GAME_NO_PIECE_FOUND);

	//Move pieces to invalid positions
	pos = (ChessPiecePosition ) { .row = 6, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 0 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next) == CHESS_GAME_INVALID_MOVE);
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 3 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next) == CHESS_GAME_INVALID_MOVE);
	pos = (ChessPiecePosition ) { .row = 7, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 0 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next) == CHESS_GAME_INVALID_MOVE);

	//Make a few valid moves (knight should eat pawn)
	pos = (ChessPiecePosition ) { .row = 7, .column = 1 };
	pos_next = (ChessPiecePosition ) { .row = 5, .column = 2 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 3, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 4, .column = 0 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 5, .column = 2 };
	pos_next = (ChessPiecePosition ) { .row = 4, .column = 0 };
	chessGameSetMove(res, pos, pos_next);

	chessGameDestroy(res);

	//Unresolved king threat check
	res = chessGameCreate(HISTORY_SIZE);
	ASSERT_TRUE(res!=NULL);
	//Scholar's mate
	pos = (ChessPiecePosition ) { .row = 1, .column = 4 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 4 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 6, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 5, .column = 0 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 0, .column = 3 };
	ASSERT_TRUE(chessGameCheckmate(res)== false);
	pos_next = (ChessPiecePosition ) { .row = 2, .column = 5 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 5, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 4, .column = 0 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 0, .column = 5 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 2 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 4, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 0 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 2, .column = 5 };
	pos_next = (ChessPiecePosition ) { .row = 6, .column = 5 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 3, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 2, .column = 0 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next)
					== CHESS_GAME_UNRESOLVED_THREATENED_KING);
	ASSERT_TRUE(chessGameCheckmate(res)== true);
	chessGameDestroy(res);

	//Move threaten king check
	res = chessGameCreate(HISTORY_SIZE);
	ASSERT_TRUE(res!=NULL);
	//Queen threatens king on pawn move
	pos = (ChessPiecePosition ) { .row = 1, .column = 4 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 4 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 6, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 5, .column = 0 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 0, .column = 3 };
	pos_next = (ChessPiecePosition ) { .row = 4, .column = 7 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 6, .column = 5 };
	pos_next = (ChessPiecePosition ) { .row = 5, .column = 5 };
	ASSERT_TRUE(
			chessGameSetMove(res, pos, pos_next)
					== CHESS_GAME_MOVE_THREATEN_KING);
	chessGameDestroy(res);

	return true;
}

static bool ChessGameGetMovesTest() {
	ChessGame* res;
	ArrayList* arr;
	ChessPiecePosition pos, pos_next;

	res = chessGameCreate(HISTORY_SIZE);
	ASSERT_TRUE(res!=NULL);
	//Get knight moves
	pos = (ChessPiecePosition ) { .row = 0, .column = 1 };
	arr = chessGameGetMoves(res, pos);
	printf("Knight moves from pos:<%d,%d>\n", pos.row, pos.column);
	for (int i = 0; i < arr->actualSize; i++) {
		ChessMove move = arrayListGetAt(arr, i);
		printf("move:<%d,%d>%s%s\n", move.currentPosition.row,
				move.currentPosition.column, move.isThreatened ? "*" : "",
				move.capturedPiece.type != CHESS_PIECE_EMPTY ? "^" : "");
	}
	free(arr);
	pos = (ChessPiecePosition ) { .row = 1, .column = 4 };
	pos_next = (ChessPiecePosition ) { .row = 3, .column = 4 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 6, .column = 0 };
	pos_next = (ChessPiecePosition ) { .row = 5, .column = 0 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 0, .column = 3 };
	pos_next = (ChessPiecePosition ) { .row = 4, .column = 7 };
	chessGameSetMove(res, pos, pos_next);
	//Get queen moves
	pos = (ChessPiecePosition ) { .row = 4, .column = 7 };
	arr = chessGameGetMoves(res, pos);
	printf("Queen moves from pos:<%d,%d>\n", pos.row, pos.column);
	for (int i = 0; i < arr->actualSize; i++) {
		ChessMove move = arrayListGetAt(arr, i);
		printf("move:<%d,%d>%s%s\n", move.currentPosition.row,
				move.currentPosition.column, move.isThreatened ? "*" : "",
				move.capturedPiece.type != CHESS_PIECE_EMPTY ? "^" : "");
	}
	free(arr);
	pos = (ChessPiecePosition ) { .row = 6, .column = 6 };
	pos_next = (ChessPiecePosition ) { .row = 5, .column = 6 };
	chessGameSetMove(res, pos, pos_next);
	pos = (ChessPiecePosition ) { .row = 0, .column = 1 };
	//Get pawn moves, can eat queen
	arr = chessGameGetMoves(res, pos_next);
	printf("Pawn moves from pos:<%d,%d>\n", pos_next.row, pos_next.column);
	for (int i = 0; i < arr->actualSize; i++) {
		ChessMove move = arrayListGetAt(arr, i);
		printf("move:<%d,%d>%s%s\n", move.currentPosition.row,
				move.currentPosition.column, move.isThreatened ? "*" : "",
				move.capturedPiece.type != CHESS_PIECE_EMPTY ? "^" : "");
	}
	free(arr);
	chessGameDestroy(res);

	return true;
}

static bool ChessGameBasicTest() {
	ChessGame* res = chessGameCreate(HISTORY_SIZE);
	chessGamePrintBoard(res);
	ASSERT_TRUE(res != NULL);
	ChessGame* res2 = chessGameCopy(res);
	ASSERT_TRUE(res2 != NULL);
	ASSERT_TRUE(res->history != res2->history);
	chessGameDestroy(res);
	chessGameDestroy(res2);
	return true;
}

int main123567() {
	RUN_TEST(ChessGameBasicTest);
	RUN_TEST(ChessGameSetMoveTest);
	RUN_TEST(ChessGameGetMovesTest);

	/*/*
	 RUN_TEST(ChessGameUndoMoveTest);
	 RUN_TEST(ChessGameUndoMoveTest2);
	 RUN_TEST(ChessGameValidMoveTest);
	 RUN_TEST(ChessGameWinnerTest);
	 */
	return 0;
}
