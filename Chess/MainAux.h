/*
 * MainAux.h
 *
 *  Created on: 12 במרס 2018
 *      Author: Liran
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "ChessCmdParser.h"
#include "GameSettings.h"

#define MOVE_DELI ",<>"


#define PAWN "pawn"
#define KNIGHT "knight"
#define ROOK "rook"
#define BISHOP "bishop"
#define QUEEN "queen"
#define KING "king"


#define STARTING_PROGRAM_LINE  " Chess\n-------\n"
#define SETTINGS_STATE_LINE "Specify game settings or type 'start' to begin a game with the current settings:\n"

/*
 * Receives an input from a former process function
 * and parses it to create the user's command.
 */
CmdCommand* mainAuxGetUserCommand(bool isSettings);

/*
 * Distinguishes between different game function and handles each one separately.
 * @return
 *  1  - if it's needed to end the game after the command.
 *  0  - else.
 */
int mainAuxGameState(GameSettings* settings, CmdCommand* command, bool* isSettings);

/*
 * Distinguishes between different settings function and handles each one separately.
 * @return
 * 1 - if it's needed to end the game after the command.
 * 0 - else.
 */
int mainAuxSettingsState(GameSettings* settings, CmdCommand* command, bool* isSettings);

/*
 * Returns the color of current player.
 */
char* mainAuxWhichPlayer(GameSettings* settings);

#endif /* MAINAUX_H_ */
