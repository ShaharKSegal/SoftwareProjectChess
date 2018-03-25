#ifndef BUTTON_H_
#define BUTTON_H_
#include "UI_Widget.h"
#include <stdbool.h>

/**
 * A structure for button. Which is a widget.
 * Aside from basic SDL pointers it has 2 mode: active or inactive.
 * Each mode calls a different UI_EVENT when the button is preset.
 */
typedef struct button_t {
	SDL_Renderer* renderer;
	SDL_Texture* buttonActiveTexture;
	SDL_Texture* buttonInactiveTexture;
	SDL_Rect location;
	bool isActive;
	UI_EVENT eventActive;
	UI_EVENT eventInactive;
} Button;

/**
 * A button creation function. uses the renderer and images to create SDL_Textures
 * in the given SDL_Rect.
 * Can have memmory allocations or SDL errors.
 * @returns
 * If any argument is NULL/invalid returns NULL. Otherwise a Button type.
 */
Widget* buttonCreate(SDL_Renderer* renderer, SDL_Rect location,
		const char* activeImage, const char* inactiveImage,
		UI_EVENT eventActive, UI_EVENT eventInactive,
		bool isActive);

/**
 * A setter for isActive field in button (to be used post creation).
 */
void buttonSetActive(Button* button, bool isActive);

/**
 * A setter for location field in button (to be used post creation).
 */
void buttonSetLocation(Button* button, SDL_Rect rect);

#endif
