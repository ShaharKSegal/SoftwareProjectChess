#include "GameSettings.h"
#include "ChessErrorHandler.h"
#include <stdlib.h>
#include <string.h>


/*
 * Converts the user color integer value to its string name.
 *
 * @param userColor - the user's color.
 *
 * @return
 * the string compatible with the given int,"white/n" or "black/n";
 */
static char* userColorToChar(int userColor) {
	if (userColor == CHESS_WHITE_PLAYER)
		return WHITE_USER;
	return BLACK_USER;
}

/*
 * Prints the settings relevant to 1-game mode.
 */
static void printSettingOnePlayer(FILE* file, GameSettings* settings) {
	if (fprintf(file, "%s", GAME_MODE_1_PLAYER_LINE) < 0) {
		hadFileFailure();
		return;
	}
	if (fprintf(file, "%s %s\n", DIFFICULTY_LEVEL_IINE,
			gameSettingsDifficultyLevelToString(settings->maxDepth)) < 0) {
		hadFileFailure();
		return;
	}
	if (fprintf(file, "%s %s", USER_COLOR_LINE,
			userColorToChar(settings->userColor)) < 0) {
		hadFileFailure();
	}
}

/*
 * Prints the settings relevant to 2-game mode.
 */
static void printSettingTwoPlayers(FILE* file) {
	if (fprintf(file, "%s", GAME_MODE_2_PLAYER_LINE) < 0) {
		hadFileFailure();
	}
}

/**
 * Creates a new game settings.
 *
 * @return
 * NULL if a memory allocation failure occurs.
 * Otherwise, a new game settings instance is returned.
 */
