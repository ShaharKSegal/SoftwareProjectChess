#ifndef UIMAINWINDOWCONTROLLER_H_
#define UIMAINWINDOWCONTROLLER_H_
#include <SDL.h>
#include "ChessGame.h"
#include "UI_WindowController.h"

/**
 * Summary: This is an implementation of the main window controller.
 * Which creates a Window type with the relevant widgets
 * and operations regarding the main menu.
 */

/**
 * Creates a WindowController type with the relevant Window type.
 * Can have memory failures or SDL errors
 * @returns
 * NULL if some error occurred, otherwise a WindowController* of the main menu.
 */
WindowController* mainWindowControllerCreate();

#endif
