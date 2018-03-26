#include <stdlib.h>
#include "UI_Auxiliary.h"
#include "UI_SettingsWindow.h"
#include "UI_Button.h"
#include "ChessErrorHandler.h"

#define ONE_PLAYER_X 150
#define ONE_PLAYER_Y 30
#define ONE_PLAYER_H 100
#define ONE_PLAYER_W 200
#define TWO_PLAYERS_X 450
#define TWO_PLAYERS_Y 30
#define TWO_PLAYERS_H 100
#define TWO_PLAYERS_W 200

#define WHITE_COLOR_X 150
#define WHITE_COLOR_Y 200
#define WHITE_COLOR_H 100
#define WHITE_COLOR_W 200
#define BLACK_COLOR_X 450
#define BLACK_COLOR_Y 200
#define BLACK_COLOR_H 100
#define BLACK_COLOR_W 200

#define DIFFICULTY_X 160
#define DIFFICULTY_Y 350
#define DIFFICULTY_H 75
#define DIFFICULTY_W 100
#define DIFFICULTY_X_DELTA 100

#define START_X 150
#define START_Y 450
#define START_H 100
#define START_W 200
#define BACK_X 450
#define BACK_Y 450
#define BACK_H 100
#define BACK_W 200

static const int ONE_PLAYER_WIDGET_ID = 0;
static const int TWO_PLAYER_WIDGET_ID = 1;
static const int WHITE_COLOR_WIDGET_ID = 2;
static const int BLACK_COLOR_WIDGET_ID = 3;
static const int DIFFICULTY_AMATEUR_WIDGET_ID = 4;
static const int DIFFICULTY_EASY_WIDGET_ID = 5;
static const int DIFFICULTY_MODERATE_WIDGET_ID = 6;
static const int DIFFICULTY_HARD_WIDGET_ID = 7;
static const int DIFFICULTY_EXPERT_WIDGET_ID = 8;
static const int START_WIDGET_ID = 9;
static const int WIDGET_NUM = 11;

static bool addGameModeWidgets(SDL_Renderer* renderer, Widget** widgets) {
	SDL_Rect onePlayerR = { .x = ONE_PLAYER_X, .y = ONE_PLAYER_Y, .h =
			ONE_PLAYER_H, .w = ONE_PLAYER_W };
	SDL_Rect twoPlayersdR = { .x = TWO_PLAYERS_X, .y = TWO_PLAYERS_Y, .h =
			TWO_PLAYERS_H, .w = TWO_PLAYERS_W };
	if (!createButtonInWidgetArray(widgets, ONE_PLAYER_WIDGET_ID, renderer,
			onePlayerR, UI_PIC_ONE_PLAYER_ACTIVE, UI_PIC_ONE_PLAYER_INACTIVE,
			UI_EVENT_NONE, UI_BUTTON_EVENT_ONE_PLAYER_MODE, true))
		return false;
	if (!createButtonInWidgetArray(widgets, TWO_PLAYER_WIDGET_ID, renderer,
			twoPlayersdR, UI_PIC_TWO_PLAYER_ACTIVE, UI_PIC_TWO_PLAYER_INACTIVE,
			UI_EVENT_NONE, UI_BUTTON_EVENT_TWO_PLAYER_MODE, false))
		return false;
	return true;
}

static bool addUserColorWidgets(SDL_Renderer* renderer, Widget** widgets) {
	SDL_Rect whiteR = { .x = WHITE_COLOR_X, .y = WHITE_COLOR_Y, .h =
			WHITE_COLOR_H, .w = WHITE_COLOR_W };
	SDL_Rect blackR = { .x = BLACK_COLOR_X, .y = BLACK_COLOR_Y, .h =
			BLACK_COLOR_H, .w = BLACK_COLOR_W };
	if (!createButtonInWidgetArray(widgets, WHITE_COLOR_WIDGET_ID, renderer,
			whiteR, UI_PIC_WHITE_COLOR_ACTIVE, UI_PIC_WHITE_COLOR_INACTIVE,
			UI_EVENT_NONE, UI_BUTTON_EVENT_WHITE_COLOR_MODE, true))
		return false;
	if (!createButtonInWidgetArray(widgets, BLACK_COLOR_WIDGET_ID, renderer,
			blackR, UI_PIC_BLACK_COLOR_ACTIVE, UI_PIC_BLACK_COLOR_INACTIVE,
			UI_EVENT_NONE, UI_BUTTON_EVENT_BLACK_COLOR_MODE, false))
		return false;
	return true;
}