GameSettings* gameSettingsCreate() {
	ChessGame* game = chessGameCreate();
	if (game == NULL)
		return NULL;
	GameSettings* settings = malloc(sizeof(GameSettings));
	if (settings == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	settings->chessGame = game;
	settings->gameMode = ONE_PLAYER; //char gameMode
	if (settings->gameMode == ONE_PLAYER) {
		settings->maxDepth = DIFFICULTY_LEVEL_2_INT; //unsigned int
		settings->userColor = CHESS_WHITE_PLAYER; //int
	}

	return settings;
}

/**
 *	Creates a copy of a given settings.
 *	The new copy has the same status as the src settings.
 *
 *	@param src - the source settings which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 */
GameSettings* gameSettingsCopy(GameSettings* src) {
	if (src == NULL)
		return NULL;
	ChessGame* game = chessGameCopy(src->chessGame);
	if (game == NULL)
		return NULL;
	GameSettings* settings = malloc(sizeof(GameSettings));
	if (settings == NULL) {
		hadMemoryFailure();
		return NULL;
	}
	*settings = *src;
	settings->chessGame = game;
	return settings;
}

/**
 * Frees all memory allocation associated with a given game settings instance.
 * If settings instance is NULL the function does nothing.
 *
 * @param settings - the source settings
 */
void gameSettingsDestroy(GameSettings* settings) {
	if (settings == NULL)
		return;
	chessGameDestroy(settings->chessGame);
	free(settings);
}

/**
 * Changes the user's color.
 *
 * @param settings - The source settings, assumes not NULL, and the new user color (int type).
 * @return
 * GAME_SETTINGS_WRONG_USER_COLOR    - if the new user color level is not white(1) or black (0).
 * GAME_SETTINGS_USER_COLOR_SUCCESS  - On success. The user color is updated.
 * GAME_SETTINGS_INVALID_COMMAND     - if the game mode is not one player.
 */
GAME_SETTINGS_MESSAGE gameSettingsChangeUserColor(GameSettings* settings, int userColor) {
	if (settings->gameMode == ONE_PLAYER) {
		if (userColor != CHESS_WHITE_PLAYER && userColor != CHESS_BLACK_PLAYER)
			return GAME_SETTINGS_WRONG_USER_COLOR;
		settings->userColor = userColor;
		return GAME_SETTINGS_USER_COLOR_SUCCESS;
	} else
		return GAME_SETTINGS_INVALID_COMMAND;
}

/**
 * Changes the game settings' game mode.
 *
 * @param settings - The source settings, assumes not NULL, and new game mode (char type).
 * @return
 * GAME_SETTINGS_WRONG_GAME_MODE    - if the new game mode is not ONE_PLAYER nor TWO_PLAYERS.
 * GAME_SETTINGS_GAME_MODE_SUCCESS  - On success. The game mode is updated.
 */
GAME_SETTINGS_MESSAGE gameSettingsChangeGameMode(GameSettings* settings, char gameMode) {
	if (gameMode == ONE_PLAYER || gameMode == TWO_PLAYERS) {
		settings->gameMode = gameMode;
		return GAME_SETTINGS_GAME_MODE_SUCCESS;
	}
	return GAME_SETTINGS_WRONG_GAME_MODE;
}

/**
 * Changes the game settings' difficulty level.
 *
 * @param settings - The source settings, assumes not NULL, and the new difficulty level (int type).
 * @return
 * GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL    - if the new difficulty level is not an integer between 1 and 5 (including).
 * GAME_SETTINGS_DIFFIVULTY_LEVEL_SUCCESS  - On success. The game difficulty level is updated.
 * GAME_SETTINGS_INVALID_COMMAND           - if the game mode is not one player.
 */
GAME_SETTINGS_MESSAGE gameSettingsChangeDifficulty(GameSettings* settings, int difficulty) {
	if (settings->gameMode == ONE_PLAYER) {
		if (difficulty < 6 && difficulty > 0) {
			settings->maxDepth = difficulty;
			return GAME_SETTINGS_DIFFICULTY_LEVEL_SUCCESS;
		}
		return GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL;
	}
	return GAME_SETTINGS_INVALID_COMMAND;
}

/*
 * Prints the settings that are shared by the two game modes, and handles each separately.
 *
 * @param settings - the current game settings, file - the file to write the settings to.
 *
 */
void gameSettingsPrint(FILE* file, GameSettings* settings) {
	if (fprintf(file, "%s\n", SETTINGS_LINE) < 0) {
		hadFileFailure();
		return;
	}
	if (settings->gameMode == ONE_PLAYER) {
		printSettingOnePlayer(file, settings);
	} else {
		//(settings->gameMode == TWO_PLAYERS){
		printSettingTwoPlayers(file);
	}

}

/*
 * Prints the current game settings to screen.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_PRINT_SETTINGS_FAIL    - if a printing failure occurs.
 * GAME_SETTINGS_PRINT_SETTINGS_SUCCESS - otherwise.
 */
GAME_SETTINGS_MESSAGE gameSettingsPrintSettingsToUser(GameSettings* settings) {
	gameSettingsPrint(stdout, settings);
	if (getHadFileFailure())
		return GAME_SETTINGS_FILE_FAILURE;
	return GAME_SETTINGS_PRINT_SUCCESS;
}

/*
 * Resets all game settings to default values.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_DEFAULT_SUCCESS - all values are default.
 */
GAME_SETTINGS_MESSAGE gameSettingsDefaulter(GameSettings* settings) {
	settings->gameMode = ONE_PLAYER;
	settings->maxDepth = DIFFICULTY_LEVEL_2_INT;
	settings->userColor = CHESS_WHITE_PLAYER;
	return GAME_SETTINGS_DEFAULT_SUCCESS;
}

/*
 * Terminates the program. All memory resources will be freed (a settings command).
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_QUIT_SUCCESS - the program is exiting.
 */
GAME_SETTINGS_MESSAGE gameSettingsQuitGameSettings(GameSettings* settings) {
	gameSettingsDestroy(settings);
	return GAME_SETTINGS_QUIT_SUCCESS;
}

/*
 * Terminates the program. All memory resources will be freed (a game command).
 *
 * @param settings - the current game settings
 *
 *@return
 * CHESS_GAME_QUIT_SUCCESS - the program is exiting.
 */
CHESS_GAME_MESSAGE gameSettingsQuitGame(GameSettings* settings) {
	gameSettingsDestroy(settings);
	return CHESS_GAME_QUIT_SUCCESS;
}

/*
 * Starts the game. Once this command is asked, the state is shifted to game state.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_START_SUCCESS - the game starts.
 */
GAME_SETTINGS_MESSAGE gameSettingsStart() {
	return GAME_SETTINGS_START_SUCCESS;
}

/*
 * Converts the integer value of the difficulty level to its name.
 *
 * @param level - the difficulty level value.
 *
 *@return
 * the difficulty level name.
 */
char* gameSettingsDifficultyLevelToString(unsigned int level) {
	switch (level) {
	case DIFFICULTY_LEVEL_1_INT:
		return DIFFICULTY_LEVEL_1;
	case DIFFICULTY_LEVEL_2_INT:
		return DIFFICULTY_LEVEL_2;
	case DIFFICULTY_LEVEL_3_INT:
		return DIFFICULTY_LEVEL_3;
	case DIFFICULTY_LEVEL_4_INT:
		return DIFFICULTY_LEVEL_4;
	case DIFFICULTY_LEVEL_5_INT:
		return DIFFICULTY_LEVEL_5;
	}
	return NULL;
}

/*
 * Converts the name of the difficulty level to its integer value.
 *
 * @param level - the difficulty level name.
 *
 *@return
 * the difficulty level value.
 */
int gameSettingsCharDifficultyLevelToInt(char* level) {
	if (strcmp(level, DIFFICULTY_LEVEL_1) == 0)
		return DIFFICULTY_LEVEL_1_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_2) == 0)
		return DIFFICULTY_LEVEL_2_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_3) == 0)
		return DIFFICULTY_LEVEL_3_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_4) == 0)
		return DIFFICULTY_LEVEL_4_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_5) == 0)
		return DIFFICULTY_LEVEL_5_INT;
	return -1;
}

/*
 * Restarts the games, keeps the settings values of the previous game and start a new game.
 *
 * @param settings - the current game settings
 *
 *@return
 * CHESS_GAME_ERROR - if an error has occurred in creating the game.
 * CHESS_GAME_RESTART - if the game restarted successfully.
 */
CHESS_GAME_MESSAGE gameSettingsRestart(GameSettings* settings) {
	ChessGame* game = chessGameCreate();
	if (game == NULL) {
		return CHESS_GAME_ERROR; //TODO: game messages deals with this option?
	}
	chessGameDestroy(settings->chessGame);
	settings->chessGame = game;
	return CHESS_GAME_RESTART;
}
