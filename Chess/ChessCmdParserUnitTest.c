#include <string.h>
#include <stdlib.h>
#include "unit_test_util.h"
#include "ChessCmdParser.h"

static bool chessParserCheckParseLine() {
	CmdCommand* cmd = NULL;
	bool isSettings = true;

	// Truthful settings lines
	cmd = parseLine("game_mode 17", isSettings);
	ASSERT_TRUE(
			cmd->cmd == CMD_GAME_MODE && cmd->argTypeValid
					&& *((int* )cmd->arg) == 17);
	cmd = parseLine("difficulty \t	421\n", isSettings);
	ASSERT_TRUE(
			cmd->cmd == CMD_DIFFICULTY && cmd->argTypeValid
					&& *((int* )cmd->arg) == 421);
	cmd = parseLine("	user_color 42\n", isSettings);
	ASSERT_TRUE(
			cmd->cmd == CMD_USER_COLOR && cmd->argTypeValid
					&& *((int* )cmd->arg) == 42);
	cmd = parseLine("load D:\\University", isSettings);
	ASSERT_TRUE(
			cmd->cmd == CMD_LOAD && cmd->argTypeValid
					&& !strcmp((char* )cmd->arg, "D:\\University"));
	cmd = parseLine("default", isSettings);
	ASSERT_TRUE(cmd->cmd == CMD_DEFAULT && cmd->argTypeValid);
	cmd = parseLine("print_settings", isSettings);
	ASSERT_TRUE(cmd->cmd == CMD_PRINT_SETTINGS && cmd->argTypeValid);
	cmd = parseLine("quit", isSettings);
	ASSERT_TRUE(cmd->cmd == CMD_QUIT && cmd->argTypeValid);
	cmd = parseLine("start", isSettings);
	ASSERT_TRUE(cmd->cmd == CMD_START && cmd->argTypeValid);

	isSettings = !isSettings;

	// Truthful game lines
	cmd = parseLine("move some_str to other_str", isSettings);
	ASSERT_TRUE(
			cmd->cmd == CMD_MOVE && cmd->argTypeValid
					&& !strcmp("some_str", *((char** )cmd->arg))
					&& !strcmp("other_str", *((char** )cmd->arg + 1)));
	cmd = parseLine("get_moves \t	421 \n", isSettings);
	ASSERT_TRUE(
			cmd->cmd == CMD_GET_MOVES && cmd->argTypeValid
					&& !strcmp("421", (char* ) cmd->arg));
	cmd = parseLine("	save D:\\University\n", isSettings);
	ASSERT_TRUE(
			cmd->cmd == CMD_SAVE && cmd->argTypeValid
					&& !strcmp((char* ) cmd->arg, "D:\\University"));
	cmd = parseLine("undo", isSettings);
	ASSERT_TRUE(cmd->cmd == CMD_UNDO && cmd->argTypeValid);
	cmd = parseLine("reset", isSettings);
	ASSERT_TRUE(cmd->cmd == CMD_RESET && cmd->argTypeValid);
	cmd = parseLine("quit", isSettings);
	ASSERT_TRUE(cmd->cmd == CMD_QUIT && cmd->argTypeValid);

	// dumb lies!!!
	cmd = parseLine("add_disc 17", true); // non existing cmd
	ASSERT_TRUE(cmd->cmd == CMD_INVALID);
	cmd = parseLine("add_disc 17", false); // non existing cmd
	ASSERT_TRUE(cmd->cmd == CMD_INVALID);
	cmd = parseLine("game_mode not_int", true); // not an integer arg
	ASSERT_TRUE(cmd->cmd == CMD_GAME_MODE && !cmd->argTypeValid);
	cmd = parseLine("load", true); // not enough args
	ASSERT_TRUE(cmd->cmd == CMD_LOAD && !cmd->argTypeValid);
	cmd = parseLine("move some_str to other_str to another", false); // too many args
	ASSERT_TRUE(cmd->cmd == CMD_MOVE && !cmd->argTypeValid);
	cmd = parseLine("move some_str from other_str", false); // invalid move syntax
	ASSERT_TRUE(cmd->cmd == CMD_MOVE && !cmd->argTypeValid);
	return true;
}
int main() {
	RUN_TEST(chessParserCheckParseLine);
	return 0;
}
