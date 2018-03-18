#ifndef UIWINDOWCONTROLLER_H_
#define UIWINDOWCONTROLLER_H_
#include <SDL.h>
#include "ChessGame.h"
#include "UI_Window.h"

typedef enum {
	UI_MAIN_MODE, UI_SETTINGS_MODE, UI_LOAD_MODE, UI_GAME_MODE
} UI_MODE;

typedef enum {
	UI_CONTROLLER_EVENT_INVOKE_DRAW,
	UI_CONTROLLER_EVENT_ERROR,
	UI_CONTROLLER_EVENT_QUTT,
	UI_CONTROLLER_EVENT_NONE
} UI_CONTROLLER_EVENT;

typedef struct window_controller_t WindowController;

struct window_controller_t {
	Window* window;
	void* data;
	UI_CONTROLLER_EVENT (*handleEventController)(WindowController**, SDL_Event*);
	void (*destroyWindowController)(WindowController*);
};

WindowController* windowControllerCreate(Window* window, void* data,
		UI_CONTROLLER_EVENT (*handleEventController)(WindowController**,
				SDL_Event*), void (*destroyWindowController)(WindowController*));

UI_CONTROLLER_EVENT windowControllerHandleEvent(WindowController** controller,
		SDL_Event* event);

void windowControllerBaseDestroy(WindowController* controller);

void windowControllerDestroy(WindowController* controller);

#endif
