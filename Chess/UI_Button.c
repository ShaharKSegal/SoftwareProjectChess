#include <stdlib.h>
#include <stdio.h>
#include "UI_Button.h"
#include "ChessErrorHandler.h"

const static int R_MAP_CONST = 34;
const static int G_MAP_CONST = 177;
const static int B_MAP_CONST = 76;

/**
 * A destroy function for buttons.
 */
static void buttonDestroy(Widget* widget) {
	if (widget == NULL )
		return;
	Button* button = (Button*) widget->data;
	SDL_DestroyTexture(button->buttonActiveTexture);
	SDL_DestroyTexture(button->buttonInactiveTexture);
	free(button);
	free(widget);
}

/**
 * A handle event function for buttons. Ignores SDL_Events which are not
 * SDL_MOUSEBUTTONDOWN or the mouse click is not on the button location.
 * @returns
 * button->eventActive or button->eventInactive the button was clicked (based
 * on isActive field). Otherwise UI_EVENT_NONE.
 */
static UI_EVENT buttonHandleEvent(Widget* widget, SDL_Event* event) {
	if (widget == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	Button* button = (Button*) widget->data;
	if (event->type == SDL_MOUSEBUTTONDOWN) {
		SDL_Point point = { .x = event->button.x, .y = event->button.y };
		if (SDL_PointInRect(&point, &button->location))
			return button->isActive ?
					button->eventActive : button->eventInactive;
	}
	return UI_EVENT_NONE;
}

/**
 * A draw function for buttons. Used the texture based on isActive field.
 */
static void buttonDraw(Widget* widget) {
	if (widget == NULL )
		return;
	Button* button = widget->data;
	int res;
	if (button->isActive) {
		res = SDL_RenderCopy(button->renderer, button->buttonActiveTexture,
				NULL, &button->location);
		if (res == -1) {
			hadSDLError();
			return;
		}
	} else {
		res = SDL_RenderCopy(button->renderer, button->buttonInactiveTexture,
				NULL, &button->location);
		if (res == -1) {
			hadSDLError();
			return;
		}
	}
}

/**
 * Creates the button texture based on the renderer and image.
 * Used to avoid code duplication (called twice, for active and inactive textures).
 */
static SDL_Texture* createButtonTexture(SDL_Renderer* renderer,
		const char* image) {
	SDL_Surface* surface = SDL_LoadBMP(image);
	if (surface == NULL ) {
		hadSDLError();
		return NULL ;
	}
	SDL_SetColorKey(surface, SDL_TRUE,
			SDL_MapRGB(surface->format, R_MAP_CONST, G_MAP_CONST, B_MAP_CONST));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == NULL ) {
		hadSDLError();
		return NULL ;
	}
	return texture;
}

/**
 * A button creation function. uses the renderer and images to create SDL_Textures
 * in the given SDL_Rect.
 * Can have memory allocations or SDL errors.
 * NOTE: Calls widgetCreate.
 * @returns
 * If any argument is NULL/invalid returns NULL. Otherwise a widget of a button.
 */
Widget* buttonCreate(SDL_Renderer* renderer, SDL_Rect location,
		const char* activeImage, const char* inactiveImage,
		UI_EVENT eventActive, UI_EVENT eventInactive, bool isActive) {
	if (renderer == NULL || activeImage == NULL || inactiveImage == NULL ) {
		return NULL ;
	}

	Button* button = (Button*) malloc(sizeof(Button));
	if (button == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	Widget* widget = widgetCreate(button, buttonDraw, buttonHandleEvent,
			buttonDestroy);
	if (widget == NULL ) {
		free(button);
		return NULL ;
	}

	button->renderer = renderer;
	button->eventActive = eventActive;
	button->eventInactive = eventInactive;
	button->location = location;
	button->isActive = isActive;
	button->buttonActiveTexture = createButtonTexture(renderer, activeImage);
	button->buttonInactiveTexture = createButtonTexture(renderer,
			inactiveImage);
	if (button->buttonActiveTexture == NULL
			|| button->buttonInactiveTexture == NULL ) {
		buttonDestroy(widget);
		return NULL ;
	}
	return widget;
}

/**
 * A setter for isActive field in button (to be used post creation).
 */
void buttonSetActive(Button* button, bool isActive) {
	button->isActive = isActive;
}

/**
 * A setter for location field in button (to be used post creation).
 */
void buttonSetLocation(Button* button, SDL_Rect rect) {
	button->location = rect;
}
