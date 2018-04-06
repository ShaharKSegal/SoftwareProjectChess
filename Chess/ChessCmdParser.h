#ifndef CHESSPARSER_H_
#define CHESSPARSER_H_
#include <stdbool.h>
#include "ChessErrorHandler.h"

//specify the maximum line length
#define CMD_MAX_LINE_LENGTH 2048

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
	bool argTypeValid;
	void* arg;
} CmdCommand;

/**
 * Checks if a specified string represents a valid integer.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool parserCmdIsInt(const char* str);

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
 *   argTypeValid - tell whether the arg type is correct (e.g. integer)
 *   arg - the arguments in case there should be one.
 */
CmdCommand* parserCmdParseLine(char* str, bool isSettings);

/**
 * destroy function for the given command.
 * NULL safe.
 */
void parserCmdCommandDestroy(CmdCommand* command);

#endif
