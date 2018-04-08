#include <stdlib.h>
#include <limits.h>
#include "Minimax.h"
#include "ChessErrorHandler.h"

static int fromCharToScore(CHESS_PIECE_TYPE type) {
	int score = 0;
	switch (type) {
	case CHESS_PIECE_PAWN:
		score = 1;
		break;
	case CHESS_PIECE_BISHOP:
		score = 3;
		break;
	case CHESS_PIECE_KNIGHT:
		score = 3;
		break;
	case CHESS_PIECE_ROOK:
		score = 5;
		break;
	case CHESS_PIECE_QUEEN:
		score = 9;
		break;
	case CHESS_PIECE_KING:
		score = 100;
		break;
	case CHESS_PIECE_EMPTY:
		score = 0;
		break;
	}
	return (score);
}

static int scoringFunction(ChessBoard* gameBoard) {
	int score = 0;
	for (int i = 0; i < CHESS_N_ROWS; i++) {
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			if (gameBoard->position[i][j].player)
				score += fromCharToScore(gameBoard->position[i][j].type);
			else
				score -= fromCharToScore(gameBoard->position[i][j].type);
		}
	}
	return score;
}

static TreeNode* createTreeNode(int depth) {
	TreeNode* node = malloc(sizeof(TreeNode));
	if (node == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	node->depth = depth;
	return node;
}

static bool isBetterScore(int newScore, int oldScore, int player) {
	return player == CHESS_WHITE_PLAYER ?
			newScore > oldScore : newScore < oldScore;
	/*
	 if (player == CHESS_WHITE_PLAYER) {
	 if (newScore > oldScore)
	 return true;
	 else
	 return false;
	 } else {
	 if (newScore < oldScore)
	 return true;
	 else
	 return false;
	 }*/
}

static bool isBetterLocation(ChessMove new_move, ChessMove current_move,
bool initialized, int newScore, int idealScore) {
	if (initialized) {
		if (newScore != idealScore)
			return false;
		if (new_move.previousPosition.column
				< current_move.previousPosition.column)
			return true;
		else if (new_move.previousPosition.column
				== current_move.previousPosition.column)
			if (new_move.previousPosition.row
					< current_move.previousPosition.row)
				return true;
			else if (new_move.previousPosition.row
					== current_move.previousPosition.row)
				if (new_move.currentPosition.column
						< current_move.currentPosition.column)
					return true;
				else if (new_move.currentPosition.column
						== current_move.currentPosition.column)
					if (new_move.currentPosition.row
							< current_move.currentPosition.row)
						return true;
					else
						return false;
				else
					return false;
			else
				return false;
		else
			return false;
	}
	return true;
}

static int MinimaxValidation(int depth, int maxDepth, ChessGame* game,
		int player) {
	CHESS_GAME_MESSAGE msg = chessGameGetCurrentState(game);
	if (depth > maxDepth)
		return scoringFunction(&(game->gameBoard));
	else if (msg == CHESS_GAME_DRAW)
		return DRAW;
	else if (msg == CHESS_GAME_CHECKMATE)
		return player == CHESS_WHITE_PLAYER ? WHITE_CHECKMATE : BLACK_CHECKMATE; //TODO: check correctness
	return INT_MAX;
}

static int MinimaxRec(TreeNode* parent, ChessGame* game, int maxDepth,
		int depth, int player, int parentIdealScore) {
	if (depth > maxDepth
			|| (chessGameGetCurrentState(game) != CHESS_GAME_NONE
					&& chessGameGetCurrentState(game) != CHESS_GAME_CHECK))
		return MinimaxValidation(depth, maxDepth, game, player);

	int idealScore = player == CHESS_WHITE_PLAYER ? INT_MIN : INT_MAX;
	bool initialized = false;

	for (int i = 0; i < CHESS_N_ROWS; i++) {
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			ChessPiece piece = game->gameBoard.position[i][j];

			//skip if not the player's piece
			if (piece.player != player) {
				continue;
			}
			ChessPiecePosition position = { .row = i, .column = j };
			ArrayList* moves = chessGameGetMoves(game, position); //all possible player moves for a specific piece

			//going through all moves of a specific piece
			for (int move = 0; move < moves->actualSize; move++) {
				TreeNode* node = createTreeNode(depth);
				if (node == NULL)
					return 0;
				node->piece = piece;
				node->move.currentPosition =
						arrayListGetAt(moves, move).currentPosition;
				node->move.previousPosition = position;

				chessGameSetMove(game, position, node->move.currentPosition);
				node->score = MinimaxRec(node, game, maxDepth, depth + 1,
						chessGameGetOpponentByPlayer(player), idealScore);
				chessGameUndoMove(game);

				if (isBetterScore(node->score, idealScore, player)
						|| isBetterLocation(node->move, parent->bestMove,
								initialized, node->score, idealScore)) {
					initialized = true;
					idealScore = node->score;
					parent->bestMove = node->move; //only relevant if parent is root
					parent->bestPiece = node->piece;
					if (isBetterScore(parentIdealScore, idealScore,
							chessGameGetOpponentByPlayer(player))) {
						free(node);
						arrayListDestroy(moves);
						return idealScore;
					}

				}
				free(node);
			}
			arrayListDestroy(moves);
		}
	}
	return idealScore;
}

TreeNode* chessGameMinimax(GameSettings* settings) {
	TreeNode* root = createTreeNode(0);
	ChessGame* copiedGame = chessGameCopyEmptyHistory(settings->chessGame,
			settings->maxDepth);
	int player = copiedGame->currentPlayer;
	int worstScore = player == CHESS_WHITE_PLAYER ? INT_MAX : INT_MIN;
	root->score = MinimaxRec(root, copiedGame, settings->maxDepth,
			root->depth + 1, player, worstScore);
	if (getHadMemoryFailure())
		return NULL;
	chessGameDestroy(copiedGame);
	return root;
}
