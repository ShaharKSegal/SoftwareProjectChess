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
#define EMPTY_ENTRY_SYMBOL '_'

/**
 * Definitions for print game
 */
static const int PRINT_GAME_LINE_SIZE = 20;
static const char PRINT_GAME_ZERO_CHAR = '0';
static const char PRINT_GAME_VERTICAL_BAR = '|';
static const char PRINT_GAME_WHITESPACE = ' ';
#define PRINT_GAME_BEFORE_LAST_LINE "  -----------------\n"
#define PRINT_GAME_LAST_LINE "   A B C D E F G H\n"

/**
 * Declaration of history size
 */
static const int HISTORY_SIZE = 6;

/**
 * Declaration of chess pieces. These are constants.
 */
static const ChessPiece WHITE_PAWN = { .type = CHESS_PIECE_PAWN, .player =
CHESS_WHITE_PLAYER, .representation = WHITE_PAWN_SYMBOL };
static const ChessPiece BLACK_PAWN = { .type = CHESS_PIECE_PAWN, .player =
CHESS_BLACK_PLAYER, .representation = BLACK_PAWN_SYMBOL };
static const ChessPiece WHITE_BISHOP = { .type = CHESS_PIECE_BISHOP, .player =
CHESS_WHITE_PLAYER, .representation = WHITE_BISHOP_SYMBOL };
static const ChessPiece BLACK_BISHOP = { .type = CHESS_PIECE_BISHOP, .player =
CHESS_BLACK_PLAYER, .representation = BLACK_BISHOP_SYMBOL };
static const ChessPiece WHITE_KNIGHT = { .type = CHESS_PIECE_KNIGHT, .player =
CHESS_WHITE_PLAYER, .representation = WHITE_KNIGHT_SYMBOL };
static const ChessPiece BLACK_KNIGHT = { .type = CHESS_PIECE_KNIGHT, .player =
CHESS_BLACK_PLAYER, .representation = BLACK_KNIGHT_SYMBOL };
static const ChessPiece WHITE_ROOK = { .type = CHESS_PIECE_ROOK, .player =
CHESS_WHITE_PLAYER, .representation = WHITE_ROOK_SYMBOL };
static const ChessPiece BLACK_ROOK = { .type = CHESS_PIECE_ROOK, .player =
CHESS_BLACK_PLAYER, .representation = BLACK_ROOK_SYMBOL };
static const ChessPiece WHITE_QUEEN = { .type = CHESS_PIECE_QUEEN, .player =
CHESS_WHITE_PLAYER, .representation = WHITE_QUEEN_SYMBOL };
static const ChessPiece BLACK_QUEEN = { .type = CHESS_PIECE_QUEEN, .player =
CHESS_BLACK_PLAYER, .representation = BLACK_QUEEN_SYMBOL };
static const ChessPiece WHITE_KING = { .type = CHESS_PIECE_KING, .player =
CHESS_WHITE_PLAYER, .representation = WHITE_KING_SYMBOL };
static const ChessPiece BLACK_KING = { .type = CHESS_PIECE_KING, .player =
CHESS_BLACK_PLAYER, .representation = BLACK_KING_SYMBOL };
static const ChessPiece EMPTY_ENTRY = { .type = CHESS_PIECE_EMPTY, .player =
CHESS_NON_PLAYER, .representation = EMPTY_ENTRY_SYMBOL };

/**
 * Functions declarations (if needed).
 */
/**
 *  Checks if the king of that player is threatened,
 *  using the isPositionThreatened function.
 *  @param game - current game.
 *  @param player - player of the king.
 *	@return
 *	true if the king is threatened, false if it's not.
 */
static bool isKingThreatened(ChessGame* game, int player);

/**
 * Sets the the given piece in the given position of the game's board.
 * Updates king position field if piece is king.
 */
static void setPieceInPosition(ChessGame* game, ChessPiecePosition pos,
		ChessPiece piece) {
	if (piece.type == CHESS_PIECE_KING) {
		if (piece.player == CHESS_WHITE_PLAYER)
			game->whiteKingPosition = pos;
		else
			game->blackKingPosition = pos;
	}
	game->gameBoard.position[pos.row][pos.column] = piece;
}

/**
 * Gets chess piece in the given position of the given game.
 */
static ChessPiece getPieceByPosition(ChessGame* game, ChessPiecePosition pos) {
	return chessGameGetPieceByPosition(&(game->gameBoard), pos);
}

