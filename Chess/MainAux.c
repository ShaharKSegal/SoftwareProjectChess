#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ChessGameMove.h"
#include "SaveGame.h"
#include "LoadGame.h"
#include "Minimax.h"
#include "MainAux.h"

/*
 * Definitions of settings messages
 */
#define SETTINGS_MESSAGE_GAME_MODE "Game mode is set to %c-player\n"
#define SETTINGS_MESSAGE_WRONG_MODE "Wrong game mode\n"
#define SETTINGS_MESSAGE_DIFFICULTY_LEVEL "Difficulty level is set to %s\n"
#define SETTINGS_MESSAGE_WRONG_DIFFICULTY_LEVEL "Wrong difficulty level. The value should be between 1 to 5\n"
#define SETTINGS_MESSAGE_USER_COLOR "User color is set to %s"
#define SETTINGS_MESSAGE_WRONG_USER_COLOR "Wrong user color. The value should be 0 or 1\n"
#define SETTINGS_MESSAGE_INVALID_COMMAND "ERROR: invalid command\n"
#define SETTINGS_MESSAGE_FILE_LOAD_FAILURE "Error: File doesn’t exist or cannot be opened\n"
#define SETTINGS_MESSAGE_FILE_SAVE_FAILURE "File cannot be created or modified\n"
#define SETTINGS_MESSAGE_SAVE_FILE "Game saved to: %s\n"
#define SETTINGS_MESSAGE_DEFAULT "All settings reset to default\n"
#define SETTINGS_MESSAGE_EXITING "Exiting..."
#define SETTINGS_MESSAGE_STARTING_GAME "Starting game...\n"
#define SETTINGS_MESSAGE_FILE_ERROR "ERROR: executing the asked function on the relevant file has failed, please try again\n"

/*
 * Definitions of game messages
 */
#define GAME_MESSAGE_INVALID_POSITION_ON_BOARD "Invalid position on the board\n"
#define GAME_MESSAGE_NO_PIECE_FOUND "The specified position does not contain your piece\n"
#define GAME_MESSAGE_NO_PLAYER_PIECE "The specified position does not contain a player piece\n"
#define GAME_MESSAGE_ILLEGAL_MOVE "Illegal move\n"
#define GAME_MESSAGE_ILLEGAL_MOVE_KING_THREATEN "Illegal move: king will be threatened\n"
#define GAME_MESSAGE_ILLEGAL_MOVE_KING_STILL_THREATEN "Illegal move: king is still threatened\n"
#define GAME_MESSAGE_EMPTY_HISTORY "Empty history, no move to undo\n"
#define GAME_MESSAGE_UNDO_MOVE "Undo move for %s player: "
#define GAME_MESSAGE_CHECK "Check: %s king is threatened\n"
#define GAME_MESSAGE_DRAW_GAME "The game ends in a draw\n"
#define GAME_MESSAGE_CHECKMATE "Checkmate! %s player wins the game\n"
#define GAME_MESSAGE_RESTARTING "Restarting...\n"
#define GAME_MESSAGE_EXITING "Exiting..."
#define GAME_MESSAGE_INVALID_COMMAND "Error: Invalid Command\n"

/*
 *
 */
#define PAWN_STR "pawn"
#define KNIGHT_STR "knight"
#define ROOK_STR "rook"
#define BISHOP_STR "bishop"
#define QUEEN_STR "queen"
#define KING_STR "king"

/*
 * Move command consts
 */
#define MOVE_DELI ",<>"
static const char prefix = '<';
static const char suffix = '>';

/**
 * Retrieves the column letter according to its column number, between 0-7.
 */
char columnIntToChar(int column) {
	if (column < 0 || column > CHESS_N_COLUMNS - 1)
		return 0;
	return 'A' + column;
}
/*
 * Handles with printing the needed messages to the user, given the name of the message by a
 * settings-related command, the command and the general settings.
 */
