#include <stdlib.h>
#include "UI_Widget.h"
#include "ChessErrorHandler.h"

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

void widgetDestroy(Widget* widget) {
	if (widget == NULL )
		return;
	widget->destroyWidget(widget);
}

void widgetListDestory(Widget** widgets, int numOfWigits) {
	if (widgets == NULL )
		return;
	for (int i = 0; i < numOfWigits; i++)
		widgetDestroy(widgets[i]);
	free(widgets);
}
