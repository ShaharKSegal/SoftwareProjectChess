#ifndef CHESSERRORHANDLER_H_
#define CHESSERRORHANDLER_H_
#include <stdbool.h>

/**
 * Report memory failure to the error handler.
 */
void hadMemoryFailure();

/**
 * Get the current state of memory failures (did we have any?).
 *
 * @return
 * bool - true if we had memory failure, false if not.
 */
bool getHadMemoryFailure();

void printSDLError();

bool getHadSDLError();

#endif
