#include <stdio.h>
#include <stdlib.h>
#include "ChessErrorHandler.h"
#include "ChessGameMove.h"
#include "ChessGame.h"

/**
 * Definitions for game pieces initial position.
 */
#define WHITE_OTHER_ROW 0
#define BLACK_OTHER_ROW 7
#define BISHOP_COLUMN_L 2
#define BISHOP_COLUMN_R 5
#define KNIGHT_COLUMN_L 1
#define KNIGHT_COLUMN_R 6
#define ROOK_COLUMN_L 0
#define ROOK_COLUMN_R 7
#define QUEEN_COLUMN 3
#define KING_COLUMN 4

/**
 * Definitions for game pieces symbols.
 */
#define WHITE_PAWN_SYMBOL 'm'
#define BLACK_PAWN_SYMBOL 'M'
#define WHITE_BISHOP_SYMBOL 'b'
#define BLACK_BISHOP_SYMBOL 'B'
#define WHITE_KNIGHT_SYMBOL 'n'
#define BLACK_KNIGHT_SYMBOL 'N'
#define WHITE_ROOK_SYMBOL 'r'
#define BLACK_ROOK_SYMBOL 'R'
#define WHITE_QUEEN_SYMBOL 'q'
#define BLACK_QUEEN_SYMBOL 'Q'
#define WHITE_KING_SYMBOL 'k'
#define BLACK_KING_SYMBOL 'K'
#define EMPTY_ENTRY_SYMBOL '_'

/**
 * Definitions for print game
 */
const static int PRINT_GAME_LINE_SIZE = 21;
const static char PRINT_GAME_ZERO_CHAR = '0';
const static char PRINT_GAME_VERTICAL_BAR = '|';
const static char PRINT_GAME_WHITESPACE = ' ';
const static char* PRINT_GAME_BEFORE_LAST_LINE = "  -----------------  \n";
const static char* PRINT_GAME_LAST_LINE = "   A B C D E F G H   \n";

/**
 * Declaration of history size
 */
const static int historySize = 6;

/**
 * Declaration of chess pieces. These are constants.
 */
const static ChessPiece WHITE_PAWN = { .type = CHESS_PIECE_PAWN, .player =
		CHESS_WHITE_PLAYER, .consoleRepresentation = WHITE_PAWN_SYMBOL };
const static ChessPiece BLACK_PAWN = { .type = CHESS_PIECE_PAWN, .player =
		CHESS_BLACK_PLAYER, .consoleRepresentation = BLACK_PAWN_SYMBOL };
const static ChessPiece WHITE_BISHOP = { .type = CHESS_PIECE_BISHOP, .player =
		CHESS_WHITE_PLAYER, .consoleRepresentation = WHITE_BISHOP_SYMBOL };
const static ChessPiece BLACK_BISHOP = { .type = CHESS_PIECE_BISHOP, .player =
		CHESS_BLACK_PLAYER, .consoleRepresentation = BLACK_BISHOP_SYMBOL };
const static ChessPiece WHITE_KNIGHT = { .type = CHESS_PIECE_KNIGHT, .player =
		CHESS_WHITE_PLAYER, .consoleRepresentation = WHITE_KNIGHT_SYMBOL };
const static ChessPiece BLACK_KNIGHT = { .type = CHESS_PIECE_KNIGHT, .player =
		CHESS_BLACK_PLAYER, .consoleRepresentation = BLACK_KNIGHT_SYMBOL };
const static ChessPiece WHITE_ROOK = { .type = CHESS_PIECE_ROOK, .player =
		CHESS_WHITE_PLAYER, .consoleRepresentation = WHITE_ROOK_SYMBOL };
const static ChessPiece BLACK_ROOK = { .type = CHESS_PIECE_ROOK, .player =
		CHESS_BLACK_PLAYER, .consoleRepresentation = BLACK_ROOK_SYMBOL };
const static ChessPiece WHITE_QUEEN = { .type = CHESS_PIECE_QUEEN, .player =
		CHESS_WHITE_PLAYER, .consoleRepresentation = WHITE_QUEEN_SYMBOL };
const static ChessPiece BLACK_QUEEN = { .type = CHESS_PIECE_QUEEN, .player =
		CHESS_BLACK_PLAYER, .consoleRepresentation = BLACK_QUEEN_SYMBOL };
const static ChessPiece WHITE_KING = { .type = CHESS_PIECE_KING, .player =
		CHESS_WHITE_PLAYER, .consoleRepresentation = WHITE_KING_SYMBOL };
const static ChessPiece BLACK_KING = { .type = CHESS_PIECE_KING, .player =
		CHESS_BLACK_PLAYER, .consoleRepresentation = BLACK_KING_SYMBOL };