static bool addDifficultyWidgets(SDL_Renderer* renderer, Widget** widgets) {
	SDL_Rect diffR = { .x = DIFFICULTY_X, .y = DIFFICULTY_Y, .h = DIFFICULTY_H,
			.w = DIFFICULTY_W };
	if (!createButtonInWidgetArray(widgets, DIFFICULTY_AMATEUR_WIDGET_ID,
			renderer, diffR, UI_PIC_DIFFICULTY_AMATEUR_ACTIVE,
			UI_PIC_DIFFICULTY_AMATEUR_INACTIVE, UI_EVENT_NONE,
			UI_BUTTON_EVENT_AMATEUR_MODE, false))
		return false;
	diffR.x += DIFFICULTY_X_DELTA;
	if (!createButtonInWidgetArray(widgets, DIFFICULTY_EASY_WIDGET_ID, renderer,
			diffR, UI_PIC_DIFFICULTY_EASY_ACTIVE,
			UI_PIC_DIFFICULTY_EASY_INACTIVE, UI_EVENT_NONE,
			UI_BUTTON_EVENT_EASY_MODE, true))
		return false;
	diffR.x += DIFFICULTY_X_DELTA;
	if (!createButtonInWidgetArray(widgets, DIFFICULTY_MODERATE_WIDGET_ID,
			renderer, diffR, UI_PIC_DIFFICULTY_MODERATE_ACTIVE,
			UI_PIC_DIFFICULTY_MODERATE_INACTIVE, UI_EVENT_NONE,
			UI_BUTTON_EVENT_MODERATE_MODE, false))
		return false;
	diffR.x += DIFFICULTY_X_DELTA;
	if (!createButtonInWidgetArray(widgets, DIFFICULTY_HARD_WIDGET_ID, renderer,
			diffR, UI_PIC_DIFFICULTY_HARD_ACTIVE,
			UI_PIC_DIFFICULTY_HARD_INACTIVE, UI_EVENT_NONE,
			UI_BUTTON_EVENT_HARD_MODE, false))
		return false;
	diffR.x += DIFFICULTY_X_DELTA;
	if (!createButtonInWidgetArray(widgets, DIFFICULTY_EXPERT_WIDGET_ID,
			renderer, diffR, UI_PIC_DIFFICULTY_EXPERT_ACTIVE,
			UI_PIC_DIFFICULTY_EXPERT_INACTIVE, UI_EVENT_NONE,
			UI_BUTTON_EVENT_EXPERT_MODE, false))
		return false;
	return true;
}

static Widget** createSettingsWindowWidgets(SDL_Renderer* renderer) {
	Widget** widgets = (Widget**) calloc(WIDGET_NUM, sizeof(Widget*));
	if (widgets == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}

	SDL_Rect startR = { .x = START_X, .y = START_Y, .h = START_H, .w = START_W };
	SDL_Rect backR = { .x = BACK_X, .y = BACK_Y, .h = BACK_H, .w = BACK_W };
	if (!addGameModeWidgets(renderer, widgets)
			|| !addUserColorWidgets(renderer, widgets)
			|| !addDifficultyWidgets(renderer, widgets))
		return NULL ;
	if (!createButtonInWidgetArray(widgets, START_WIDGET_ID, renderer, startR,
			UI_PIC_START_ACTIVE, UI_PIC_START_INACTIVE,
			UI_BUTTON_EVENT_START_GAME, UI_EVENT_NONE, true))
		return false;
	if (!createButtonInWidgetArray(widgets, START_WIDGET_ID + 1, renderer,
			backR, UI_PIC_BACK, UI_PIC_BACK, UI_BUTTON_EVENT_BACK,
			UI_EVENT_NONE, true))
		return false;
	return widgets;
}

