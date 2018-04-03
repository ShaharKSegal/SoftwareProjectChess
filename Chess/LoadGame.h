#ifndef LOADGAME_H_
#define LOADGAME_H_

#include "GameSettings.h"

/**
 *	Creates settings from a given file.
 *
 *	@param fileName - the route of the needed file
 *	@return
 *	GAME_SETTINGS_LOAD_FILE_FAIL    - if either the file can't be opened or it doesn't exist or a memory allocation failure occurred.
 *	GAME_SETTINGS_LOAD_FILE_SUCCESS - Otherwise
 *
 */
GAME_SETTINGS_MESSAGE chessGameLoad(GameSettings* settings, char* fileName);

#endif /* LOADGAME_H_ */

