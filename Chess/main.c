#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "UI_Window.h"
#include "UI_Auxiliary.h"
#include "UI_WindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"

#define CHESS_FLAG_MAIN_CONSOLE "-c"
#define CHESS_FLAG_MAIN_GUI "-g"

int main(int argc, char** argv) {
	if (argc || argv) {
		//TODO: remove this, this is to make the compiler hate me less
	}
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 0;
	}
	WindowController* controller = mainWindowControllerCreate();
	if (controller == NULL ) {
		if (getHadCriticalError()) {
			printCriticalError();
		}
		SDL_Quit();
		return 0;
	}
	windowDraw(controller->window);
	if (getHadCriticalError()) {
		printCriticalError();
		SDL_Quit();
		return 0;
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
	return 0;
}

