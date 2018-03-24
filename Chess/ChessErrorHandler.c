#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "ChessErrorHandler.h"

const static char* SDL_ERROR_MSG =
		"ERROR: encountered a problem in sdl lib - %s";
const static char* MEMORY_ERROR_MSG = "ERROR: dynamic memory allocation failed";

static bool memFailure = false;
static bool sdlError = false;

/**
 * Report memory failure to the error handler.
 */
void hadMemoryFailure() {
	memFailure = true;
}

/**
 * Report SDL error to the error handler.
 */
void hadSDLError() {
	sdlError = true;
}

/**
 * Print critical error messages to console. Prioritizes memory failures.
 */
void printCriticalError() {
	if (memFailure)
		printf(MEMORY_ERROR_MSG);
	else if (sdlError)
		printf(SDL_ERROR_MSG, SDL_GetError());
}

/**
 * Get the current state of critical errors (did we have any?).
 *
 * @return
 * bool - true if we had a critical error, false if not.
 */
bool getHadCriticalError() {
	return memFailure || sdlError;
}

/**
 * Get the current state of memory failures (did we have any?).
 *
 * @return
 * bool - true if we had memory failure, false if not.
 */
bool getHadMemoryFailure() {
	return memFailure;
}

/**
 * Get the current state of SDL error (did we have any?).
 *
 * @return
 * bool - true if we had SDL error, false if not.
 */
bool getHadSDLError() {
	return sdlError;
}