const static ChessPiece EMPTY_ENTRY = { .type = CHESS_PIECE_EMPTY, .player = -1,
		.consoleRepresentation = EMPTY_ENTRY_SYMBOL };

/**
 * Sets the the given piece in the given position of the game's board.
 * Updates king position field if piece is king.
 */
void chessGameSetPieceInPosition(ChessGame* game, ChessPiecePosition pos,
		ChessPiece piece) {
	if (piece.type == CHESS_PIECE_KING) {
		if (game->currentPlayer == CHESS_WHITE_PLAYER)
			game->whiteKingPosition = pos;
		else
			game->blackKingPosition = pos;
	}
	game->gameBoard.position[pos.row][pos.column] = piece;
}

ChessPiecePosition chessGameGetPlayerKingPosition(ChessGame* game) {
	return game->currentPlayer == CHESS_WHITE_PLAYER ?
			game->whiteKingPosition : game->blackKingPosition;
}

bool chessGameIsPlayerKingPosition(ChessGame* game, ChessPiecePosition pos) {
	return chessGameIsPositionEquals(pos, chessGameGetPlayerKingPosition(game));
}

ChessPiecePosition chessGameGetOpponentKingPosition(ChessGame* game) {
	return game->currentPlayer == CHESS_WHITE_PLAYER ?
			game->blackKingPosition : game->whiteKingPosition;
}

bool chessGameCheckPositionsForThreat(ChessGame* game, bool currentPlayer) {
	ChessPiecePosition threatPos;
	ChessPiece threatPiece;
	ChessPiecePosition kingPos =
			currentPlayer ?
					chessGameGetPlayerKingPosition(game) :
					chessGameGetOpponentKingPosition(game);
	int player = chessGameGetPieceByPosition(&(game->gameBoard), kingPos).player;
	for (int i = 0; i < CHESS_N_ROWS; i++)
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			threatPos = (ChessPiecePosition ) { .row = i, .column = j };
			threatPiece = chessGameGetPieceByPosition(&(game->gameBoard),
					threatPos);
			if (threatPiece.type == CHESS_PIECE_EMPTY
					|| threatPiece.player == player)
				continue;
			if (chessMoveIsValidMove(&(game->gameBoard), threatPos, kingPos))
				return true;
		}
	return false;
}

/**
 * Changes game->currentPlayer to the other player.
 */
void chessGameChangePlayer(ChessGame* game) {
	char currentPlayer = chessGameGetCurrentPlayer(game);
	if (!currentPlayer)
		return;
	if (currentPlayer == CHESS_WHITE_PLAYER)
		game->currentPlayer = CHESS_BLACK_PLAYER;
	else
		game->currentPlayer = CHESS_WHITE_PLAYER;
}

/**
 * Gets the opposite of game->currentPlayer.
 */
short chessGameGetOpponent(ChessGame* game) {
	return chessGameGetCurrentPlayer(game) == CHESS_WHITE_PLAYER ?
			CHESS_BLACK_PLAYER : CHESS_WHITE_PLAYER;
}

/**
 * Creates a new board for a new game.
 *
 * @return
 * NULL if a memory allocation failure occurred.
 * Otherwise, a new board instance is returned.
 */
ChessBoard chessBoardCreate() {
	ChessBoard board;
	ChessPiece piece;
	for (int i = 0; i < CHESS_N_ROWS; i++)
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			bool isWhite = i == WHITE_OTHER_ROW;
			switch (i) {
			case WHITE_PAWN_ROW:
				piece = WHITE_PAWN;
				break;
			case BLACK_PAWN_ROW:
				piece = BLACK_PAWN;
				break;
			case WHITE_OTHER_ROW:
			case BLACK_OTHER_ROW:
				switch (j) {
				case BISHOP_COLUMN_L:
				case BISHOP_COLUMN_R:
					piece = isWhite ? WHITE_BISHOP : BLACK_BISHOP;
					break;
				case KNIGHT_COLUMN_L:
				case KNIGHT_COLUMN_R:
					piece = isWhite ? WHITE_KNIGHT : BLACK_KNIGHT;
					break;
				case ROOK_COLUMN_L:
				case ROOK_COLUMN_R:
					piece = isWhite ? WHITE_ROOK : BLACK_ROOK;
					break;
				case QUEEN_COLUMN:
					piece = isWhite ? WHITE_QUEEN : BLACK_QUEEN;
					break;
				case KING_COLUMN:
					piece = isWhite ? WHITE_KING : BLACK_KING;
					break;
				}
				break;
			default:
				piece = EMPTY_ENTRY;
			}
			board.position[i][j] = piece;
		}
	return board;
}

