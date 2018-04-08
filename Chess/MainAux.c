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

char columnIntToChar(int column) {
	switch (column) {
	case 0:
		return 'A';
		break;
	case 1:
		return 'B';
		break;
	case 2:
		return 'C';
		break;
	case 3:
		return 'D';
		break;
	case 4:
		return 'E';
		break;
	case 5:
		return 'F';
		break;
	case 6:
		return 'G';
		break;
	case 7:
		return 'H';
		break;
	default:
		return '0';
	}
}

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
		difficultyLevel = difficultyLevelToMessage(settings->maxDepth);
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

void handlingSettingsCommand(GameSettings* settings, CmdCommand* command) {
	switch (command->cmd) {
	case CMD_GAME_MODE:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_WRONG_GAME_MODE, settings,
					command);
			break;
		}
		settingsMessageToOutput(
				changeGameMode(settings, *((char *) (command->arg))), settings,
				command);
		break;
	case CMD_DIFFICULTY:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL,
					settings, command);
			break;
		}
		settingsMessageToOutput(
				changeDifficulty(settings, *((int *) (command->arg))), settings,
				command);
		break;
	case CMD_USER_COLOR:
		if (!command->argTypeValid) {
			settingsMessageToOutput(GAME_SETTINGS_WRONG_USER_COLOR, settings,
					command);
			break;
		}
		settingsMessageToOutput(
				changeUserColor(settings, *((int *) (command->arg))), settings,
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
		player = (!settings->chessGame->currentPlayer) ?
		PRINT_BLACK_USER :
															PRINT_WHITE_USER;
		printf(GAME_MESSAGE_UNDO_MOVE, player);
		break;
	case CHESS_GAME_SUCCESS:
		break;
	case CHESS_GAME_CHECK:
		player = (!settings->chessGame->currentPlayer) ?
		PRINT_BLACK_USER :
															PRINT_WHITE_USER;
		printf(GAME_MESSAGE_CHECK, player);
		break;
	case CHESS_GAME_DRAW:
		printf(GAME_MESSAGE_DRAW_GAME);
		GameSettingsDestroy(settings);
		break;
	case CHESS_GAME_CHECKMATE:
		player = (settings->chessGame->currentPlayer) ?
		PRINT_BLACK_USER :
														PRINT_WHITE_USER;
		GameSettingsDestroy(settings);
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
 * checks is a string starts with a specific given string.
 */bool startsWith(const char *a, const char *b) {
	if (strncmp(a, b, strlen(b)) == 0)
		return true;
	return false;
}

/*
 * checks is a string ends with a specific given string.
 */bool endsWith(const char * str, const char * suffix) {
	int strLen = strlen(str);
	int suffixLen = strlen(suffix);

	if ((strLen >= suffixLen)
			&& (0 == strcmp(str + (strLen - suffixLen), suffix)))
		return true;
	return false;
}

/*
 * receives the added arg to the command and uninitialized chessPiecePosition instance, and initializes the position if the arg is defined as needed.
 * @return
 * true - if pos is initialized with the correct position values.
 * false - else.
 */bool isPositionValid(char* str, ChessPiecePosition* pos) {
	if (!startsWith(str, "<"))
		return false;
	if (!endsWith(str, ">"))
		return false;
	char* token;
	token = strtok(str, MOVE_DELI);
	if (token == NULL) {
		hadMemoryFailure();
		return false;
	}

	if (!parserCmdIsInt(token))
		return false;
	int i = *token - '0';
	pos->row = i - 1;
	token = strtok(NULL, MOVE_DELI);
	if (token == NULL) {
		hadMemoryFailure();
		return false;
	}

	int j = *token - 'A';
	pos->column = j;
	return true;
}

void printMove(ChessMove move) {
	printf("<%d,%c>", (move.currentPosition.row) + 1,
			columnIntToChar(move.currentPosition.column));
	if (move.isThreatened)
		printf("*");
	if (move.capturedPiece.type != CHESS_PIECE_EMPTY)
		printf("^");
	printf("\n");
}

int handleGetMovesCommand(GameSettings* settings, CmdCommand* command) {
	ChessPiece piece;
	ChessPiecePosition pos = { .row = -1, .column = -1 };
	if (!isPositionValid(((char*) command->arg), &pos)) {
		if (getHadMemoryFailure())
			return -1;
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
/*
 * @return
 * -1 if NULL - memory failure occurred.
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
	ChessPiecePosition fromPosition = { .row = -1, .column = -1 };
	ChessPiecePosition toPosition = { .row = -1, .column = -1 };

	if (!isPositionValid(((char**) command->arg)[0], &fromPosition)
			|| (!isPositionValid(((char**) command->arg)[1], &toPosition))) {
		if (getHadMemoryFailure())
			return -1;
		gameMessageToOutput(CHESS_GAME_INVALID_POSITION, settings);
		return 0;
	}
	CHESS_GAME_MESSAGE message = chessGameSetMove(settings->chessGame,
			fromPosition, toPosition);
	int res;
	if (message == CHESS_GAME_SUCCESS) {
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
 * carries out the undo function and prints the relevant message;
 */
void handleUndoCommand(GameSettings* settings, ChessMove* move) {
	chessGameUndoMove(settings->chessGame);
	gameMessageToOutput(CHESS_GAME_UNDO_SUCCESS, settings);
	printf("<%d,%c> -> <%d,%c>\n", (move->currentPosition.row) + 1,
			columnIntToChar(move->currentPosition.column),
			(move->previousPosition.row) + 1,
			columnIntToChar(move->previousPosition.column));
}

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
 * receives an input from the user and saves it in a variable.
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
 * receives an input from a former process function
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
 * returns the name of the piece based on its type.
 */
char* typeToString(CHESS_PIECE_TYPE type) {
	switch (type) {
	case CHESS_PIECE_PAWN:
		return PAWN;
	case CHESS_PIECE_BISHOP:
		return BISHOP;
	case CHESS_PIECE_KNIGHT:
		return KNIGHT;
	case CHESS_PIECE_ROOK:
		return ROOK;
	case CHESS_PIECE_QUEEN:
		return QUEEN;
	case CHESS_PIECE_KING:
		return KING;
	default:
		return NULL;

	}
}

/*
 * carries out the computer move according to the minimax function
 * @return
 * 1 if the last move resulted in checkmate or draw.
 * 0 if else.
 */
int computerTurn(GameSettings* settings) {
	TreeNode* root = chessGameMinimax(settings);
	ChessMove move = root->bestMove;
	ChessPiece piece = root->bestPiece;
	CHESS_GAME_MESSAGE message = chessGameSetMove(settings->chessGame,
			move.previousPosition, move.currentPosition);
	if (message == CHESS_GAME_SUCCESS)
		printf("Computer: move %s at <%d,%c> to <%d,%c>\n",
				typeToString(piece.type), (move.previousPosition.row) + 1,
				columnIntToChar(move.previousPosition.column),
				(move.currentPosition.row) + 1,
				columnIntToChar(move.currentPosition.column));
	free(root);
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

bool isUserTurn(GameSettings* settings) {
	return settings->userColor == settings->chessGame->currentPlayer;
}

int handleComputerTurn(GameSettings* settings) {
	int state;
	if (settings->gameMode == ONE_PLAYER && !isUserTurn(settings)) {
		state = computerTurn(settings);
		if (state) //last move resulted in checkmate or draw
		{
			GameSettingsDestroy(settings);
			return 1; //quit game
		}
	}
	chessGamePrintBoard(settings->chessGame, stdout);
	return 0;
}

int moveCommandResults(GameSettings* settings, int result) { //todo: check if it's okay to pass the result value like this
	switch (result) {
	case -1: //memory failure
		return -1;
	case 0: 	//invalid command
		return 0;
	case 1: 	//move is set, checkmate or draw
		return 1; //quit game
	case 2: //move is set
		return (handleComputerTurn(settings));
	default:
		return 0;
	}
}

/*
 * returns the color of current player.
 */
char* mainAuxWhichPlayer(GameSettings* settings) {
	return settings->chessGame->currentPlayer ?
	PRINT_WHITE_USER :
												PRINT_BLACK_USER;
}

/*
 * distinguishes between different settings function and handles each one.
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
 * distinguishes between different game function and handles each one.
 * @return
 * -1  - if memory failure occurred.
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
		break;
	case CMD_UNDO: //undo move executed successfully
		if (result && settings->gameMode == ONE_PLAYER && !isUserTurn(settings))
			return handleComputerTurn(settings);
		return 0;
	case CMD_MOVE:
		return moveCommandResults(settings, result);
	default: //save, get_moves
		break;
	}
	return 0;
}