void settingsMessageToOutput(GAME_SETTINGS_MESSAGE message,
		GameSettings* settings, CmdCommand* command) {
	char* difficultyLevel;
	char* userColor;

	switch (message) {
	case GAME_SETTINGS_GAME_MODE_SUCCESS:
		printf(SETTINGS_MESSAGE_GAME_MODE, settings->gameMode);
		break;
	case GAME_SETTINGS_WRONG_GAME_MODE:
		printf(SETTINGS_MESSAGE_WRONG_MODE);
		break;
	case GAME_SETTINGS_DIFFICULTY_LEVEL_SUCCESS:
		difficultyLevel = gameSettingsDifficultyLevelToString(
				settings->maxDepth);
		printf(SETTINGS_MESSAGE_DIFFICULTY_LEVEL, difficultyLevel);
		break;
	case GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL:
		printf(SETTINGS_MESSAGE_WRONG_DIFFICULTY_LEVEL);
		break;
	case GAME_SETTINGS_USER_COLOR_SUCCESS:
		userColor = WHITE_USER;
		if (!settings->userColor)
			userColor = BLACK_USER;
		printf(SETTINGS_MESSAGE_USER_COLOR, userColor);
		break;
	case GAME_SETTINGS_WRONG_USER_COLOR:
		printf(SETTINGS_MESSAGE_WRONG_USER_COLOR);
		break;
	case GAME_SETTINGS_INVALID_COMMAND:
		printf(SETTINGS_MESSAGE_INVALID_COMMAND);
		break;
	case GAME_SETTINGS_PRINT_SUCCESS:
		break;
	case GAME_SETTINGS_LOAD_FILE_FAIL:
		printf(SETTINGS_MESSAGE_FILE_LOAD_FAILURE);
		break;
	case GAME_SETTINGS_LOAD_FILE_OPEN_FAIL:
		printf(SETTINGS_MESSAGE_FILE_LOAD_FAILURE);
		break;
	case GAME_SETTINGS_LOAD_FILE_SUCCESS:
		break;
	case GAME_SETTINGS_SAVE_GAME_FAIL:
		printf(SETTINGS_MESSAGE_FILE_SAVE_FAILURE);
		break;
	case GAME_SETTINGS_SAVE_GAME_SUCCESS:
		printf(SETTINGS_MESSAGE_SAVE_FILE, (char*) command->arg);
		break;
	case GAME_SETTINGS_DEFAULT_SUCCESS:
		printf(SETTINGS_MESSAGE_DEFAULT);
		break;
	case GAME_SETTINGS_QUIT_SUCCESS:
		printf(SETTINGS_MESSAGE_EXITING);
		break;
	case GAME_SETTINGS_START_SUCCESS:
		printf(SETTINGS_MESSAGE_STARTING_GAME);
		break;
	case GAME_SETTINGS_FILE_FAILURE: //problem with loading or saving
		printf(SETTINGS_MESSAGE_FILE_ERROR);
		break;
	}
}

/*
 * Handles with a settings-related command. Once given a command, the function deals with it according
 * to the command's type and sends the proper message to be printed if needed.
 */
void handlingSettingsCommand(GameSettings* settings, CmdCommand* command) {
	switch (command->cmd) {
	case CMD_GAME_MODE:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_WRONG_GAME_MODE, settings,
					command);
			break;
		}
		settingsMessageToOutput(
				gameSettingsChangeGameMode(settings, *((char *) (command->arg))), settings,
				command);
		break;
	case CMD_DIFFICULTY:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL,
					settings, command);
			break;
		}
		settingsMessageToOutput(
				gameSettingsChangeDifficulty(settings, *((int *) (command->arg))), settings,
				command);
		break;
	case CMD_USER_COLOR:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_WRONG_USER_COLOR, settings,
					command);
			break;
		}
		settingsMessageToOutput(
				gameSettingsChangeUserColor(settings, *((int *) (command->arg))), settings,
				command);
		break;
	case CMD_LOAD:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_LOAD_FILE_FAIL, settings,
					command);
			break;
		}
		settingsMessageToOutput(gameSettingsLoad(settings, command->arg),
				settings, command);
		break;
	case CMD_DEFAULT:
		settingsMessageToOutput(gameSettingsDefaulter(settings), settings,
				command);
		break;
	case CMD_PRINT_SETTINGS:
		settingsMessageToOutput(gameSettingsPrintSettingsToUser(settings),
				settings, command);
		break;
	case CMD_START:
		settingsMessageToOutput(gameSettingsStart(), settings, command);
		break;
	case CMD_QUIT:
		settingsMessageToOutput(gameSettingsQuitGameSettings(settings),
				settings, command);
		break;
	case CMD_INVALID:
		settingsMessageToOutput(GAME_SETTINGS_INVALID_COMMAND, settings,
				command);
		break;
	default:
		break;
	}
	return;
}

