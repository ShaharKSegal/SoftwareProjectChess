#ifndef GAMESETTINGS_H_
#define GAMESETTINGS_H_
#include "ChessGame.h"

/*
 * Type used for returning error codes from setting functions
 */
typedef enum game_settings_message_t {
	GAME_SETTINGS_GAME_MODE_SUCCESS,
	GAME_SETTINGS_WRONG_GAME_MODE,
	GAME_SETTINGS_DIFFICULTY_LEVEL_SUCCESS,
	GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL,
	GAME_SETTINGS_USER_COLOR_SUCCESS,
	GAME_SETTINGS_WRONG_USER_COLOR,
	GAME_SETTINGS_INVALID_COMMAND,
	GAME_SETTINGS_FILE_FAILURE,
	GAME_SETTINGS_LOAD_FILE_OPEN_FAIL,
	GAME_SETTINGS_LOAD_FILE_FAIL,
	GAME_SETTINGS_LOAD_FILE_SUCCESS,
	GAME_SETTINGS_SAVE_GAME_FAIL,
	GAME_SETTINGS_SAVE_GAME_SUCCESS,
	GAME_SETTINGS_DEFAULT_SUCCESS,
	GAME_SETTINGS_QUIT_SUCCESS,
	GAME_SETTINGS_START_SUCCESS,
	GAME_SETTINGS_PRINT_SUCCESS,
} GAME_SETTINGS_MESSAGE;

/*
 * Difficulty levels definitions - strings.
 */
#define DIFFICULTY_LEVEL_1 "amateur"
#define DIFFICULTY_LEVEL_2 "easy"
#define DIFFICULTY_LEVEL_3 "moderate"
#define DIFFICULTY_LEVEL_4 "hard"
#define DIFFICULTY_LEVEL_5 "expert"

#define DELIMETER " \n"

/*
 * Difficulty levels definitions - integers.
 */
#define DIFFICULTY_LEVEL_1_INT 1
#define DIFFICULTY_LEVEL_2_INT 2
#define DIFFICULTY_LEVEL_3_INT 3
#define DIFFICULTY_LEVEL_4_INT 4
#define DIFFICULTY_LEVEL_5_INT 5

/*
 typedef enum {
	CHESS_DIFFICULTY_AMATEUR = 1,
	CHESS_DIFFICULTY_EASY = 2,
	CHESS_DIFFICULTY_MODERATE = 3,
	CHESS_DIFFICULTY_HARD = 4,
	CHESS_DIFFICULTY_EXPERT = 5
} CHESS_DIFFICULTY;
*/

/*
 * Definitions for game modes
 */
#define ONE_PLAYER '1'
#define TWO_PLAYERS '2'

/*
 * Definitions for user colors
 */
#define WHITE_USER "white\n"
#define BLACK_USER "black\n"

#define PRINT_WHITE_USER "white"
#define PRINT_BLACK_USER "black"

/*
 * Save/Load line definitions
 */
#define GAME_MODE_1_PLAYER_LINE "GAME_MODE: 1-player\n"
#define GAME_MODE_2_PLAYER_LINE "GAME_MODE: 2-player\n"
#define USER_COLOR_WHITE_LINE "USER_COLOR: white\n"
#define DIFFICULTY_LEVEL_IINE "DIFFICULTY:"
#define USER_COLOR_LINE "USER_COLOR:"
#define SETTINGS_LINE "SETTINGS:"



typedef struct game_setting_t {
	ChessGame* chessGame;
	char gameMode;
	int userColor; //relevant for 1-mode only
	unsigned int maxDepth; //relevant for 1-mode only
} GameSettings;


#endif /* GAMESETTINGS_H_ */

/**
 * Creates a new game settings.
 *
 * @return
 * NULL if a memory allocation failure occurs.
 * Otherwise, a new game settings instance is returned.
 */
GameSettings* gameSettingsCreate();

/**
 *	Creates a copy of a given settings.
 *	The new copy has the same status as the src settings.
 *
 *	@param src - the source settings which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 */
GameSettings* gameSettingsCopy(GameSettings* src);

/**
 * Frees all memory allocation associated with a given game settings instance.
 * If settings instance is NULL the function does nothing.
 *
 * @param settings - the source settings
 */
