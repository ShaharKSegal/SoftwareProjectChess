#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "ChessErrorHandler.h"
#include "LoadGame.h"

#define LINE_LENGTH 22

static void checkKingPosition(ChessGame* chessGame, char symbol, int place,
		int lineNumber) {
	if (symbol == WHITE_KING_SYMBOL) {
		chessGame->whiteKingPosition.row = lineNumber;
		chessGame->whiteKingPosition.column = place;
	} else if (symbol == BLACK_KING_SYMBOL) {
		chessGame->blackKingPosition.row = lineNumber;
		chessGame->blackKingPosition.column = place;
	}
}

static unsigned int loadDifficulty(char* line) {
	char* token = strtok(line, DELIMETER);
	token = strtok(NULL, DELIMETER);
	int maxDepth = charDifficultyLevelToInt(token);
	return maxDepth;
}

static int loadUserColor(char* line) {
	if (strcmp(USER_COLOR_WHITE_LINE, line) == 0)
		return CHESS_WHITE_PLAYER;
	else
		return CHESS_BLACK_PLAYER;
}

static char loadGameMode(char* line) {
	if (strcmp(GAME_MODE_1_PLAYER_LINE, line) == 0)
		return ONE_PLAYER;
	else
		return TWO_PLAYERS;
}

static int loadPlayer(char* line) {
	if (strcmp(WHITE_USER, line) == 0)
		return CHESS_WHITE_PLAYER;
	else
		return CHESS_BLACK_PLAYER;

}

static void loadSettings(GameSettings* settings, FILE* file) {
	ChessGame* game = settings->chessGame;
	char line[LINE_LENGTH];
	int lineNumber = CHESS_N_ROWS;
	int player;
	unsigned int maxDepth = DIFFICULTY_LEVEL_2_INT;
	char gameMode = ONE_PLAYER;
	int userColor = CHESS_WHITE_PLAYER;
	int count = 0;
	int place;
	while (fgets(line, sizeof(line), file) != NULL ) {
		switch (count) {
		case 0:
			player = loadPlayer(line);
			game->currentPlayer = player;
			count++;
			break;
		case 1:
			count++;
			break;
		case 2:
			gameMode = loadGameMode(line);
			settings->gameMode = gameMode;
			count++;
			if (gameMode == TWO_PLAYERS)
				count = 5;
			break;
		case 3:
			maxDepth = loadDifficulty(line);
			settings->maxDepth = maxDepth;
			count++;
			break;
		case 4:
			userColor = loadUserColor(line);
			settings->userColor = userColor;
			count++;
			break;
		case 5:
			place = 0;
			if (lineNumber > 0) {
				lineNumber--;
				for (int pos = 3; pos < 18; pos = pos + 2) {
					checkKingPosition(game, line[pos], place, lineNumber);
					game->gameBoard.position[lineNumber][place] =
							chessGameCharToChessPieceConverter(line[pos]);
					place++;
				}
			} else
				return;
		}
	}
	if (ferror(file)) {
		hadFileFailure();
		return;
	}
	chessGameUpdateIsCheck(game);
}

/**
 *	Creates settings from a given file.
 *
 *	@param fileName - the route of the needed file
 *	@return
 *	GAME_SETTINGS_LOAD_FILE_FAIL    - if either the file can't be opened or it doesn't exist or a memory allocation failure occurred.
 *	GAME_SETTINGS_LOAD_FILE_SUCCESS - Otherwise
 *
 */
GAME_SETTINGS_MESSAGE gameSettingsLoad(GameSettings* settings, char* fileName) {
	if (settings == NULL || fileName == NULL )
		return GAME_SETTINGS_LOAD_FILE_FAIL;
	FILE* file = fopen(fileName, "r");
	if (file == NULL ) {
		return GAME_SETTINGS_LOAD_FILE_OPEN_FAIL;
	}
	loadSettings(settings, file);
	fclose(file);
	return getHadFileFailure() ?
			GAME_SETTINGS_LOAD_FILE_FAIL : GAME_SETTINGS_LOAD_FILE_SUCCESS;
}

