#include "GameSettings.h"
#include "ChessErrorHandler.h"
#include <stdlib.h>
#include <string.h>

/**
 * Creates a new game settings.
 *
 * @return
 * NULL if a memory allocation failure occurs.
 * Otherwise, a new game settings instance is returned.
 */
GameSettings* GameSettingsCreate() {
	ChessGame* game = chessGameCreate();
	if (game == NULL )
		return NULL ;
	GameSettings* settings = malloc(sizeof(GameSettings));
	if (settings == NULL ) {
		hadMemoryFailure();
		return NULL ;
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
	if (game == NULL )
		return NULL ;
	GameSettings* settings = malloc(sizeof(GameSettings));
	if (settings == NULL ) {
		hadMemoryFailure();
		return NULL ;
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
void GameSettingsDestroy(GameSettings* settings) {
	if (settings == NULL )
		return;
	chessGameDestroy(settings->chessGame);
	free(settings);
}

GAME_SETTINGS_MESSAGE changeUserColor(GameSettings* settings, int userColor) {
	if (settings->gameMode == ONE_PLAYER) {
		if (userColor != CHESS_WHITE_PLAYER && userColor != CHESS_BLACK_PLAYER)
			return GAME_SETTINGS_WRONG_USER_COLOR;
		settings->userColor = userColor;
		return GAME_SETTINGS_USER_COLOR_SUCCESS;
	} else
		return GAME_SETTINGS_INVALID_COMMAND;
}

GAME_SETTINGS_MESSAGE changeGameMode(GameSettings* settings, char gameMode) {
	if (gameMode == ONE_PLAYER || gameMode == TWO_PLAYERS) {
		settings->gameMode = gameMode;
		return GAME_SETTINGS_GAME_MODE_SUCCESS;
	}
	return GAME_SETTINGS_WRONG_GAME_MODE;
}

GAME_SETTINGS_MESSAGE changeDifficulty(GameSettings* settings, int difficulty) {
	if (settings->gameMode == ONE_PLAYER) {
		if (difficulty < 6 && difficulty > 0) {
			settings->maxDepth = difficulty;
			return GAME_SETTINGS_DIFFICULTY_LEVEL_SUCCESS;
		}
		return GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL;
	}
	return GAME_SETTINGS_INVALID_COMMAND;
}

static void printSettingOnePlayer(FILE* file, GameSettings* settings) {
	if (fprintf(file, "%s", GAME_MODE_1_PLAYER_LINE) < 0) {
		hadFileFailure();
		return;
	}
	if (fprintf(file, "%s %s\n", DIFFICULTY_LEVEL_IINE,
			difficultyLevelToMessage(settings->maxDepth)) < 0) {
		hadFileFailure();
		return;
	}
	if (fprintf(file, "%s %s", USER_COLOR_LINE,
			userColorToChar(settings->userColor)) < 0) {
		hadFileFailure();
		return;
	}
	return;
}

static void printSettingTwoPlayers(FILE* file) {
	if (fprintf(file, "%s", GAME_MODE_2_PLAYER_LINE) < 0) {
		hadFileFailure();
	}
	return;
}

void printSettings(FILE* file, GameSettings* settings) {
	if (fprintf(file, "%s\n", SETTINGS_LINE) < 0) {
		hadFileFailure();
		return;
	}
	if (settings->gameMode == ONE_PLAYER) {
		printSettingOnePlayer(file, settings);
	}
	else
	{
		//(settings->gameMode == TWO_PLAYERS){
		printSettingTwoPlayers(file);
	}

}

/*
 * Prints the current game settings.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_PRINT_FAILURE    - if a printing failure occurs.
 * GAME_SETTINGS_PRINT_SUCCESS - otherwise.
 */
GAME_SETTINGS_MESSAGE gameSettingsPrintSettingsToUser(GameSettings* settings) {
	printSettings(stdout, settings);
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
 * Terminates the program. All memory resources will be freed.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_QUIT_SUCCESS - the program is exiting.
 */
GAME_SETTINGS_MESSAGE gameSettingsQuitGameSettings(GameSettings* settings) {
	GameSettingsDestroy(settings);
	return GAME_SETTINGS_QUIT_SUCCESS;
}

/*
 * Terminates the program. All memory resources will be freed.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_QUIT_SUCCESS - the program is exiting.
 */
CHESS_GAME_MESSAGE gameSettingsQuitGame(GameSettings* settings) {
	GameSettingsDestroy(settings);
	return CHESS_GAME_QUIT_SUCCESS;
}

GAME_SETTINGS_MESSAGE gameSettingsStart(){
	return GAME_SETTINGS_START_SUCCESS;
}

char* difficultyLevelToMessage(unsigned int level) {
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

int charDifficultyLevelToInt(char* level) {
	if (strcmp(level, DIFFICULTY_LEVEL_1)==0)
		return DIFFICULTY_LEVEL_1_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_2)==0)
		return DIFFICULTY_LEVEL_2_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_3)==0)
		return DIFFICULTY_LEVEL_3_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_4)==0)
		return DIFFICULTY_LEVEL_4_INT;
	if (strcmp(level, DIFFICULTY_LEVEL_5)==0)
		return DIFFICULTY_LEVEL_5_INT;
	return -1;
}

char* userColorToChar(int userColor) {
	if (userColor == CHESS_WHITE_PLAYER)
		return WHITE_USER;
	return BLACK_USER;
}

/*
 * Resets all game settings to default values.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_DEFAULT_SUCCESS - all values are default.
 */
CHESS_GAME_MESSAGE gameSettingsRestart(GameSettings* settings) {
	settings->gameMode = ONE_PLAYER;
	settings->maxDepth = DIFFICULTY_LEVEL_2_INT;
	settings->userColor = CHESS_WHITE_PLAYER;
	return CHESS_GAME_RESTART;
}
