#ifndef CHESSPARSER_H_
#define CHESSPARSER_H_
#include <stdbool.h>
#include "ChessErrorHandler.h"

//specify the maximum line length
#define CMD_MAX_LINE_LENGTH 1024

//a type used to represent a command
typedef enum {
	CMD_GAME_MODE,
	CMD_DIFFICULTY,
	CMD_USER_COLOR,
	CMD_LOAD,
	CMD_DEFAULT,
	CMD_PRINT_SETTINGS,
	CMD_START,
	CMD_QUIT,

	CMD_MOVE,
	CMD_GET_MOVES,
	CMD_UNDO,
	CMD_SAVE,
	CMD_RESET,

	CMD_INVALID, // Generic invalid command
} CMD_COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	CMD_COMMAND cmd;
	void* arg;
} CmdCommand;

/**
 * Parses a specified line. The arguments are parsed and added to arg,
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
CmdCommand* parseLine(char* str, bool isSettings);

#endif