/*
 * Prints the needed messages to the user, given the name of the message sent by a
 * game-related command, and the general settings.
 */
void gameMessageToOutput(CHESS_GAME_MESSAGE message, GameSettings* settings) {
	char* player;

	switch (message) {
	case CHESS_GAME_INVALID_POSITION:
		printf(GAME_MESSAGE_INVALID_POSITION_ON_BOARD);
		break;
	case CHESS_GAME_NO_PIECE_FOUND:
		printf(GAME_MESSAGE_NO_PIECE_FOUND);
		break;
	case CHESS_GAME_NO_PLAYER_PIECE_FOUND:
		printf(GAME_MESSAGE_NO_PLAYER_PIECE);
		break;
	case CHESS_GAME_INVALID_MOVE:
		printf(GAME_MESSAGE_ILLEGAL_MOVE);
		break;
	case CHESS_GAME_MOVE_THREATEN_KING:
		printf(GAME_MESSAGE_ILLEGAL_MOVE_KING_THREATEN);
		break;
	case CHESS_GAME_UNRESOLVED_THREATENED_KING:
		printf(GAME_MESSAGE_ILLEGAL_MOVE_KING_STILL_THREATEN);
		break;
	case CHESS_GAME_EMPTY_HISTORY:
		printf(GAME_MESSAGE_EMPTY_HISTORY);
		break;
	case CHESS_GAME_UNDO_SUCCESS:
		player = !(settings->chessGame->currentPlayer) ?
		PRINT_BLACK_USER :
															PRINT_WHITE_USER;

		printf(GAME_MESSAGE_UNDO_MOVE, player);
		break;
	case CHESS_GAME_SUCCESS:
		break;
	case CHESS_GAME_CHECK:
		player = !(settings->chessGame->currentPlayer) ?
		PRINT_BLACK_USER :
															PRINT_WHITE_USER;
		printf(GAME_MESSAGE_CHECK, player);
		break;
	case CHESS_GAME_DRAW:
		printf(GAME_MESSAGE_DRAW_GAME);
		gameSettingsDestroy(settings);
		break;
	case CHESS_GAME_CHECKMATE:
		player = (settings->chessGame->currentPlayer) ?
		PRINT_BLACK_USER :
														PRINT_WHITE_USER;
		gameSettingsDestroy(settings);
		printf(GAME_MESSAGE_CHECKMATE, player); //TODO check if not previous player
		break;
	case CHESS_GAME_RESTART:
		printf(GAME_MESSAGE_RESTARTING);
		break;
	case CHESS_GAME_NONE:
		break;
	case CHESS_GAME_QUIT_SUCCESS:
		printf(GAME_MESSAGE_EXITING);
		break;
	case CHESS_GAME_INVALID_COMMAND:
		printf(GAME_MESSAGE_INVALID_COMMAND);
		break;
	case CHESS_GAME_ERROR:
		printCriticalError();
		break;
	}

	return;
}

/*
 * Checks if the format of the position, presented by a string (str) is correct: starts with '<' symbol,
 * ends with '>' symbol and the string contains one comma in between.
 * @return
 * true if the string's format is correct, false otherwise.
 */
