#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_Window.h"
#include "ChessErrorHandler.h"

static const int RBGA_CONST = 255;

/**
 * A common base draw function.
 * Can have SDL errors.
 */
static void windowBaseDraw(Window* window) {
	if (window == NULL )
		return;
	SDL_Renderer* renderer = window->renderer;
	//Draw window
	SDL_Rect rec = { .x = 0, .y = 0, .w = UI_WINDOW_W, .h = UI_WINDOW_H };
	if (SDL_SetRenderDrawColor(renderer, RBGA_CONST, RBGA_CONST, RBGA_CONST,
			RBGA_CONST) == -1) {
		hadSDLError();
		return;
	}
	if (SDL_RenderClear(renderer) == -1) {
		hadSDLError();
		return;
	}
	if (SDL_RenderCopy(renderer, window->bgTexture, NULL, &rec) == -1) {
		hadSDLError();
		return;
	}
	for (int i = 0; i < window->numOfWidgets; i++) {
		if (window->widgets[i] == NULL )
			continue;
		(window->widgets[i])->drawWidget(window->widgets[i]);
		if (getHadSDLError())
			return;
	}
	SDL_RenderPresent(renderer);
}

/**
 * A common window base creation for all windows.
 * Can have memory allocations or SDL errors.
 * NOTE: widgets, data and functions pointers are NULLIFIED and should be filled
 * by the specific window.
 * @returns
 * Window type to be used.
 */
Window* windowBaseCreate(char* image) {
	Window* window = (Window*) malloc(sizeof(Window));
	if (window == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	SDL_Window* sdlWindow = SDL_CreateWindow(UI_WINDOW_TITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, UI_WINDOW_W,
			UI_WINDOW_H, SDL_WINDOW_SHOWN);
	if (sdlWindow == NULL ) {
		hadSDLError();
		return NULL ;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (renderer == NULL ) {
		SDL_DestroyWindow(sdlWindow);
		hadSDLError();
		return NULL ;
	}
	//Create a background texture:
	SDL_Surface* surface = SDL_LoadBMP(image);
	if (surface == NULL ) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(sdlWindow);
		hadSDLError();
		return NULL ;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == NULL ) {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		hadSDLError();
		return NULL ;
	}

	window->sdlWindow = sdlWindow;
	window->renderer = renderer;
	window->bgTexture = texture;
	window->numOfWidgets = 0;
	window->widgets = NULL;
	window->handleEvent = NULL;
	window->drawWindow = NULL;
	window->destroyWindow = NULL;
	window->data = NULL;
	return window;
}

/**
 * A generic draw window function. Uses base draw if window.drawWindow is NULL.
 * Can have SDL errors.
 */
void windowDraw(Window* window) {
	if (window == NULL )
		return;
	if (window->drawWindow == NULL )
		windowBaseDraw(window);
	else
		window->drawWindow(window);

}

/**
 * A generic handle event function based on the SDL_Event.
 * NOTE: there is no base handleEvent, so window.handleEvent must not be NULL.
 */
UI_EVENT windowHandleEvent(Window* window, SDL_Event* event) {
	if (window == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	return window->handleEvent(window, event);
}

/**
 * A common base destroy function.
 */
void windowBaseDestroy(Window* window) {
	if (window == NULL )
		return;
	widgetListDestory(window->widgets, window->numOfWidgets);
	SDL_DestroyTexture(window->bgTexture);
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->sdlWindow);
	free(window->data);
	free(window);
}

/**
 * A generic destroy function. Calls windowBaseDestroy if window.destroyWindow is NULL.
 */
void windowDestroy(Window* window) {
	if (window == NULL )
		return;
	if (window->destroyWindow != NULL )
		window->destroyWindow(window);
	else
		windowBaseDestroy(window);
}
