#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "ChessErrorHandler.h"

static bool memFailure = false;
static bool sdlError = false;

void hadMemoryFailure() {
	memFailure = true;
}

void printSDLError() {
	printf("ERROR: encountered a problem in sdl lib - %s", SDL_GetError());
	sdlError = true;
	return;
}

bool getHadMemoryFailure() {
	return memFailure;
}

bool getHadSDLError() {
	return sdlError;
}