bool isFormatValid(char* str) {
	if (!(strlen(str) > 0 && str[0] == prefix))
		return false;
	int length = strlen(str);
	if (!(length > 0 && str[length - 1] == suffix))
		return false;
	int count = 0;
	for (int i = 0; i < length; i++) {
		if (str[i] == ',')
			count++;
	}
	return count == 1;
}

/*
 * receives the added arg to the command and uninitialized chessPiecePosition instance, and initializes the
 * position if the arg is defined as needed.
 * @return
 * true - if pos is initialized with the correct position values.
 * false - the string doesn't contain a legal position on the board, thus pos is not initialized.
 */
bool isPositionValid(char* str, ChessPiecePosition* pos) {
	//Checking to see if the string contain only one '<' and one '>' symbols.
	int prefixCount = 0;
	int suffixCount = 0;
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		if (str[i] == prefix)
			prefixCount++;
		else if (str[i] == suffix)
			suffixCount++;
	}
	if (prefixCount > 1 || suffixCount > 1)
		return false;

	char* token;
	token = strtok(str, MOVE_DELI); //first token - the row's number.

	//Checks to see if the string representing the row of the move is not an empty string.
	if (token == NULL)
		return false;

	//Checks to see if the string representing the row is an integer.
	if (!parserCmdIsInt(token))
		return false;

	//Checks to see if the string representing the row is an integer below 10 (is a single digit).
	if (strlen(token) > 1)
		return false;
	int i = *token - '0';
	if (i > 8 || i < 1)
		return false;

	//Populates the position's row.
	pos->row = i - 1;

	token = strtok(NULL, MOVE_DELI); //next token - the column's char.

	//Checks to see if the string representing the column of the move is not an empty string.
	if (token == NULL)
		return false;

	//Checks to see if the string representing the column is one character.
	if (strlen(token) > 1)
		return false;
	int j = *token - 'A';

	//Checks to see if the string representing the column is one letter between 'A'-'H'.
	if (j > 7 || j < 0)
		return false;

	//Populates the position's column.
	pos->column = j;
	return true;
}

/**
 * sub function of handleGetMovesCommand function, once a move is approved to be written as a legal move,
 * the function prints it and add the needed supplementary: '^' if the move captures a piece, '*' if the move causes
 * the opponent's king to be threatened.
 */
void printMove(ChessMove move) {
	printf("<%d,%c>", (move.currentPosition.row) + 1,
			columnIntToChar(move.currentPosition.column));
	if (move.isThreatened)
		printf("*");
	if (move.capturedPiece.type != CHESS_PIECE_EMPTY)
		printf("^");
	printf("\n");
}

/**
 * Fully handles with the getMoves command.
 * First, checks to see if the format of the position is valid. then, checks to see if the position is valid and if so,
 * populates the position's instance it creates with the correct row and column.
 * Then, it gets the piece in the position and checks if it is indeed the current player's piece.
 * If it is, it prints all legal moves of the piece from its current position.
 *
 * @return
 * 0 if the function won't be executed and the next messages will be sent:
 * CHESS_GAME_INVALID_COMMAND - if the position's format is invalid.
 * CHESS_GAME_INVALID_POSITION - if the position's format is valid but the position on the board is not.
 * CHESS_GAME_NO_PLAYER_PIECE_FOUND - if the position on the board is valid but the player has no
 * piece in this position.
 * 1 - if the function will be executed, and legal moves will be printed to the user (it there are any).
 *
 */
