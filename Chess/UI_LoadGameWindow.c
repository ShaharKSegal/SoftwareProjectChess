#include <stdlib.h>
#include <stdio.h>
#include "UI_Auxiliary.h"
#include "UI_LoadGameWindow.h"
#include "UI_Button.h"
#include "ChessErrorHandler.h"

#define FIRSTGAME_X 350
#define FIRSTGAME_Y 50
#define FIRSTGAME_H 50
#define FIRSTGAME_W 150
#define GAME_DELTA 60
#define LOAD_BACK_X 250
#define LOAD_BACK_Y 500
#define LOAD_BACK_H 50
#define LOAD_BACK_W 100
#define LOAD_X 450
#define LOAD_Y 500
#define LOAD_H 50
#define LOAD_W 100
#define NEXT_X 450
#define NEXT_Y 400
#define NEXT_H 50
#define NEXT_W 100
#define PREV_X 250
#define PREV_Y 400
#define PREV_H 50
#define PREV_W 100

static const int LOAD_BTN_WIDGET_ID = 0;
static const int SAVES_IN_PAGE = 5;
static const int OTHER_BUTTONS_NUM = 4;

static void destroyPathArray(char** paths, int numOfPaths) {
	for (int i = 0; i < numOfPaths; i++) {
		if (paths[i] != NULL )
			free(paths[i]);
	}
	free(paths);
}

static void destroySlotPaths(LoadGameWindowData* data) {
	destroyPathArray(data->slotsPicPath, 2 * data->numOfPageSlots);
}

static int getNumOfPageSlots(int page, int maxNumOfSlots) {
	int startSlot = page * SAVES_IN_PAGE;
	return (startSlot + SAVES_IN_PAGE) < maxNumOfSlots ?
			SAVES_IN_PAGE : maxNumOfSlots - startSlot;
}