/**
 * Checks if a piece can be put in the specified position.
 * NOTE: Does NOT validate threats made to the king.
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
static CHESS_GAME_MESSAGE chessGameIsValidMove(ChessGame* game,
		ChessPiecePosition cur_pos, ChessPiecePosition next_pos) {
	// Checks if positions are valid
	if (!chessGameIsValidPosition(cur_pos)
			|| !chessGameIsValidPosition(next_pos))
		return CHESS_GAME_INVALID_POSITION;

	//Checks if the position contains a piece of the current player
	ChessPiece piece = getPieceByPosition(game, cur_pos);
	if (piece.player != chessGameGetCurrentPlayer(game))
		return CHESS_GAME_NO_PIECE_FOUND;

	// Checks moves validity (without validating threats)
	if (!chessMoveIsValidMove(&(game->gameBoard), cur_pos, next_pos))
		return CHESS_GAME_INVALID_MOVE;
	return CHESS_GAME_SUCCESS;
}

/**
 *  Checks if the given position is threatened, by going over all the pieces on
 *  board and testing if they can make a move to that position.
 *  @param game
 *  @param pos - the potentially threatened position
 *  @param checkKing -  For any threatening position, should we check if the
 *  					move causes it's own king to be threatened.
 *  					This should be true in case we test threats to regular
 *  					pieces, but in case the threatened pos in a king
 *  					we shouldn't check for that.
 *	@return
 *	true if the position is threatened, false if it's not.
 */
static bool isPositionThreatened(ChessGame* game, ChessPiecePosition pos,
bool checkKing) {
	ChessPiece piece = getPieceByPosition(game, pos);
	ChessPiecePosition threatPos;
	for (int i = 0; i < CHESS_N_ROWS; i++)
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			threatPos = (ChessPiecePosition ) { .row = i, .column = j };
			if (chessMoveIsValidMove(&(game->gameBoard), threatPos, pos)) {
				// need to perform the move in order to check king threats
				if (checkKing) {
					ChessPiece threatPiece = getPieceByPosition(game,
							threatPos);
					// Change board to test for threats
					setPieceInPosition(game, pos, threatPiece);
					setPieceInPosition(game, threatPos, EMPTY_ENTRY);

					bool res = isKingThreatened(game, threatPiece.player);

					// Undo changes to board
					setPieceInPosition(game, threatPos, threatPiece);
					setPieceInPosition(game, pos, piece);
					if (res)
						continue;
				}
				return true;
			}

		}
	return false;
}

/**
 *  Checks if the king of that player is threatened,
 *  using the isPositionThreatened function.
 *  @param game - current game.
 *  @param player - player of the king.
 *	@return
 *	true if the king is threatened, false if it's not.
 */
static bool isKingThreatened(ChessGame* game, int player) {
	ChessPiecePosition kingPos =
			player == CHESS_WHITE_PLAYER ?
					game->whiteKingPosition : game->blackKingPosition;
	return isPositionThreatened(game, kingPos, false);
}

/**
 * Get the current opponent based on the current player in the given game.
 */
static short getCurrentOpponent(ChessGame* game) {
	return chessGameGetCurrentPlayer(game) == CHESS_WHITE_PLAYER ?
			CHESS_BLACK_PLAYER :
			CHESS_WHITE_PLAYER;
}

/**
 * Changes game->currentPlayer to the other player.
 */
static void changePlayer(ChessGame* game) {
	game->currentPlayer = getCurrentOpponent(game);
}

/**
 * Creates a new board for a new game.
 *
 * @return
 * NULL if a memory allocation failure occurred.
 * Otherwise, a new board instance is returned.
 */