int handleGetMovesCommand(GameSettings* settings, CmdCommand* command) {
	if (!isFormatValid(((char*) command->arg))) {
		gameMessageToOutput(CHESS_GAME_INVALID_COMMAND, settings);
		return 0;
	}

	ChessPiece piece;
	ChessPiecePosition pos = { .row = -1, .column = -1 };
	if (!isPositionValid(((char*) command->arg), &pos)) {
		gameMessageToOutput(CHESS_GAME_INVALID_POSITION, settings);
		return 0;
	}
	piece = chessGameGetPieceByPosition(&(settings->chessGame->gameBoard), pos);
	if (piece.player != settings->chessGame->currentPlayer) {
		gameMessageToOutput(CHESS_GAME_NO_PLAYER_PIECE_FOUND, settings);
		return 0;
	}
	ArrayList* moves = chessGameGetMoves(settings->chessGame, pos);
	for (int i = 0; i < moves->actualSize; i++)
		printMove(arrayListGetAt(moves, i));
	arrayListDestroy(moves);
	return 1;

}
/* Fully handles with the move command.
 * First, checks to see if the format of the positions are valid. then, checks whether the positiona are
 * valid and if so, populates the positions' instances it creates with the correct rows and columns.
 * Then, if the move is set, the state of the board is checked and the return value is according to the state.
 * A proper message is printed in addition.
 *
 * @return
 * 0 if CHESS_GAME_INVALID_POSITION - if cur_pos or next_pos are out-of-range.
 * 		CHESS_GAME_INVALID_MOVE - if the given next_pos is illegal for this piece.
 * 		CHESS_GAME_MOVE_THREATEN_KING - if the move will cause your king to be threatened.
 * 		CHESS_GAME_UNRESOLVED_THREATENED_KINTENED_KING.
 * 1 if CHESS_GAME_DRAW 		- if the game is draw.
 *		CHESS_GAME_CHECKMATE	- if their is a checkmate.
 * 2 if	CHESS_GAME_NONE			- if none of the above is true.
 *      CHESS_GAME_CHECK		- if the game is in check.
 *
 */
int handleMoveCommand(GameSettings* settings, CmdCommand* command) {
	if (!isFormatValid(((char**) command->arg)[0])
			|| (!isFormatValid(((char**) command->arg)[1]))) {
		gameMessageToOutput(CHESS_GAME_INVALID_COMMAND, settings);
		return 0;
	}

	ChessPiecePosition fromPosition = { .row = -1, .column = -1 };
	ChessPiecePosition toPosition = { .row = -1, .column = -1 };

	if (!isPositionValid(((char**) command->arg)[0], &fromPosition)
			|| (!isPositionValid(((char**) command->arg)[1], &toPosition))) {
		gameMessageToOutput(CHESS_GAME_INVALID_POSITION, settings);
		return 0;
	}
	CHESS_GAME_MESSAGE message = chessGameSetMove(settings->chessGame,
			fromPosition, toPosition);
	int res;
	if (message == CHESS_GAME_SUCCESS) { //the move is set
		message = chessGameGetCurrentState(settings->chessGame);
		if (message == CHESS_GAME_CHECKMATE || message == CHESS_GAME_DRAW)
			res = 1;
		else
			res = 2;
	} else
		res = 0;
	gameMessageToOutput(message, settings);
	return res;

}
/*
 * Carries out the undo function and prints the relevant message;
 */
void handleUndoCommand(GameSettings* settings, ChessMove* move) {
	chessGameUndoMove(settings->chessGame);
	gameMessageToOutput(CHESS_GAME_UNDO_SUCCESS, settings);
	printf("<%d,%c> -> <%d,%c>\n", (move->currentPosition.row) + 1,
			columnIntToChar(move->currentPosition.column),
			(move->previousPosition.row) + 1,
			columnIntToChar(move->previousPosition.column));
}

/*
 * Handles with a game-related command. Once given a command, the function deals with it according
 * to the command's type and sends the proper message to be printed if needed.
 * @return
 * 1 - if the command was fully executed.
 * 0 - otherwise.
 */