/**
 * Creates a new game.
 *
 * @return
 * NULL if a memory allocation failure occurs.
 * Otherwise, a new game instance is returned.
 */
ChessGame* chessGameCreate() {
	ArrayList* list = arrayListCreate(historySize);
	if (list == NULL )
		return NULL ;
	ChessGame* game = malloc(sizeof(ChessGame));
	if (game == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	game->historySize = historySize;
	game->history = list;
	game->currentPlayer = CHESS_WHITE_PLAYER;
	game->gameBoard = chessBoardCreate();
	game->whiteKingPosition = (ChessPiecePosition ) { WHITE_OTHER_ROW,
					KING_COLUMN };
	game->blackKingPosition = (ChessPiecePosition ) { BLACK_OTHER_ROW,
					KING_COLUMN };
	game->isThreatened = false;
	return game;
}

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
ChessGame* chessGameCopy(ChessGame* src) {
	if (src == NULL )
		return NULL ;
	ChessGame* game = chessGameCreate(src->historySize);
	if (game == NULL )
		return NULL ;
	ArrayList* history = arrayListCopy(src->history);
	if (history == NULL )
		return NULL ;
	arrayListDestroy(game->history);
	game->history = history;
	for (int i = 0; i < CHESS_N_ROWS; i++)
		for (int j = 0; j < CHESS_N_COLUMNS; j++)
			game->gameBoard.position[i][j] = src->gameBoard.position[i][j];
	game->currentPlayer = src->currentPlayer;
	game->maxDepth = src->maxDepth;
	return game;
}

/**
 * Frees all memory allocation associated with a given game.
 * If game is NULL the function does nothing.
 *
 * @param game - the source game
 */
void chessGameDestroy(ChessGame* game) {
	if (game == NULL )
		return;
	arrayListDestroy(game->history);
	free(game);
}

/**
 * Checks if a piece can be put in the specified position.
 *
 * @param game - The source game. Assumes not NULL.
 * @param cur_pos - The piece's position on board. Assumes not NULL.
 * @param next_pos - The specified position. Assumes not NULL.
 *
 * @return
 * CHESS_GAME_INVALID_POSITION - if cur_pos or next_pos are out-of-range.
 * CHESS_GAME_INVALID_MOVE - if the given next_pos is illegal for this piece.
 * CHESS_GAME_SUCCESS - otherwise
 */
CHESS_GAME_MESSAGE chessGameIsValidMove(ChessGame* game,
		ChessPiecePosition cur_pos, ChessPiecePosition next_pos) {
	// Checks if positions are valid
	if (!chessGameIsValidPosition(cur_pos)
			|| !chessGameIsValidPosition(next_pos))
		return CHESS_GAME_INVALID_POSITION;

	// Checks moves validity (without validating threats made to the king)
	if (!chessMoveIsValidMove(&(game->gameBoard), cur_pos, next_pos))
		return CHESS_GAME_INVALID_MOVE;
	return CHESS_GAME_SUCCESS;
}

/**
 * Gets all valid moves for the specified position.
 *
 * @param game - The game. Assumes not NULL.
 * @param pos - The piece's position on board. Assumes not NULL.
 *
 * @return
 * CHESS_GAME_INVALID_POSITION - if cur_pos or next_pos are out-of-range.
 * CHESS_GAME_INVALID_MOVE - if the given next_pos is illegal for this piece.
 * CHESS_GAME_SUCCESS - otherwise
 */
ArrayList* chessGameGetMoves(ChessGame* game, ChessPiecePosition pos) {
	return chessMoveGetMoves(&(game->gameBoard), pos);
}

/**
 * Sets the next move in a given game by specifying ChessPiecePosition.
 *
 * @param game - The source game. Assumes not NULL.
 * @param cur_pos - The piece's position on board. Assumes not NULL.
 * @param next_pos - The specified position. Assumes not NULL.
 *
 * @return
 * CHESS_GAME_INVALID_POSITION - if cur_pos or next_pos are out-of-range.
 * CHESS_GAME_INVALID_MOVE - if the given next_pos is illegal for this piece.
 * CHESS_GAME_MOVE_THREATEN_KING - if the move will cause your king to be threatened.
 * CHESS_GAME_UNRESOLVED_THREATENED_KING - if the move doesn't resolve the threatened king.
 * CHESS_GAME_SUCCESS - otherwise
 */
CHESS_GAME_MESSAGE chessGameSetMove(ChessGame* game, ChessPiecePosition cur_pos,
		ChessPiecePosition next_pos) {
	CHESS_GAME_MESSAGE res = chessGameIsValidMove(game, cur_pos, next_pos);
	if (res != CHESS_GAME_SUCCESS)
		return res;

	// Update current player
	chessGameChangePlayer(game);

	// Update history
	ArrayList* history = game->history;
	ChessPiece piece = chessGameGetPieceByPosition(&(game->gameBoard), cur_pos);
	ChessMove move = { .previousPosition = cur_pos, .currentPosition = next_pos,
			.capturedPiece = chessGameGetPieceByPosition(&(game->gameBoard),
					next_pos) };
	ChessMove deletedMove;
	if (arrayListIsFull(history)) {
		deletedMove = arrayListGetFirst(history);
		arrayListRemoveFirst(history);
	}
	arrayListAddLast(history, move);
	chessGameSetPieceInPosition(game, next_pos, piece);
	chessGameSetPieceInPosition(game, cur_pos, EMPTY_ENTRY);

	// Checks if after we made our move, our king is threatened
	if (chessGameCheckPositionsForThreat(game, true)) {
		chessGameUndoMove(game); //Undo the move we made
		if (arrayListIsFull(history)) // Adds deletedMove to history if exists
			arrayListAddFirst(history, deletedMove);
		return game->isThreatened ?
				CHESS_GAME_UNRESOLVED_THREATENED_KING :
				CHESS_GAME_MOVE_THREATEN_KING;
	}
	// Update threatening position.
	game->isThreatened = chessGameCheckPositionsForThreat(game, false);
	return res;
}

/**
 * Undo the last move on the board and changes the current player's turn.
 * If the user invoked this command more than historySize times in a row, an error occurs.
 *
 * @param src - The source game, assumes not NULL.
 * @return
 * CHESS_GAME_EMPTY_HISTORY    - if the user invoked this function more then
 *                               historySize in a row.
 * CHESS_GAME_SUCCESS          - On success. The last move on the board is removed,
 * 								 and the current player is changed.
 */
CHESS_GAME_MESSAGE chessGameUndoMove(ChessGame* game) {
	ArrayList* history = game->history;
	if (arrayListIsEmpty(history))
		return CHESS_GAME_EMPTY_HISTORY;
	ChessMove move = arrayListGetLast(history);
	ChessPiece currPiece = chessGameGetPieceByPosition(&(game->gameBoard),
			move.currentPosition);
	chessGameSetPieceInPosition(game, move.previousPosition, currPiece);
	chessGameSetPieceInPosition(game, move.currentPosition, move.capturedPiece);
	arrayListRemoveLast(history);
	chessGameChangePlayer(game);
	return CHESS_GAME_SUCCESS;
}

/**
 * On success, the function prints the board game,
 * by the defined representation of each piece.
 *
 * @param game - Assumes not NULL.
 */
void chessGamePrintBoard(ChessGame* game) {
	char line[PRINT_GAME_LINE_SIZE + 1];
	line[1] = line[PRINT_GAME_LINE_SIZE - 1] = PRINT_GAME_VERTICAL_BAR;
	line[PRINT_GAME_LINE_SIZE - 2] = PRINT_GAME_WHITESPACE;
	line[PRINT_GAME_LINE_SIZE] = 0;
	for (int i = CHESS_N_ROWS - 1; i >= 0; i--) {
		line[0] = PRINT_GAME_ZERO_CHAR + (char) (i + 1);
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			line[2 * j + 2] = PRINT_GAME_WHITESPACE;
			line[2 * j + 3] =
					game->gameBoard.position[i][j].consoleRepresentation;
		}
		printf("%s\n", line);
	}
	printf(PRINT_GAME_BEFORE_LAST_LINE);
	printf(PRINT_GAME_LAST_LINE);
}

/**
 * Returns the current player of the specified game.
 * @param game - Assume not null
 * @return
 * game->currentPlayer
 */
short chessGameGetCurrentPlayer(ChessGame* game) {
	return game->currentPlayer;
}

/**
 * Checks if there's a winner in the specified game status. The function returns either
 * CHESS_GAME_PLAYER_1_SYMBOL or CHESS_GAME_PLAYER_2_SYMBOL in case there's a winner, where
 * the value returned is the symbol of the winner. If the game is over and there's a tie
 * then the value CHESS_GAME_TIE_SYMBOL is returned. in any other case the null characters
 * is returned.
 * @param src - the source game
 * @return
 * CHESS_GAME_PLAYER_1_SYMBOL - if player 1 won
 * CHESS_GAME_PLAYER_2_SYMBOL - if player 2 won
 * CHESS_GAME_TIE_SYMBOL - If the game is over and there's a tie
 * null character - otherwise
 */
char chessCheckWinner(ChessGame* src) {
	// TODO: implement check winner.
	return 0;
}
