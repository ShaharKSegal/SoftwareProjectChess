#ifndef WINDOW_H_
#define WINDOW_H_
#include "UI_Widget.h"

/**
 * A structure to represent a window in the UI.
 * Includes relevant basic operations (draw/destroy/handle events) and widgets.
 */
typedef struct window_t Window;
struct window_t {
	SDL_Window* sdlWindow;
	SDL_Renderer* renderer;
	SDL_Texture* bgTexture;
	Widget** widgets;
	int numOfWidgets;
	void* data;
	void (*drawWindow)(Window*);
	UI_EVENT (*handleEvent)(Window*, SDL_Event*);
	void (*destroyWindow)(Window*);
};

/**
 * A common window base creation for all windows.
 * Can have memmory allocations or SDL errors.
 * NOTE: widgets, data and functions pointers are NULLIFIED and should be filled
 * by the specific window.
 * @returns
 * Window type to be used.
 */
Window* windowBaseCreate();

/**
 * A generic draw window function. Uses base draw if window.drawWindow is NULL.
 * Can have SDL errors.
 */
void windowDraw (Window* window);

/**
 * A generic handle event function based on the SDL_Event.
 * NOTE: there is no base handleEvent, so window.handleEvent must not be NULL.
 */
UI_EVENT windowHandleEvent(Window* window, SDL_Event* event);

/**
 * A common base destroy function.
 */
void windowBaseDestroy(Window* window);

/**
 * A generic destroy function. Calls windowBaseDestroy if window.destroyWindow is NULL.
 */
void windowDestroy(Window* window);

#endif