int handlingGameCommand(GameSettings* settings, CmdCommand* command) {
	ChessMove move;
	switch (command->cmd) {
	case CMD_RESET:
		gameMessageToOutput(gameSettingsRestart(settings), settings);
		return 1;
	case CMD_SAVE:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_SAVE_GAME_FAIL, settings,
					command);
			return 0;
		}
		settingsMessageToOutput(gameSettingsSave(command->arg, settings),
				settings, command);
		return 1;
	case CMD_UNDO:
		if (arrayListIsEmpty(settings->chessGame->history)) {
			gameMessageToOutput(CHESS_GAME_EMPTY_HISTORY, settings);
			return 0;
		}
		move = arrayListGetLast(settings->chessGame->history); //first move to undo
		handleUndoCommand(settings, &move);
		//second move to undo
		if (!arrayListIsEmpty(settings->chessGame->history)) {
			move = arrayListGetLast(settings->chessGame->history);
			handleUndoCommand(settings, &move);
		}
		chessGamePrintBoard(settings->chessGame, stdout);
		return 1;
	case CMD_GET_MOVES:
		if (!command->argTypeValid) {
			gameMessageToOutput(CHESS_GAME_INVALID_POSITION, settings);
			return 0;
		}
		return handleGetMovesCommand(settings, command);
	case CMD_MOVE:
		if (!command->argTypeValid) {
			gameMessageToOutput(CHESS_GAME_INVALID_POSITION, settings);
			return 0;
		}
		return handleMoveCommand(settings, command);
	case CMD_QUIT:
		gameMessageToOutput(gameSettingsQuitGame(settings), settings);
		return 1;
	case CMD_INVALID:
		gameMessageToOutput(CHESS_GAME_INVALID_COMMAND, settings);
		return 1;
	default:
		return 0;
	}
}

/*
 * Receives an input from the user and saves it in a variable.
 * @return
 * the saved string input from the user.
 */
