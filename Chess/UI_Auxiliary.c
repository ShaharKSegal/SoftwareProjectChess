#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_Button.h"
#include "ChessErrorHandler.h"

const static char* WARNING_TITLE = "Warning";
const static char* UNSAVED_MESSAGE =
		"Unsaved changes detected, would you like to save the game?";
const static SDL_MessageBoxButtonData BUTTONS[] = { { 0, UI_MSGBOX_EVENT_NO,
		"no" }, { 0, UI_MSGBOX_EVENT_YES, "yes" }, { 0, UI_MSGBOX_EVENT_CANCEL,
		"cancel" }, };
const static SDL_MessageBoxColorScheme COLOR_SCHEME = { { { 255, 0, 0 }, { 0,
		255, 0 }, { 255, 255, 0 }, { 0, 0, 255 }, { 255, 0, 255 } } };

/**
 * Adds a new button to the widgets array in the given index.
 * Handles failures and frees memory in the widget array.
 * NOTE: assumes the given index is the end of the actual size of the array.
 */
bool createButtonInWidgetArray(Widget** widgets, int index,
		SDL_Renderer* renderer, SDL_Rect location, const char* activeImage,
		const char* inactiveImage, UI_EVENT eventActive, UI_EVENT eventInactive,
		bool isActive) {
	widgets[index] = buttonCreate(renderer, location, activeImage,
			inactiveImage, eventActive, eventInactive, isActive);
	if (widgets[index] == NULL ) {
		widgetListDestory(widgets, index);
		return false;
	}
	return true;
}

/**
 * Popup message box for unsaved changes, asking the user approval.
 * @return:
 * UI_MSGBOX_EVENT_YES - The controller will change to the save screen.
 * UI_MSGBOX_EVENT_NO - Ignore unsaved changes and continue.
 * UI_MSGBOX_EVENT_CANCEL - Go back to the window as if nothing happened.
 */
UI_EVENT unsavedChangesPopup() {
	const SDL_MessageBoxData messageboxdata = {
			SDL_MESSAGEBOX_WARNING,
			NULL,
			WARNING_TITLE,
			UNSAVED_MESSAGE,
			SDL_arraysize(BUTTONS),
			BUTTONS,
			&COLOR_SCHEME
	};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		printSDLError();
		return UI_EVENT_ERROR;
	}
	if (buttonid == -1)
		return UI_EVENT_NONE;
	return (UI_EVENT) buttonid;
}
