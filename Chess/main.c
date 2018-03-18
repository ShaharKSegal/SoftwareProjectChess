#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "UI_Window.h"
#include "UI_WindowController.h"
#include "UI_MainWindowController.h"
#include "ChessErrorHandler.h"

#define CHESS_FLAG_MAIN_CONSOLE "-c"
#define CHESS_FLAG_MAIN_GUI "-g"

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	WindowController* controller = mainWindowControllerCreate();
	if (controller == NULL ) {
		if (getHadMemoryFailure()) {
			// TODO: report to user that an error occurred
		}
		SDL_Quit();
		return 0;
	}
	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		//handle event might invoke SDL errors or memory allocation failures
		UI_CONTROLLER_EVENT res = windowControllerHandleEvent(&controller,
				&event);
		if (res == UI_CONTROLLER_EVENT_QUTT || res == UI_CONTROLLER_EVENT_ERROR
				|| getHadSDLError() || getHadMemoryFailure()) {
			break;
		} else if (res == UI_CONTROLLER_EVENT_INVOKE_DRAW) {
			windowDraw(controller->window); //draw might invoke SDL errors
			if (getHadSDLError()) {
				break;
			}
		}
	}
	windowControllerDestroy(controller);
	SDL_Quit();
	return 0;
}

