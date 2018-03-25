#include <stdlib.h>
#include "UI_Widget.h"
#include "ChessErrorHandler.h"

/**
 * A common widget base creation for all widgets.
 * Can have memmory allocations errors.
 * @returns
 * If any argument is NULL, returns NULL.
 * Otherwise a Widget with the relevant fields filled.
 */
Widget* widgetCreate(void* data, void (*drawWidget)(Widget*),
		UI_EVENT (*handleEvent)(Widget*, SDL_Event*), void (*destroyWidget)(Widget*)) {
	if (data == NULL || drawWidget == NULL || handleEvent == NULL
			|| destroyWidget == NULL )
		return NULL ;
	Widget* widget = (Widget*) malloc(sizeof(Widget));
	if (widget == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}
	widget->data = data;
	widget->destroyWidget = destroyWidget;
	widget->drawWidget = drawWidget;
	widget->handleEvent = handleEvent;

	return widget;
}

/**
 * A generic destroy function. Calls widget.destroyWidget is not NULL.
 */
void widgetDestroy(Widget* widget) {
	if (widget == NULL )
		return;
	widget->destroyWidget(widget);
}

/**
 * A generic destroy function for a list of widgets.
 * Assumes there are numOfWigits widgets.
 * Calls widget.destroyWidget for each widget and then frees widgets.
 */
void widgetListDestory(Widget** widgets, int numOfWigits) {
	if (widgets == NULL )
		return;
	for (int i = 0; i < numOfWigits; i++)
		widgetDestroy(widgets[i]);
	free(widgets);
}
