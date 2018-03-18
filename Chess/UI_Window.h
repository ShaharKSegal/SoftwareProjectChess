#ifndef WINDOW_H_
#define WINDOW_H_
#include "UI_Widget.h"

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

Window* windowBaseCreate();

Window* windowCreate(SDL_Window* window, SDL_Renderer* renderer,
		SDL_Texture* bgTexture, Widget** widgets, int numOfWidgets, void* data,
		void (*drawWindow)(Window*), UI_EVENT (*handleEvent)(Window*, SDL_Event*),
		void (*destroyWindow)(Window*));

void windowBaseDraw(Window* window);

void windowDraw (Window* window);

UI_EVENT windowHandleEvent(Window* window, SDL_Event* event);

void windowBaseDestroy(Window* window);

void windowDestroy(Window* window);

#endif
