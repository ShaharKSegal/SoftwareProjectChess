
#ifndef MINIMAX_H_
#define MINIMAX_H_
#include "ChessGameCommon.h"
#include "ChessGame.h"
#include "GameSettings.h"

/*
 * Definitions for pieces' scores
 */
#define PAWN_SCORE 1
#define KNIGHT_SCORE 3
#define BISHOP_SCORE 3
#define ROOK_SCORE 5
#define QUEEN_SCORE 9
#define KING_SCORE 100

/*
 * Board states definitions
 */
#define DRAW 0
#define BLACK_CHECKMATE -1000
#define WHITE_CHECKMATE 1000
#define NO_SPECIAL_SITUATION 2000; //random big number

typedef struct tree_node_t {
	ChessMove move;
	ChessPiece piece;
	int depth;
	int score;
	ChessMove bestMove;
	ChessPiece bestPiece;
} TreeNode;

TreeNode* chessGameMinimax(GameSettings* settings);

#endif /* MINIMAX_H_ */
