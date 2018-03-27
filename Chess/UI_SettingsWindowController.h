
#ifndef UI_SETTINGSWINDOWCONTROLLER_H_
#define UI_SETTINGSWINDOWCONTROLLER_H_

#include "GameSettings.h"
#include "UI_WindowController.h"

/**
 * Summary: This is an implementation of the settings window controller.
 * Which creates a Window type with the relevant widgets
 * and operations regarding the settings menu.
 */

/**
 * Creates a WindowController type with the relevant Window type.
 * Adds a GameSettings type as it's data.
 * Can have memory failures or SDL errors
 * @returns
 * NULL if some error occurred, otherwise a WindowController* of the settings menu.
 */
WindowController* settingsWindowControllerCreate();

#endif /* UI_SETTINGSWINDOWCONTROLLER_H_ */
