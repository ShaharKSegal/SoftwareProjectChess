#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include "UI_Widget.h"
#include "UI_Window.h"

/**
 * Summary: This is an implementation of the main window, which creates
 * a Window type with the relevant widgets and operations regarding the main menu.
 */

/**
 * Creates a Window type with widgets and operations of the main menu
 * Can have memory failures or SDL errors
 * @returns
 * NULL if some error occurred, otherwise a Window* of the main menu.
 */
Window* mainWindowCreate();

#endif