char* getUserInput() {
	char* cmdStr = malloc(CMD_MAX_LINE_LENGTH);
	if (cmdStr == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	cmdStr[0] = 0; //Ensure empty line if no input delivered
	fgets(cmdStr, CMD_MAX_LINE_LENGTH, stdin);
	if (cmdStr == NULL)
		return NULL;
	return cmdStr;
}

/*
 * Receives an input from a former process function
 * and parses it to create the user's command.
 */
CmdCommand* mainAuxGetUserCommand(bool isSettings) {
	CmdCommand* command;
	char* cmdStr = getUserInput();
	if (cmdStr == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	command = parserCmdParseLine(cmdStr, isSettings);
	if (getHadMemoryFailure())
		return NULL;
	free(cmdStr);
	return command;
}

/*
 * Returns the name of the piece based on its type.
 */
char* typeToString(CHESS_PIECE_TYPE type) {
	switch (type) {
	case CHESS_PIECE_PAWN:
		return PAWN_STR;
	case CHESS_PIECE_BISHOP:
		return BISHOP_STR;
	case CHESS_PIECE_KNIGHT:
		return KNIGHT_STR;
	case CHESS_PIECE_ROOK:
		return ROOK_STR;
	case CHESS_PIECE_QUEEN:
		return QUEEN_STR;
	case CHESS_PIECE_KING:
		return KING_STR;
	default:
		return NULL;

	}
}

/*
 * Carries out the computer move according to the minimax function
 * @return
 * 1 if the last move resulted in checkmate or draw (and the game needs to be ended).
 * 0 if else.
 */
int computerTurn(GameSettings* settings) {
	ChessMove move = chessGameMinimax(settings);
	if (getHadCriticalError())
		return 0;
	ChessPiece piece = chessGameGetPieceByPosition(
			&(settings->chessGame->gameBoard), move.previousPosition);
	CHESS_GAME_MESSAGE message = chessGameSetMove(settings->chessGame,
			move.previousPosition, move.currentPosition); //sets the  move
	if (message == CHESS_GAME_SUCCESS) //if the move was successfully set
		printf("Computer: move %s at <%d,%c> to <%d,%c>\n",
				typeToString(piece.type), (move.previousPosition.row) + 1,
				columnIntToChar(move.previousPosition.column),
				(move.currentPosition.row) + 1,
				columnIntToChar(move.currentPosition.column));
	message = chessGameGetCurrentState(settings->chessGame);
	switch (message) {
	case CHESS_GAME_NONE:
		return 0;
	case CHESS_GAME_CHECK:
		return 0;
	case CHESS_GAME_DRAW:
		return 1;
	case CHESS_GAME_CHECKMATE:
		return 1;
	default:
		return 0;
	}
	return 0;
}

/*
 * Returns true if this is the user's turn (relevant only in 1-game mode).
 */
bool isUserTurn(GameSettings* settings) {
	return settings->userColor == settings->chessGame->currentPlayer;
}

/*
 * Handles with the situation where the user has set the move, and if its a 1-game-mode it's now the computer's turn.
 * Once the computer's move is set, the state of the game is retrieved. if it's checkmate or draw -
 * the function destroys the settings and the game. Otherwise, the function prints the board after the computer's
 * move is set.
 * @return
 * 1 - if the game needs to be over since the computer's last move resulted in checkmate or draw.
 * 0 - otherwise.
 */
int handleComputerTurn(GameSettings* settings) {
	int state;
	if (settings->gameMode == ONE_PLAYER && !isUserTurn(settings)) {
		state = computerTurn(settings);
		if (state) //last move resulted in checkmate or draw
		{
			gameSettingsDestroy(settings);
			return 1; //quit game
		}
	}
	chessGamePrintBoard(settings->chessGame, stdout);
	return 0;
}

/*
 * After the user's move is set, the result determines whether to end the game (in case of draw or checkmate),
 * proceed to the next player's move(2-game mode) or to continue to the computer's move (1-game mode only).
 * @return
 * 0 - if the move hasn't been set.
 * 1 - if the move has been set and resulted in checkmate or draw and the game needs the end.
 * 2 - if the move has been set, and the game continues.
 *
 */
int moveCommandResults(GameSettings* settings, int result) { //todo: check if it's okay to pass the result value like this
	switch (result) {
	case 0: 	//invalid command, command hasn't been executed.
	case 1: 	//move is set, checkmate or draw
		return result; //quit game
	case 2: //move is set, continue to next move (or computer's move)
		return (handleComputerTurn(settings));
	default:
		return 0;
	}
}

/*
 * Returns the color of current player.
 */
char* mainAuxWhichPlayer(GameSettings* settings) {
	return settings->chessGame->currentPlayer ? PRINT_WHITE_USER : PRINT_BLACK_USER;
}

/*
 * Distinguishes between different settings function and handles each one separately.
 * @return
 * 1 - if it's needed to end the game after the command.
 * 0 - else.
 */
int mainAuxSettingsState(GameSettings* settings, CmdCommand* command,
bool* isSettings) {
	handlingSettingsCommand(settings, command);
	if (command->argTypeValid) {
		switch (command->cmd) {
		case CMD_QUIT:
			return 1;
		case CMD_START:
			*isSettings = false;
			if (settings->gameMode == ONE_PLAYER && !isUserTurn(settings)) //computer goes first
				computerTurn(settings);
			chessGamePrintBoard(settings->chessGame, stdout);
			break;
		default:
			break;
		}
	}
	return 0;
}

/*
 * Distinguishes between different game function and handles each one separately.
 * @return
 *  1  - if it's needed to end the game after the command.
 *  0  - else.
 */
int mainAuxGameState(GameSettings* settings, CmdCommand* command,
bool* isSettings) {
	int result;
	result = handlingGameCommand(settings, command);
	switch (command->cmd) {
	case CMD_QUIT:
		return 1;
	case CMD_RESET:
		printf(SETTINGS_STATE_LINE);
		*isSettings = true;
		return 0;
	case CMD_UNDO: //undo move executed successfully
		if (result && settings->gameMode == ONE_PLAYER && !isUserTurn(settings))
			return handleComputerTurn(settings);
		return 0;
	case CMD_MOVE:
		return moveCommandResults(settings, result);
	default: //save, get_moves
		return 0;
	}
	return 0;
}
