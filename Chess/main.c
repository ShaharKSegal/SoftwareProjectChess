#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Bufferset.h"
#include "UI_Window.h"
#include "UI_Auxiliary.h"
#include "UI_WindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"
#include "MainAux.h"
#include "GameSettings.h"

#define CHESS_FLAG_MAIN_CONSOLE "-c"
#define CHESS_FLAG_MAIN_GUI "-g"

static int guiMain() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	WindowController* controller = mainWindowControllerCreate();
	if (controller == NULL ) {
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
	bool isSettings = true;
	printf(STARTING_PROGRAM_LINE);
	GameSettings* settings = gameSettingsCreate();
	printf(SETTINGS_STATE_LINE);
	int quitGame = false;

	if (settings == NULL || getHadMemoryFailure()) {
		printCriticalError();
		return 0;
	}
	CmdCommand* command = mainAuxGetUserCommand(isSettings);
	if (command == NULL ) {
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
				printf("Enter your move (%s player):\n",
						mainAuxWhichPlayer(settings));
			}
			command = mainAuxGetUserCommand(isSettings);
		}
	}
	if (getHadMemoryFailure()) {
		printCriticalError();
		gameSettingsDestroy(settings);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
	BUFF_SET();
	int res;
	if (argc > 2) {
		printf("ERROR: Too many arguments!\n");
		res = EXIT_FAILURE;
	}
	else if (argc == 2) {
		if (!strcmp(argv[1], CHESS_FLAG_MAIN_GUI))
			res = guiMain();
		else if (!strcmp(argv[1], CHESS_FLAG_MAIN_CONSOLE))
			res = consoleMain();
		else {
			printf("ERROR: First argument must be %s or %s",
					CHESS_FLAG_MAIN_GUI, CHESS_FLAG_MAIN_CONSOLE);
			res = EXIT_FAILURE;
		}
	}
	else
		res = consoleMain();
	return res;
}

