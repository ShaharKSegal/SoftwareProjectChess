#include <stdlib.h>
#include <limits.h>
#include "Minimax.h"
#include "ChessErrorHandler.h"

/*
 * Special scores
 */
#define DRAW_SCORE 0
#define BLACK_CHECKMATE_SCORE -1000
#define WHITE_CHECKMATE_SCORE 1000

/*
 * Defines the score for each piece.
 */
static int pieceTypeToScore(CHESS_PIECE_TYPE type) {
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

/*
 * Calculated the board score according to the pieces on it.
 */
static int scoringFunction(ChessBoard* gameBoard) {
	int score = 0;
	for (int i = 0; i < CHESS_N_ROWS; i++) {
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			if (gameBoard->position[i][j].player)
				score += pieceTypeToScore(gameBoard->position[i][j].type);
			else
				score -= pieceTypeToScore(gameBoard->position[i][j].type);
		}
	}
	return score;
}

/*
 * Creates a tree node.
 * @return
 * NULL - if a memory failte occurred.
 * TreeNode* instance otherwise.
 */
static TreeNode* createTreeNode() {
	TreeNode* node = malloc(sizeof(TreeNode));
	if (node == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	return node;
}

/*
 * Checks if the new score is better than the current one.
 * @return
 * true - if the new score is higher and the player is white, or lower and the player is black.
 * false - otherwise.
 */
static bool isBetterScore(int newScore, int oldScore, int player) {
	return player == CHESS_WHITE_PLAYER ?
			newScore > oldScore : newScore < oldScore;
}

/*
 * Checks if the new move has the same score as the current chosen move. if so, checks if it's source position
 * is the more to the left and the lower. if it's the same, checks whether the destination position is the more
 * to the left and the lower.
 * @return
 * true - if the move is better by the above mentioned definitions.
 * false - otherwise.
 */
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

/*
 * Checks the state before executing another round of the recursion.
 * @return
 * 1000 - white player wins.
 * -1000 - black player wins.
 * 0 - the game ends in a draw.
 * the board score - if depth > maxDepth.
 */
static int MinimaxValidation(int depth, int maxDepth, ChessGame* game,
		int player) {
	CHESS_GAME_MESSAGE msg = chessGameGetCurrentState(game);
	if (msg == CHESS_GAME_CHECKMATE)
		return player == CHESS_WHITE_PLAYER ? BLACK_CHECKMATE_SCORE : WHITE_CHECKMATE_SCORE;
	else if (msg == CHESS_GAME_DRAW)
		return DRAW_SCORE;
	else if (depth > maxDepth)
		return scoringFunction(&(game->gameBoard));
	return INT_MAX;
}

/*
 * The recursive algorithm, once called updates the parent node with the best move and returns the score of the said
 * move.
 */
static int MinimaxRec(TreeNode* parent, ChessGame* game, int maxDepth,
		int depth, int player, int parentIdealScore) {
	//Checking whether before entering the recursive part, we've already reached max depth, checkmate or draw.
	if (depth > maxDepth
			|| (chessGameGetCurrentState(game) != CHESS_GAME_NONE
					&& chessGameGetCurrentState(game) != CHESS_GAME_CHECK))
		return MinimaxValidation(depth, maxDepth, game, player);

	//initializing the node score to be the "worst" score for the player.
	int idealScore = player == CHESS_WHITE_PLAYER ? INT_MIN : INT_MAX;
	bool initialized = false;

	//Going through all of the curent player's pieces.
	for (int i = 0; i < CHESS_N_ROWS; i++) {
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			ChessPiece piece = game->gameBoard.position[i][j];

			//skip if not the player's piece
			if (piece.player != player) {
				continue;
			}
			ChessPiecePosition position = { .row = i, .column = j };
			ArrayList* moves = chessGameGetMoves(game, position);

			//going through all moves of a specific piece
			for (int k = 0; k < moves->actualSize; k++) {
				TreeNode* node = createTreeNode();
				if (node == NULL)
					return 0;
				node->move.currentPosition =
						arrayListGetAt(moves, k).currentPosition;
				node->move.previousPosition = position;

				chessGameSetMove(game, position, node->move.currentPosition);
				node->score = MinimaxRec(node, game, maxDepth, depth + 1,
						chessGameGetOpponentByPlayer(player), idealScore);
				chessGameUndoMove(game);

				//Checking whether this move is a better move than the last one chosen.
				if (isBetterScore(node->score, idealScore, player)
						|| isBetterLocation(node->move, parent->bestMove,
								initialized, node->score, idealScore)) {
					initialized = true;
					idealScore = node->score;
					parent->bestMove = node->move; //only relevant if parent is root

					//Pruning - if the current node's score is not better than the parent's current score, return.
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

/*
 * Returns a tree node that holds the computer's ideal move for the relevant difficulty level.
 */
TreeNode* chessGameMinimax(GameSettings* settings) {
	TreeNode* root = createTreeNode();
	ChessGame* copiedGame = chessGameCopyEmptyHistory(settings->chessGame,
			settings->maxDepth);
	int player = copiedGame->currentPlayer;
	int worstScore = player == CHESS_WHITE_PLAYER ? INT_MAX : INT_MIN;
	root->score = MinimaxRec(root, copiedGame, settings->maxDepth, 1, player, worstScore);
	//printf("%d\n", root->score);
	if (getHadMemoryFailure())
		return NULL;
	chessGameDestroy(copiedGame);
	return root;
}
