#include <string.h>
#include <stdlib.h>
#include "unit_test_util.h"
#include "ChessParser.h"

static bool spParserCheckParseLine() {
	CmdCommand* cmd = NULL;
	bool isSettings = true;
	bool memFailure = false;

	// Truthful settings lines
	cmd = parseLine("game_mode 17", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_GAME_MODE && *((int* )cmd->arg) == 17);
	cmd = parseLine("difficulty \t	421\n", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_DIFFICULTY && *((int* )cmd->arg) == 421);
	cmd = parseLine("	user_color 42\n", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_USER_COLOR && *((int* )cmd->arg) == 42);
	cmd = parseLine("load D:\\University", isSettings, &memFailure);
	ASSERT_TRUE(
			cmd->cmd == CMD_LOAD
					&& !strcmp((char* )cmd->arg, "D:\\University"));
	cmd = parseLine("default", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_DEFAULT);
	cmd = parseLine("print_settings", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_PRINT_SETTINGS);
	cmd = parseLine("quit", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_QUIT);
	cmd = parseLine("start", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_START);

	isSettings = !isSettings;

	// Truthful game lines
	cmd = parseLine("move some_str to other_str", isSettings, &memFailure);
	ASSERT_TRUE(
			cmd->cmd == CMD_MOVE && !strcmp("some_str", *((char** )cmd->arg))
					&& !strcmp("other_str", *((char** )cmd->arg + 1)));
	cmd = parseLine("get_moves \t	421 \n", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_GET_MOVES && !strcmp("421", (char* ) cmd->arg));
	cmd = parseLine("	save D:\\University\n", isSettings, &memFailure);
	ASSERT_TRUE(
			cmd->cmd == CMD_SAVE
					&& !strcmp((char* ) cmd->arg, "D:\\University"));
	cmd = parseLine("undo", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_UNDO);
	cmd = parseLine("reset", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_RESET);
	cmd = parseLine("quit", isSettings, &memFailure);
	ASSERT_TRUE(cmd->cmd == CMD_QUIT);

	// dumb lies!!!
	cmd = parseLine("add_disc 17", isSettings, &memFailure); // non existing cmd
	ASSERT_TRUE(cmd->cmd == CMD_INVALID);
	cmd = parseLine("load", isSettings, &memFailure); // not enough args
	ASSERT_TRUE(cmd->cmd == CMD_INVALID);
	cmd = parseLine("move some_str to other_str to another", isSettings,
			&memFailure); // too many args
	ASSERT_TRUE(cmd->cmd == CMD_INVALID);
	return true;
}
int main() {
	RUN_TEST(spParserCheckParseLine);
	return 0;
}
