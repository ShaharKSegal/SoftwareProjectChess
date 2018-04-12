#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "UI_Window.h"
#include "UI_Auxiliary.h"
#include "UI_WindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"
#include "MainAux.h"
#include "GameSettings.h"

/*
 * Arguments
 */
#define CHESS_FLAG_MAIN_CONSOLE "-c"
#define CHESS_FLAG_MAIN_GUI "-g"

/*
 * Printable strs
 */
#define INVALID_NUM_ARGUMENTS_ERR "ERROR: Too many arguments!\n"
#define INVALID_FIRST_ARGUMENT_ERR "ERROR: First argument must be %s or %s"
#define SDL_INIT_ERR "ERROR: unable to init SDL: %s\n"
#define ENTER_MOVE_STR "Enter your move (%s player):\n"

static int guiMain() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf(SDL_INIT_ERR, SDL_GetError());
		return EXIT_FAILURE;
	}
	WindowController* controller = mainWindowControllerCreate();
	if (controller == NULL) {
		if (getHadCriticalError()) {
			printCriticalError();
		}
		SDL_Quit();
		return EXIT_FAILURE;
	}
	windowDraw(controller->window);
	if (getHadCriticalError()) {
		printCriticalError();
		SDL_Quit();
		return EXIT_FAILURE;
	}
	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
		//handle event might invoke SDL errors or memory allocation failures
		UI_CONTROLLER_EVENT res = windowControllerHandleEvent(&controller,
				&event);
		if (res == UI_CONTROLLER_EVENT_QUTT || res == UI_CONTROLLER_EVENT_ERROR
				|| getHadCriticalError()) {
			break;
		} else if (res == UI_CONTROLLER_EVENT_INVOKE_DRAW) {
			windowDraw(controller->window); //draw might invoke SDL errors
			if (getHadSDLError()) {
				break;
			}
		}
		if (getHadFileFailure()) {
			fileFailurePopup(); // Show message to user in case of save/load failure
			unsetFileFailure(); // remove file failure flag at the end of event.
		}
	}
	if (getHadCriticalError()) {
		printCriticalError();
	}
	windowControllerDestroy(controller);
	SDL_Quit();
	return getHadCriticalError() ? EXIT_FAILURE : EXIT_SUCCESS;
}

static int consoleMain() {
	printf(STARTING_PROGRAM_LINE);
	printf(SETTINGS_STATE_LINE);

	bool isSettings = true;
	int quitGame = 0;
	GameSettings* settings = gameSettingsCreate();

	if (settings == NULL || getHadMemoryFailure()) {
		printCriticalError();
		return 0;
	}
	CmdCommand* command = mainAuxGetUserCommand(isSettings);
	if (command == NULL) {
		gameSettingsDestroy(settings);
		printCriticalError();
		return EXIT_FAILURE;
	}
	while (!quitGame && !getHadMemoryFailure()) {
		if (isSettings)
			quitGame = mainAuxSettingsState(settings, command, &isSettings);
		else {
			quitGame = mainAuxGameState(settings, command, &isSettings);
		}
		parserCmdCommandDestroy(command);
		if (!quitGame) {
			if (!isSettings) {
				printf(ENTER_MOVE_STR, mainAuxWhichPlayer(settings));
			}
			command = mainAuxGetUserCommand(isSettings);
			if (command == NULL) {
				gameSettingsDestroy(settings);
				printCriticalError();
				return EXIT_FAILURE;
			}
		}
		if (getHadFileFailure())
			unsetFileFailure(); // remove file failure flag at the end of command.
	}
	if (getHadMemoryFailure()) {
		printCriticalError();
		gameSettingsDestroy(settings);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
	int res;
	if (argc > 2) {
		printf(INVALID_NUM_ARGUMENTS_ERR);
		res = EXIT_FAILURE;
	} else if (argc == 2) {
		if (!strcmp(argv[1], CHESS_FLAG_MAIN_GUI))
			res = guiMain();
		else if (!strcmp(argv[1], CHESS_FLAG_MAIN_CONSOLE))
			res = consoleMain();
		else {
			printf(INVALID_FIRST_ARGUMENT_ERR,
			CHESS_FLAG_MAIN_GUI, CHESS_FLAG_MAIN_CONSOLE);
			res = EXIT_FAILURE;
		}
	} else
		res = consoleMain();
	return res;
}

