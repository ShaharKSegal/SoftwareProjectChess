#ifndef CHESSERRORHANDLER_H_
#define CHESSERRORHANDLER_H_
#include <stdbool.h>

/**
 * Report memory failure to the error handler.
 */
void hadMemoryFailure();

/**
 * Report SDL error to the error handler.
 */
void hadSDLError();

/**
 * Print critical error messages to console. Prioritizes memory failures.
 */
void printCriticalError();

/**
 * Get the current state of critical errors (did we have any?).
 *
 * @return
 * bool - true if we had a critical error, false if not.
 */
bool getHadCriticalError();

/**
 * Get the current state of memory failures (did we have any?).
 *
 * @return
 * bool - true if we had memory failure, false if not.
 */
bool getHadMemoryFailure();

/**
 * Get the current state of SDL error (did we have any?).
 *
 * @return
 * bool - true if we had SDL error, false if not.
 */
bool getHadSDLError();

#endif