void gameSettingsDestroy(GameSettings* settings);

/**
 * Changes the game settings' game mode.
 *
 * @param settings - The source settings, assumes not NULL, and new game mode (char type).
 * @return
 * GAME_SETTINGS_WRONG_GAME_MODE    - if the new game mode is not ONE_PLAYER nor TWO_PLAYERS.
 * GAME_SETTINGS_GAME_MODE_SUCCESS  - On success. The game mode is updated.
 */
GAME_SETTINGS_MESSAGE gameSettingsChangeGameMode(GameSettings* settings, char gameMode);

/**
 * Changes the game settings' difficulty level.
 *
 * @param settings - The source settings, assumes not NULL, and the new difficulty level (int type).
 * @return
 * GAME_SETTINGS_WRONG_DIFFICULTY_LEVEL    - if the new difficulty level is not an integer between 1 and 5 (including).
 * GAME_SETTINGS_DIFFIVULTY_LEVEL_SUCCESS  - On success. The game difficulty level is updated.
 * GAME_SETTINGS_INVALID_COMMAND           - if the game mode is not one player.
 */
GAME_SETTINGS_MESSAGE gameSettingsChangeDifficulty(GameSettings* settings, int difficulty);

/**
 * Changes the user's color.
 *
 * @param settings - The source settings, assumes not NULL, and the new user color (int type).
 * @return
 * GAME_SETTINGS_WRONG_USER_COLOR    - if the new user color level is not white(1) or black (0).
 * GAME_SETTINGS_USER_COLOR_SUCCESS  - On success. The user color is updated.
 * GAME_SETTINGS_INVALID_COMMAND     - if the game mode is not one player.
 */
GAME_SETTINGS_MESSAGE gameSettingsChangeUserColor(GameSettings* settings, int userColor);

/*
 * Prints the current game settings to screen.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_PRINT_SETTINGS_FAIL    - if a printing failure occurs.
 * GAME_SETTINGS_PRINT_SETTINGS_SUCCESS - otherwise.
 */
GAME_SETTINGS_MESSAGE gameSettingsPrintSettingsToUser(GameSettings* settings);

/*
 * Prints the settings that are shared by the two game modes, and handles each separately.
 *
 * @param settings - the current game settings, file - the file to write the settings to.
 *
 */
void gameSettingsPrint(FILE* file, GameSettings* settings);

/*
 * Resets all game settings to default values.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_DEFAULT_SUCCESS - all values are default.
 */
GAME_SETTINGS_MESSAGE gameSettingsDefaulter (GameSettings* settings);

/*
 * Terminates the program. All memory resources will be freed (a settings command).
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_QUIT_SUCCESS - the program is exiting.
 */
GAME_SETTINGS_MESSAGE gameSettingsQuitGameSettings(GameSettings* settings);

/*
 * Terminates the program. All memory resources will be freed (a game command).
 *
 * @param settings - the current game settings
 *
 *@return
 * CHESS_GAME_QUIT_SUCCESS - the program is exiting.
 */
CHESS_GAME_MESSAGE gameSettingsQuitGame(GameSettings* settings);

/*
 * Converts the integer value of the difficulty level to its name.
 *
 * @param level - the difficulty level value.
 *
 *@return
 * the difficulty level name.
 */
char* gameSettingsDifficultyLevelToString(unsigned int level);

/*
 * Converts the name of the difficulty level to its integer value.
 *
 * @param level - the difficulty level name.
 *
 *@return
 * the difficulty level value.
 */
int gameSettingsCharDifficultyLevelToInt(char* level);

/*
 * Restarts the games, keeps the settings values of the previous game and start a new game.
 *
 * @param settings - the current game settings
 *
 *@return
 * CHESS_GAME_ERROR - if an error has occurred in creating the game.
 * CHESS_GAME_RESTART - if the game restarted successfully.
 */
CHESS_GAME_MESSAGE gameSettingsRestart(GameSettings* settings);

/*
 * Starts the game. Once this command is asked, the state is shifted to game state.
 *
 * @param settings - the current game settings
 *
 *@return
 * GAME_SETTINGS_START_SUCCESS - the game starts.
 */
GAME_SETTINGS_MESSAGE gameSettingsStart ();

