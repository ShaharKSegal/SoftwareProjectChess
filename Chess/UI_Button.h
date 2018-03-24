#ifndef BUTTON_H_
#define BUTTON_H_
#include "UI_Widget.h"
#include <stdbool.h>

typedef struct button_t {
	SDL_Renderer* renderer;
	SDL_Texture* buttonActiveTexture;
	SDL_Texture* buttonInactiveTexture;
	SDL_Rect location;
	bool isActive;
	UI_EVENT eventActive;
	UI_EVENT eventInactive;
} Button;

typedef struct event_result_t {
	int message;
	int srcX;
	int srcY;
	int destX;
	int destY;
} EVENT_RESULT;

Widget* buttonCreate(SDL_Renderer* renderer, SDL_Rect location,
		const char* activeImage, const char* inactiveImage,
		UI_EVENT eventActive, UI_EVENT eventInactive,
		bool isActive);

void buttonSetActive(Button* button, bool isActive);

void buttonSetLocation(Button* button, SDL_Rect rect);

#endif
