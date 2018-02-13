#include <string.h>
#include <stdlib.h>
#include "ChessErrorHandler.h"

static bool memFailure = false;

void hadMemoryFailure() {
	memFailure = true;
}

bool getHadMemoryFailure() {
	return memFailure;
}
