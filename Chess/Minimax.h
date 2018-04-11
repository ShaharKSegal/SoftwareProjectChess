
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
 * Returns a ChessMove that is the computer's ideal move for the relevant difficulty level.
 */
ChessMove chessGameMinimax(GameSettings* settings);

#endif /* MINIMAX_H_ */
