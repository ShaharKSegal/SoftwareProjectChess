#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_Window.h"
#include "ChessErrorHandler.h"

Window* windowBaseCreate(char* image) {
	Window* window = (Window*) malloc(sizeof(Window));
	if (window == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	SDL_Window* sdlWindow = SDL_CreateWindow(UI_WINDOW_TITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, UI_WINDOW_W,
			UI_WINDOW_H, SDL_WINDOW_OPENGL);
	if (sdlWindow == NULL ) {
		printSDLError();
		return NULL ;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (renderer == NULL ) {
		SDL_DestroyWindow(sdlWindow);
		printSDLError();
		return NULL ;
	}
	//Create a background texture:
	SDL_Surface* surface = SDL_LoadBMP(image);
	if (surface == NULL ) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(sdlWindow);
		printSDLError();
		return NULL ;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == NULL ) {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		printSDLError();
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

Window* windowCreate(SDL_Window* sdlWindow, SDL_Renderer* renderer,
		SDL_Texture* bgTexture, Widget** widgets, int numOfWidgets, void* data,
		void (*drawWindow)(Window*),
		UI_EVENT (*handleEvent)(Window*, SDL_Event*),
		void (*destroyWindow)(Window*)) {
	if (sdlWindow == NULL || renderer == NULL || bgTexture == NULL
			|| widgets == NULL || drawWindow == NULL || handleEvent == NULL
			|| destroyWindow == NULL )
		return NULL ;
	Window* window = (Window*) malloc(sizeof(Window));
	if (window == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	window->sdlWindow = sdlWindow;
	window->renderer = renderer;
	window->bgTexture = bgTexture;
	window->widgets = widgets;
	window->numOfWidgets = numOfWidgets;
	window->data = data;
	window->destroyWindow = destroyWindow;
	window->drawWindow = drawWindow;
	window->handleEvent = handleEvent;

	return window;
}

void windowBaseDraw(Window* window) {
	if (window == NULL )
		return;
	SDL_Renderer* renderer = window->renderer;
	//Draw window
	SDL_Rect rec = { .x = 0, .y = 0, .w = 800, .h = 600 };
	if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) == -1) {
		printSDLError();
		return;
	}
	if (SDL_RenderClear(renderer) == -1) {
		printSDLError();
		return;
	}
	if (SDL_RenderCopy(renderer, window->bgTexture, NULL, &rec) == -1) {
		printSDLError();
		return;
	}
	for (int i = 0; i < window->numOfWidgets; i++) {
		if (window->widgets[i] == NULL)
			continue;
		(window->widgets[i])->drawWidget(window->widgets[i]);
		if (getHadSDLError())
			return;
	}
	SDL_RenderPresent(renderer);
}

void windowDraw(Window* window) {
	if (window == NULL )
		return;
	if (window->drawWindow == NULL )
		windowBaseDraw(window);
	else
		window->drawWindow(window);

}

UI_EVENT windowHandleEvent(Window* window, SDL_Event* event) {
	if (window == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	return window->handleEvent(window, event);
}

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

void windowDestroy(Window* window) {
	if (window == NULL )
		return;
	if (window->destroyWindow != NULL )
		window->destroyWindow(window);
	else
		windowBaseDestroy(window);
}
