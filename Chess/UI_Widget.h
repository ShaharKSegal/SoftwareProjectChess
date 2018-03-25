#ifndef WIDGET_H_
#define WIDGET_H_
#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>

/**
 * Enum for any UI event (in the view).
 * Including UI_EVENT_ERROR if an error occurred during a call.
 * UI_EVENT_NONE to represent that nothing happened.
 */
typedef enum {
	UI_EVENT_INVALID_ARGUMENT,
	UI_EVENT_NONE,
	UI_EVENT_ERROR,
	UI_BUTTON_EVENT_EXIT,
	UI_BUTTON_EVENT_NEW_GAME,
	UI_BUTTON_EVENT_ACTIVATED_SLOT,
	UI_BUTTON_EVENT_ONE_PLAYER_MODE,
	UI_BUTTON_EVENT_TWO_PLAYER_MODE,
	UI_BUTTON_EVENT_AMATEUR_MODE,
	UI_BUTTON_EVENT_EASY_MODE,
	UI_BUTTON_EVENT_MODERATE_MODE,
	UI_BUTTON_EVENT_HARD_MODE,
	UI_BUTTON_EVENT_EXPERT_MODE,
	UI_BUTTON_EVENT_WHITE_COLOR_MODE,
	UI_BUTTON_EVENT_BLACK_COLOR_MODE,
	UI_BUTTON_EVENT_BACK,
	UI_BUTTON_EVENT_START_GAME,
	UI_BUTTON_EVENT_LOAD,
	UI_BUTTON_EVENT_NEXT_PAGE,
	UI_BUTTON_EVENT_PREVIOUS_PAGE,
	UI_BUTTON_EVENT_CHESS_PIECE_CLICK,
	UI_BUTTON_EVENT_PIECE_DRAG,
	UI_BUTTON_EVENT_GET_MOVES,
	UI_BUTTON_EVENT_RESTART,
	UI_BUTTON_EVENT_SAVE,
	UI_BUTTON_EVENT_UNDO,
	UI_BUTTON_EVENT_MAIN_MENU_SCREEN,
	UI_MSGBOX_EVENT_YES,
	UI_MSGBOX_EVENT_NO,
	UI_MSGBOX_EVENT_CANCEL
} UI_EVENT;

/**
 * A base structure to represent any item placed on the window.
 * Includes relevant basic operations (draw/destroy/handle events) and data.
 * data contains the actual widget information (usually this is a button).
 */
typedef struct widget_t Widget;
struct widget_t {
	void (*drawWidget)(Widget*);
	UI_EVENT (*handleEvent)(Widget*, SDL_Event*);
	void (*destroyWidget)(Widget*);
	void* data;
};

/**
 * A common widget base creation for all widgets.
 * Can have memmory allocations errors.
 * @returns
 * If any argument is NULL, returns NULL.
 * Otherwise a Widget with the relevant fields filled.
 */
Widget* widgetCreate(void* data, void (*drawWidget)(Widget*),
		UI_EVENT (*handleEvent)(Widget*, SDL_Event*),
		void (*destroyWidget)(Widget*));

/**
 * A generic destroy function. Calls widget.destroyWidget is not NULL.
 */
void widgetDestroy(Widget* widget);

/**
 * A generic destroy function for a list of widgets.
 * Assumes there are numOfWigits widgets.
 * Calls widget.destroyWidget for each widget and then frees widgets.
 */
void widgetListDestory(Widget** widgets, int numOfWigits);

#endif