static ChessBoard chessBoardCreate() {
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
	ArrayList* list = arrayListCreate(HISTORY_SIZE);
	if (list == NULL)
		return NULL;
	ChessGame* game = malloc(sizeof(ChessGame));
	if (game == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	game->historySize = HISTORY_SIZE;
	game->history = list;
	game->currentPlayer = CHESS_WHITE_PLAYER;
	game->gameBoard = chessBoardCreate();
	game->whiteKingPosition = (ChessPiecePosition ) { WHITE_OTHER_ROW,
			KING_COLUMN };
	game->blackKingPosition = (ChessPiecePosition ) { BLACK_OTHER_ROW,
			KING_COLUMN };
	game->isCheck = false;
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
	if (src == NULL)
		return NULL;
	ArrayList* history = arrayListCopy(src->history);
	if (history == NULL)
		return NULL;
	ChessGame* game = malloc(sizeof(ChessGame));
	if (game == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	*game = *src;
	game->history = history;
	return game;
}

/**
 *	Creates a copy of a given game. The new copy has the same status as the src game,
 *	only with an empty history of flexible size.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
ChessGame* chessGameCopyEmptyHistory(ChessGame* src, int historySize) {
	if (src == NULL)
		return NULL;
	ArrayList* history = arrayListCreate(historySize);
	if (history == NULL)
		return NULL;
	ChessGame* game = malloc(sizeof(ChessGame));
	if (game == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	*game = *src;
	game->history = history;
	return game;
}

/**
 * Frees all memory allocation associated with a given game.
 * If game is NULL the function does nothing.
 *
 * @param game - the source game
 */
void chessGameDestroy(ChessGame* game) {
	if (game == NULL)
		return;
	arrayListDestroy(game->history);
	free(game);
}

/**
 * Gets all valid moves for the specified position.
 *
 * @param game - The game. Assumes not NULL.
 * @param pos - The piece's position on board. Assumes not NULL.
 *
 * @return
 * ArrayList with all possible moves
 */
ArrayList* chessGameGetMoves(ChessGame* game, ChessPiecePosition pos) {
	ArrayList* arr = chessMoveGetMoves(&(game->gameBoard), pos);
	if (arr == NULL)
		return NULL;
	ChessPiece piece = getPieceByPosition(game, pos);
	ChessPiece capturedPiece;
	ChessPiecePosition nextPos;
	for (int i = 0; i < arr->actualSize; i++) {
		ChessMove move = arrayListGetAt(arr, i);
		capturedPiece = move.capturedPiece;
		nextPos = move.currentPosition;

		// Change board to test for threats
		setPieceInPosition(game, nextPos, piece);
		setPieceInPosition(game, pos, EMPTY_ENTRY);

		// Remove moves that threatens the king
		if (isKingThreatened(game, piece.player)) {
			arrayListRemoveAt(arr, i);
			i--;
		} else if (isPositionThreatened(game, nextPos, true)) {
			arrayListIsThreatened(arr, i, true);
		}

		// Undo changes to board
		setPieceInPosition(game, pos, piece);
		setPieceInPosition(game, nextPos, capturedPiece);
	}
	return arr;
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

	// Update board
	ChessPiece piece = getPieceByPosition(game, cur_pos);
	ChessPiece capturedPiece = getPieceByPosition(game, next_pos);
	setPieceInPosition(game, next_pos, piece);
	setPieceInPosition(game, cur_pos, EMPTY_ENTRY);

	// Checks if after we made our move, our king is threatened. If so, undo.
	if (isKingThreatened(game, chessGameGetCurrentPlayer(game))) {
		setPieceInPosition(game, cur_pos, piece);
		setPieceInPosition(game, next_pos, capturedPiece);
		return game->isCheck ?
				CHESS_GAME_UNRESOLVED_THREATENED_KING :
				CHESS_GAME_MOVE_THREATEN_KING;
	}

	// Update current player
	changePlayer(game);

	// Update history
	ArrayList* history = game->history;
	ChessMove move = { .previousPosition = cur_pos, .currentPosition = next_pos,
			.capturedPiece = capturedPiece };
	if (arrayListIsFull(history))
		arrayListRemoveFirst(history);
	arrayListAddLast(history, move);

	// Update threatening position.
	chessGameUpdateIsCheck(game);
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
	ChessPiece currPiece = getPieceByPosition(game, move.currentPosition);
	setPieceInPosition(game, move.previousPosition, currPiece);
	setPieceInPosition(game, move.currentPosition, move.capturedPiece);
	arrayListRemoveLast(history);
	changePlayer(game);
	return CHESS_GAME_SUCCESS;
}

/**
 * On success, the function prints the board game to the file given.
 * Uses the defined representation of each piece.
 *
 * @param game - Assumes not NULL.
 * @param file - Assumes not NULL.
 */
void chessGamePrintBoard(ChessGame* game, FILE* file) {
	if (game == NULL || file == NULL)
		return;
	char line[PRINT_GAME_LINE_SIZE + 1];
	line[1] = line[PRINT_GAME_LINE_SIZE - 1] = PRINT_GAME_VERTICAL_BAR;
	line[PRINT_GAME_LINE_SIZE - 2] = PRINT_GAME_WHITESPACE;
	line[PRINT_GAME_LINE_SIZE] = 0;
	for (int i = CHESS_N_ROWS - 1; i >= 0; i--) {
		line[0] = PRINT_GAME_ZERO_CHAR + (char) (i + 1);
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			line[2 * j + 2] = PRINT_GAME_WHITESPACE;
			line[2 * j + 3] = game->gameBoard.position[i][j].representation;
		}
		if (fprintf(file, "%s\n", line) < 0) {
			hadFileFailure();
			return;
		}
	}
	if (fprintf(file, PRINT_GAME_BEFORE_LAST_LINE) < 0
			|| fprintf(file, PRINT_GAME_LAST_LINE) < 0) {
		hadFileFailure();
	}
}

/**
 * Returns the current player of the specified game.
 * @param game - Assume not null
 * @return
 * game->currentPlayer
 */
int chessGameGetCurrentPlayer(ChessGame* game) {
	return game->currentPlayer;
}

/*
 * returns the opponent of the current player.
 * @param player - the current player.
 * @return
 * CHESS_WHITE_PLAYER if player == CHESS_BLACK_PLAYER.
 * CHESS_BLACK_PLAYER if player == CHESS_WHITE_PLAYER.
 */
int chessGameGetOpponentByPlayer(int player) {
	return player == CHESS_WHITE_PLAYER ?
			CHESS_BLACK_PLAYER : CHESS_WHITE_PLAYER;
}

/**
 * Update game->isCheck of the given game.
 * @param game - the source game
 */
void chessGameUpdateIsCheck(ChessGame* game) {
	if (game != NULL)
		game->isCheck = isKingThreatened(game, chessGameGetCurrentPlayer(game));
}

/**
 * Checks if the current state is checkmate, draw or none of them.
 * @param game - the source game
 * @return
 *  CHESS_GAME_CHECK		- if the game is in check.
 * 	CHESS_GAME_DRAW 		- if the game is draw.
 *	CHESS_GAME_CHECKMATE	- if their's a checkmate.
 *	CHESS_GAME_ERROR		- if an error occurred.
 *	CHESS_GAME_NONE			- if none of the above is true.
 */
CHESS_GAME_MESSAGE chessGameGetCurrentState(ChessGame* game) {
	chessGameUpdateIsCheck(game);
	ChessPiecePosition pos;
	for (int i = 0; i < CHESS_N_ROWS; i++)
		for (int j = 0; j < CHESS_N_COLUMNS; j++) {
			pos = (ChessPiecePosition ) { .row = i, .column = j };
			if (chessGameGetCurrentPlayer(game)
					== getPieceByPosition(game, pos).player) {
				ArrayList* moves = chessGameGetMoves(game, pos);
				if (moves == NULL)
					return CHESS_GAME_ERROR;
				int size = moves->actualSize;
				arrayListDestroy(moves);
				if (size > 0)
					return game->isCheck ? CHESS_GAME_CHECK : CHESS_GAME_NONE;
			}
		}
	return game->isCheck ? CHESS_GAME_CHECKMATE : CHESS_GAME_DRAW;;
}

/**
 * Converts from char to ChessPiece instance.
 * @param piece - the char representing the piece.
 * @return
 * ChessPiece - the chessPiece the char represents.
 */
ChessPiece chessGameCharToChessPieceConverter(char piece) {
	ChessPiece chessPiece;
	switch (piece) {
	case WHITE_PAWN_SYMBOL:
		chessPiece = WHITE_PAWN;
		break;
	case BLACK_PAWN_SYMBOL:
		chessPiece = BLACK_PAWN;
		break;
	case WHITE_BISHOP_SYMBOL:
		chessPiece = WHITE_BISHOP;
		break;
	case BLACK_BISHOP_SYMBOL:
		chessPiece = BLACK_BISHOP;
		break;
	case WHITE_KNIGHT_SYMBOL:
		chessPiece = WHITE_KNIGHT;
		break;
	case BLACK_KNIGHT_SYMBOL:
		chessPiece = BLACK_KNIGHT;
		break;
	case WHITE_ROOK_SYMBOL:
		chessPiece = WHITE_ROOK;
		break;
	case BLACK_ROOK_SYMBOL:
		chessPiece = BLACK_ROOK;
		break;
	case WHITE_QUEEN_SYMBOL:
		chessPiece = WHITE_QUEEN;
		break;
	case BLACK_QUEEN_SYMBOL:
		chessPiece = BLACK_QUEEN;
		break;
	case WHITE_KING_SYMBOL:
		chessPiece = WHITE_KING;
		break;
	case BLACK_KING_SYMBOL:
		chessPiece = BLACK_KING;
		break;
	default:
		chessPiece = EMPTY_ENTRY;
		break;
	}
	return chessPiece;
}
