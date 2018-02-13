#include "ChessParser.h"
#include <string.h>
#include <stdlib.h>

#define DELI " \t\r\n"

/**
 * Commands in settings state
 */
#define GAME_MODE "game_mode"
#define DIFFICULTY "difficulty"
#define USER_COLOR "user_color"
#define LOAD "load"
#define DEFAULT "default"
#define PRINT_SETTINGS "print_settings"
#define START "start"
#define QUIT "quit"

/**
 * Commands in game state
 */
#define MOVE "move"
#define UNDO "undo"
#define GET_MOVES "get_moves"
#define SAVE "save"
#define RESET "reset"

/**
 * This string appears between the first argument and the second.
 * Used for command validation
 */
#define MOVE_FORMAT_STR "to"

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool isInt(const char* str) {
	if (str == NULL )
		return false;
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		if (*(str + i) != '0' && !(*(str + i) == '-' && i == 0)) {
			if (!atoi(str))
				return false;
			else
				return true;
		}
	return true;
}

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
char* tokenToCharPtr(bool* memFailure) {
	char* token = strtok(NULL, DELI);
	if (token == NULL )
		return NULL ;
	int len = strlen(token);
	if (!len)
		return NULL ;
	char* res = malloc(sizeof(char) * (len + 1));
	if (res == NULL ) {
		*memFailure = true;
		return NULL ;
	}
	strcpy(res, token);
	return res;
}

/**
 * Checks if there's another token in the current strtok.
 *
 */
bool hasMoreTokens() {
	char* token = strtok(NULL, DELI);
	return token != NULL ;
}

/**
 * Populate arg with an integer pointer.
 * Handles memory failures and invalid cast of the token.
 *
 */
void addIntArg(CmdCommand* command, bool* memFailure) {
	char* token = strtok(NULL, DELI);
	if (!isInt(token)) {
		command->cmd = CMD_INVALID;
		return;
	}
	int* res = malloc(sizeof(int));
	if (res == NULL ) {
		*memFailure = true;
		return;
	}
	*res = atoi(token);
	command->arg = res;
}

/**
 * Populate arg with an char pointer.
 * Handles memory failures.
 *
 */
void addStrArg(CmdCommand* command, bool* memFailure) {
	char* res = tokenToCharPtr(memFailure);
	if (*memFailure || res == NULL )
		return;
	command->arg = res;
}

/**
 * Specific for move command.
 * Checks format and populate arg with a pointer to two char pointers.
 * Only relevant for MOVE command, so the format is always arg1 " to " arg2;
 * Handles memory failures.
 */
void addMoveArg(CmdCommand* command, bool* memFailure) {
	// Get first char pointer argument
	char* res1 = tokenToCharPtr(memFailure);
	if (*memFailure || res1 == NULL ) {
		command->cmd = CMD_INVALID;
		return;
	}
	// Check command format ("to" between the arguments)
	char* token = strtok(NULL, DELI);
	if (strcmp(token, MOVE_FORMAT_STR)) {
		command->cmd = CMD_INVALID;
		free(res1);
		return;
	}
	// Get second char pointer argument
	char* res2 = tokenToCharPtr(memFailure);
	if (*memFailure || res2 == NULL ) {
		command->cmd = CMD_INVALID;
		free(res1);
		return;
	}
	// Allocate and populate arg with the two pointers
	char** res = malloc(sizeof(char*) * 2);
	if (res == NULL ) {
		command->cmd = CMD_INVALID;
		*memFailure = true;
		free(res1);
		free(res2);
		return;
	}
	res[0] = res1;
	res[1] = res2;
	command->arg = res;
}

/**
 * Parses a settings line. The arguments are parsed and added to command->arg,
 * if there's more than one argument an array of pointers will be used.
 * If the argument is of the wrong type (e.g. non-integer), cmd is CMD_INVALID
 * and any memory allocated to arg is freed.
 */
