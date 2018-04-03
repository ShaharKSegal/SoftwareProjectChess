#include "SaveGame.h"
#include "ChessErrorHandler.h"
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
GAME_SETTINGS_MESSAGE chessGameSave(char* fileName, GameSettings* settings) {
	FILE* file = fopen(fileName, "w");
	if (file == NULL ) {
		return GAME_SETTINGS_SAVE_GAME_FAIL;
	}
	char* currentColor;
	int currentPlayer = settings->chessGame->currentPlayer;
	currentColor =
			currentPlayer == CHESS_WHITE_PLAYER ? WHITE_USER : BLACK_USER;
	if (fprintf(file, "%s", currentColor) < 0) {
		hadFileFailure();
		fclose(file);
		return GAME_SETTINGS_SAVE_GAME_FAIL;
	}
	printSettings(file, settings);
	if (getHadFileFailure()) {
		fclose(file);
		return GAME_SETTINGS_SAVE_GAME_FAIL;
	}

	chessGamePrintBoard(settings->chessGame, file);
	fclose(file);
	return getHadFileFailure() ?
			GAME_SETTINGS_SAVE_GAME_FAIL : GAME_SETTINGS_SAVE_GAME_SUCCESS;
}

