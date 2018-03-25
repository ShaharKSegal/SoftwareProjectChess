#ifndef SAVEGAME_H_
#define SAVEGAME_H_
#include "GameSettings.h"
#include <stdio.h>

/**
 *	Saves relevant settings and game to a given file.
 *
 *	@param fileName - the route of the needed file, settings - the settings instance.
 *	@return
 *	GAME_SETTINGS_SAVE_GAME_FAIL -  if either the file can't be opened or nodified or a memory allocation failure occurred.
 *	GAME_SETTINGS_SAVE_GAME_FAIL - otherwise.
 *
 */
GAME_SETTINGS_MESSAGE chessGameSave(char* fileName, GameSettings* settings);

#endif /* SAVEGAME_H_ */