void parseSettingsCommand(char* cmdStr, CmdCommand* command, bool* memFailure) {
	if (!strcmp(cmdStr, GAME_MODE)) {
		command->cmd = CMD_GAME_MODE;
		addIntArg(command, memFailure);
	} else if (!strcmp(cmdStr, DIFFICULTY)) {
		command->cmd = CMD_DIFFICULTY;
		addIntArg(command, memFailure);
	} else if (!strcmp(cmdStr, USER_COLOR)) {
		command->cmd = CMD_USER_COLOR;
		addIntArg(command, memFailure);
	} else if (!strcmp(cmdStr, LOAD)) {
		command->cmd = CMD_LOAD;
		addStrArg(command, memFailure);
	} else if (!strcmp(cmdStr, DEFAULT))
		command->cmd = CMD_DEFAULT;
	else if (!strcmp(cmdStr, PRINT_SETTINGS))
		command->cmd = CMD_PRINT_SETTINGS;
	else if (!strcmp(cmdStr, START))
		command->cmd = CMD_START;
	else
		command->cmd = CMD_INVALID;
}

/**
 * Parses a game line. The arguments are parsed and added to command->arg,
 * if there's more than one argument an array of pointers will be used.
 * If the argument is of the wrong type (e.g. non-integer), cmd is CMD_INVALID
 * and any memory allocated to arg is freed.
 */
void parseGameCommand(char* cmdStr, CmdCommand* command, bool* memFailure) {
	if (!strcmp(cmdStr, MOVE)) {
		command->cmd = CMD_MOVE;
		addMoveArg(command, memFailure);
	} else if (!strcmp(cmdStr, GET_MOVES)) {
		command->cmd = CMD_GET_MOVES;
		addStrArg(command, memFailure);
	} else if (!strcmp(cmdStr, SAVE)) {
		command->cmd = CMD_SAVE;
		addStrArg(command, memFailure);
	} else if (!strcmp(cmdStr, UNDO))
		command->cmd = CMD_UNDO;
	else if (!strcmp(cmdStr, RESET))
		command->cmd = CMD_RESET;
	else
		command->cmd = CMD_INVALID;
}

/**
 * Parses a specified line into a command.
 * The arguments are parsed and added to arg.
 * if there's more than one argument an array of pointers will be used.
 * If the argument is of the wrong type (e.g. non-integer), cmd is CMD_INVALID
 * and any memory allocated to arg is freed.
 *
 * @return
 * A parsed line such that:
 *   cmd  - contains the command type, if the line is invalid then this field is
 *          set to INVALID.
 *   arg - the arguments in case there should be one.
 */
CmdCommand* parseCommand(char* cmdStr, bool isSettings, bool* memFailure) {
	CmdCommand* command = malloc(sizeof(CmdCommand));
	if (command == NULL ) {
		*memFailure = true;
		return NULL ;
	}
	if (!strcmp(cmdStr, QUIT))
		command->cmd = CMD_QUIT;
	else if (isSettings)
		parseSettingsCommand(cmdStr, command, memFailure);
	else
		parseGameCommand(cmdStr, command, memFailure);
	if (*memFailure) {
		free(command);
		return NULL ;
	}
	return command;
}

/**
 * Parses a specified line. The arguments are parsed and added to arg,
 * if there's more than one argument an array of pointers will be used.
 * If the argument is of the wrong type (e.g. non-integer), cmd is CMD_INVALID
 * and any memory allocated to arg is freed.
 * isSettings parameter tells the function what type of commands to expect.
 *
 * @return
 * A parsed line such that:
 *   cmd  - contains the command type, if the line is invalid then this field is
 *          set to INVALID.
 *   arg - the arguments in case there should be one.
 */
CmdCommand* parseLine(char* str, bool isSettings, bool* memFailure) {
	char str2[CMD_MAX_LINE_LENGTH];
	strcpy(str2, str);
	char* token;
	token = strtok(str2, DELI);
	if (token == NULL )
		return NULL ;
	CmdCommand* command = parseCommand(token, isSettings, memFailure);
	if (*memFailure) {
		return NULL ;
	}
	if (hasMoreTokens())
		command->cmd = CMD_INVALID;
	return command;
}
