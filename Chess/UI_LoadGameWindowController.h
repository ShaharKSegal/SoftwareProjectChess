#ifndef UI_LOADGAMEWINDOWCONTROLLER_H_
#define UI_LOADGAMEWINDOWCONTROLLER_H_

#include "GameSettings.h"
#include "UI_WindowController.h"

WindowController* loadGameWindowControllerCreate(GameSettings* settings,
		UI_MODE previousMode);

#endif