static char* getSlotPicPath(int slot, bool isActive) {
	char* slotPath = calloc(UI_PIC_MAX_PATH, sizeof(char));
	if (slotPath == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	sprintf(slotPath,
			isActive ?
					UI_PIC_SLOT_ACTIVE_TEMPLATE : UI_PIC_SLOT_INACTIVE_TEMPLATE,
			slot + 1);
	return slotPath;
}

static char** getSlotArrayPath(int page, int numOfPageSlots) {
	int startSlot = page * SAVES_IN_PAGE;
	int numOfPaths = 2 * numOfPageSlots;
	char** paths = calloc(numOfPaths, sizeof(char*));
	if (paths == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	for (int i = 0; i < numOfPaths; i++) {
		int slotIndex = startSlot + i / 2;
		int isActive = (i + 1) % 2;
		paths[i] = getSlotPicPath(slotIndex, isActive);
		if (paths[i] == NULL ) {
			destroyPathArray(paths, i);
			return NULL ;
		}
	}
	return paths;
}

static LoadGameWindowData* getLoadGameWindowData(Window* window) {
	return (LoadGameWindowData*) window->data;
}

static void handleEventActivateGameSlot(Window* window, int slotWidgetID) {
	getLoadGameWindowData(window)->activeSlot = slotWidgetID
			- OTHER_BUTTONS_NUM;
	return;
}

static LoadGameWindowData* createLoadGameWindowData(bool isSaveMode,
		int maxNumOfSlots) {
	LoadGameWindowData* data = malloc(sizeof(LoadGameWindowData));
	if (data == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	data->isSaveMode = isSaveMode;
	data->page = 0; //Upon creation the default page is the first one
	data->numOfPageSlots = getNumOfPageSlots(data->page, maxNumOfSlots);
	data->slotsPicPath = getSlotArrayPath(data->page, data->numOfPageSlots);
	if (data->slotsPicPath == NULL )
		return NULL ;
	data->maxNumOfSlots = maxNumOfSlots;
	data->activeSlot = -1;
	return data;
}

static bool createLoadGameWindowSlots(Widget** widgets, SDL_Renderer* renderer,
		LoadGameWindowData* data) {
	SDL_Rect slotR = { .x = FIRSTGAME_X, .y = FIRSTGAME_Y, .h = FIRSTGAME_H,
			.w = FIRSTGAME_W };
	for (int i = 0; i < data->numOfPageSlots; i++) {
		if (!createButtonInWidgetArray(widgets, OTHER_BUTTONS_NUM + i, renderer,
				slotR, data->slotsPicPath[2 * i], data->slotsPicPath[2 * i + 1],
				UI_EVENT_NONE, UI_BUTTON_EVENT_ACTIVATED_SLOT, false))
			return false;
		slotR.y += GAME_DELTA;
	}
	return true;
}

static Widget** createLoadGameWindowWidgets(SDL_Renderer* renderer,
		LoadGameWindowData* data) {
	int numOfWidgets = OTHER_BUTTONS_NUM + data->numOfPageSlots;
	if (renderer == NULL )
		return NULL ;
	Widget** widgets = (Widget**) calloc(numOfWidgets, sizeof(Widget*));
	if (widgets == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}

	SDL_Rect loadR = { .x = LOAD_X, .y = LOAD_Y, .h = LOAD_H, .w = LOAD_W };
	SDL_Rect backR = { .x = LOAD_BACK_X, .y = LOAD_BACK_Y, .h = LOAD_BACK_H,
			.w = LOAD_BACK_W };
	SDL_Rect nextR = { .x = NEXT_X, .y = NEXT_Y, .h = NEXT_H, .w = NEXT_W };
	SDL_Rect prevR = { .x = PREV_X, .y = PREV_Y, .h = PREV_H, .w = PREV_W };

	int j = LOAD_BTN_WIDGET_ID;
	if (!createButtonInWidgetArray(widgets, j++, renderer, loadR,
			data->isSaveMode ? UI_PIC_SAVE_ACTIVE : UI_PIC_LOAD_ACTIVE,
			data->isSaveMode ? UI_PIC_SAVE_INACTIVE : UI_PIC_LOAD_INACTIVE,
			data->isSaveMode ? UI_BUTTON_EVENT_SAVE : UI_BUTTON_EVENT_LOAD,
			UI_EVENT_NONE, false))
		return NULL ;
	if (!createButtonInWidgetArray(widgets, j++, renderer, backR, UI_PIC_BACK,
			UI_PIC_BACK, UI_BUTTON_EVENT_BACK, UI_BUTTON_EVENT_BACK, true))
		return NULL ;
	if (!createButtonInWidgetArray(widgets, j++, renderer, nextR,
			UI_PIC_NEXT_PAGE_ACTIVE, UI_PIC_NEXT_PAGE_INACTIVE,
			UI_BUTTON_EVENT_NEXT_PAGE, UI_EVENT_NONE,
			(data->page + 1) * SAVES_IN_PAGE < data->maxNumOfSlots))
		return NULL ;
	if (!createButtonInWidgetArray(widgets, j++, renderer, prevR,
			UI_PIC_PREV_PAGE_ACTIVE, UI_PIC_PREV_PAGE_INACTIVE,
			UI_BUTTON_EVENT_PREVIOUS_PAGE, UI_EVENT_NONE, data->page > 0))
		return NULL ;
	bool res = createLoadGameWindowSlots(widgets, renderer, data);
	return res ? widgets : NULL ;
}

static UI_EVENT loadGameWindowHandleEvent(Window* window, SDL_Event* event) {
	if (window == NULL || event == NULL )
		return UI_EVENT_INVALID_ARGUMENT;
	for (int i = 0; i < window->numOfWidgets; i++) {
		UI_EVENT eventResult = window->widgets[i]->handleEvent(
				window->widgets[i], event);
		if (eventResult != UI_EVENT_NONE) {
			if (eventResult == UI_BUTTON_EVENT_ACTIVATED_SLOT)
				handleEventActivateGameSlot(window, i);
			return eventResult;
		}
	}
	return UI_EVENT_NONE;
}

static void loadGameWindowDestroy(Window* window) {
	if (window == NULL )
		return;
	LoadGameWindowData* data = getLoadGameWindowData(window);
	if (data != NULL )
		destroySlotPaths(data);
	windowBaseDestroy(window);
}

Window* loadGameWindowCreate(bool isSaveMode, int maxNumOfSlots) {
	int numOfWidgets = OTHER_BUTTONS_NUM + SAVES_IN_PAGE;
	LoadGameWindowData* data = createLoadGameWindowData(isSaveMode,
			maxNumOfSlots);
	if (data == NULL )
		return NULL ;
	Window* window = windowBaseCreate(UI_PIC_DEFAULT_MENU);
	if (window == NULL )
		return NULL ;
	window->numOfWidgets = numOfWidgets;
	window->handleEvent = loadGameWindowHandleEvent;
	window->destroyWindow = loadGameWindowDestroy;
	window->data = data;
	window->widgets = createLoadGameWindowWidgets(window->renderer, data);
	if (window->widgets == NULL ) {
		windowDestroy(window);
		return NULL ;
	}
	return window;
}

bool loadGameWindowChangePage(Window* window, int page) {
	LoadGameWindowData* data = getLoadGameWindowData(window);
	int numOfPageSlots = getNumOfPageSlots(page, data->maxNumOfSlots);
	char** newPaths = getSlotArrayPath(page, numOfPageSlots);
	if (newPaths == NULL ) {
		return false;
	}
	destroySlotPaths(data);
	widgetListDestory(window->widgets, window->numOfWidgets);

	int numOfWidgets = OTHER_BUTTONS_NUM + numOfPageSlots;
	window->numOfWidgets = numOfWidgets;
	data->page = page;
	data->slotsPicPath = newPaths;
	data->numOfPageSlots = numOfPageSlots;
	window->widgets = createLoadGameWindowWidgets(window->renderer, data);
	return window->widgets != NULL ;
}

void loadGameActivateSlot(Window* window) {
	int gameSlot = getLoadGameWindowData(window)->activeSlot;
	Button* loadButton = (Button*) window->widgets[LOAD_BTN_WIDGET_ID]->data;
	buttonSetActive(loadButton, true);
	for (int i = OTHER_BUTTONS_NUM; i < window->numOfWidgets; i++) {
		Button* slotButton = (Button*) window->widgets[i]->data;
		buttonSetActive(slotButton, (gameSlot + OTHER_BUTTONS_NUM) == i);
	}
}
