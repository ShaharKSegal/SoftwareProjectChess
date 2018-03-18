#include <stdlib.h>
#include <stdio.h>
#include "UI_WindowController.h"
#include "ChessErrorHandler.h"

WindowController* windowControllerCreate(Window* window, void* data,
		UI_CONTROLLER_EVENT (*handleEventController)(WindowController**,
				SDL_Event*), void (*destroyWindowController)(WindowController*)) {
	if (window == NULL || handleEventController == NULL )
		return NULL ;
	WindowController* controller = (WindowController*) malloc(
			sizeof(WindowController));
	if (controller == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	controller->window = window;
	controller->data = data;
	controller->handleEventController = handleEventController;
	controller->destroyWindowController = destroyWindowController;
	windowDraw(window);
	if (getHadSDLError())
		return NULL ;
	return controller;
}

UI_CONTROLLER_EVENT windowControllerHandleEvent(
		WindowController** controllerPtr, SDL_Event* event) {
	if (controllerPtr == NULL || *controllerPtr == NULL || event == NULL )
		return UI_CONTROLLER_EVENT_NONE;
	return (*controllerPtr)->handleEventController(controllerPtr, event);
}

void windowControllerBaseDestroy(WindowController* controller) {
	if (controller == NULL || controller->window == NULL )
		return;
	windowDestroy(controller->window);
	free(controller->data);
	free(controller);
}

void windowControllerDestroy(WindowController* controller) {
	if (controller == NULL )
		return;
	if (controller->destroyWindowController != NULL )
		controller->destroyWindowController(controller);
	else
		windowControllerBaseDestroy(controller);
}
