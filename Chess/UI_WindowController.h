#ifndef UIWINDOWCONTROLLER_H_
#define UIWINDOWCONTROLLER_H_
#include <SDL.h>
#include "ChessGame.h"
#include "UI_Window.h"

/**
 * Enum for UI_CONTROLLER. Used to represent a controller.
 * E.g. used to represent what was the previous controller.
 */
typedef enum {
	UI_MAIN_CONTROLLER,
	UI_SETTINGS_CONTROLLER,
	UI_LOAD_CONTROLLER,
	UI_SAVE_CONTROLLER,
	UI_GAME_CONTROLLER,
	UI_QUIT_CONTROLLER,
} UI_CONTROLLER;

/**
 * Enum for controller events, to be used by the UI main.
 * Tells the UI main if a error occurred, or we should redraw the window ect.
 */
typedef enum {
	UI_CONTROLLER_EVENT_INVOKE_DRAW,
	UI_CONTROLLER_EVENT_MINOR_ERROR,
	UI_CONTROLLER_EVENT_ERROR,
	UI_CONTROLLER_EVENT_QUTT,
	UI_CONTROLLER_EVENT_NONE
} UI_CONTROLLER_EVENT;

/**
 * WindowController declaration
 */
typedef struct window_controller_t WindowController;

/**
 * A structure to represent a window controller in the UI.
 * Includes model (data) and view (window) and relevant operations (destroy/handle events).
 */
struct window_controller_t {
	Window* window;
	void* data;
	UI_CONTROLLER_EVENT (*handleEventController)(WindowController**, SDL_Event*);
	void (*destroyWindowController)(WindowController*);
};

/**
 * A common window controller creation for all controllers.
 * Can have memory allocations or SDL errors.
 * by the specific window.
 * @returns
 * Window type to be used.
 */
WindowController* windowControllerCreate(Window* window, void* data,
		UI_CONTROLLER_EVENT (*handleEventController)(WindowController**,
				SDL_Event*), void (*destroyWindowController)(WindowController*));

/**
 * A generic handle event function based on the SDL_Event.
 * NOTE: there is no base handleEvent, so (*controllerPtr).handleEventController must not be NULL.
 */
UI_CONTROLLER_EVENT windowControllerHandleEvent(WindowController** controllerPtr,
		SDL_Event* event);

/**
 * A generic destroy function. Calls a base function if controller.destroyWindowController is NULL.
 */
void windowControllerDestroy(WindowController* controller);

#endif