static void setUserColor(Window* window, bool isUserWhite) {
	bool isOnePlayerMode = *(bool*) window->data;
	Button* whiteColorButton =
			(Button*) window->widgets[WHITE_COLOR_WIDGET_ID]->data;
	Button* blackColorButton =
			(Button*) window->widgets[BLACK_COLOR_WIDGET_ID]->data;
	buttonSetActive(whiteColorButton, isOnePlayerMode && isUserWhite);
	buttonSetActive(blackColorButton, isOnePlayerMode && !isUserWhite);
}

static void setDifficulty(Window* window, CHESS_DIFFICULTY difficulty) {
	bool isOnePlayerMode = *(bool*) window->data;
	for (int i = 1; i <= CHESS_DIFFICULTY_EXPERT; i++) {
		Button* difficultyButton =
				(Button*) window->widgets[DIFFICULTY_AMATEUR_WIDGET_ID + i - 1]->data;
		buttonSetActive(difficultyButton,
				isOnePlayerMode && ((int) difficulty == i));
	}
}

static void setGameMode(Window* window, bool isOnePlayerMode) {
	Button* onePlayerButton =
			(Button*) window->widgets[ONE_PLAYER_WIDGET_ID]->data;
	Button* twoPlayerButton =
			(Button*) window->widgets[TWO_PLAYER_WIDGET_ID]->data;
	buttonSetActive(onePlayerButton, isOnePlayerMode);
	buttonSetActive(twoPlayerButton, !isOnePlayerMode);
	*(bool*) window->data = isOnePlayerMode;
	setUserColor(window, true);
	setDifficulty(window, CHESS_DIFFICULTY_EASY);
}

static UI_EVENT settingsWindowHandleEvent(Window* window, SDL_Event* event) {
	if (window == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	for (int i = 0; i < window->numOfWidgets; i++) {
		UI_EVENT eventResult = window->widgets[i]->handleEvent(
				window->widgets[i], event);
		switch (eventResult) {
		case UI_BUTTON_EVENT_ONE_PLAYER_MODE:
			setGameMode(window, true);
			break;
		case UI_BUTTON_EVENT_TWO_PLAYER_MODE:
			setGameMode(window, false);
			break;
		case UI_BUTTON_EVENT_WHITE_COLOR_MODE:
			setUserColor(window, true);
			break;
		case UI_BUTTON_EVENT_BLACK_COLOR_MODE:
			setUserColor(window, false);
			break;
		case UI_BUTTON_EVENT_AMATEUR_MODE:
			setDifficulty(window, CHESS_DIFFICULTY_AMATEUR);
			break;
		case UI_BUTTON_EVENT_EASY_MODE:
			setDifficulty(window, CHESS_DIFFICULTY_EASY);
			break;
		case UI_BUTTON_EVENT_MODERATE_MODE:
			setDifficulty(window, CHESS_DIFFICULTY_MODERATE);
			break;
		case UI_BUTTON_EVENT_HARD_MODE:
			setDifficulty(window, CHESS_DIFFICULTY_HARD);
			break;
		case UI_BUTTON_EVENT_EXPERT_MODE:
			setDifficulty(window, CHESS_DIFFICULTY_EXPERT);
			break;
		case UI_EVENT_NONE:
			continue;
		default:
			break;
		}
		return eventResult;
	}
	return UI_EVENT_NONE;
}

Window* settingsWindowCreate() {
	bool* isOnePlayerMode = malloc(sizeof(bool));
	*isOnePlayerMode = true;
	Window* window = windowBaseCreate(UI_PIC_SETTINGS_MENU);
	if (window == NULL )
		return NULL ;
	window->numOfWidgets = WIDGET_NUM;
	window->handleEvent = settingsWindowHandleEvent;
	window->data = isOnePlayerMode;
	window->widgets = createSettingsWindowWidgets(window->renderer);
	if (window->widgets == NULL ) {
		windowDestroy(window);
		return NULL ;
	}
	return window;
}
