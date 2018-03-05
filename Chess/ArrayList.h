#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_
#include <stdbool.h>
#include "ChessGameCommon.h"

/**
 * ArrayList summary:
 *
 * A container that represents a fixed size linked list. The capacity of the list
 * is specified at the creation. The container supports typical list
 * functionalities with the addition of random access as in arrays.
 * Upon insertion, if the maximum capacity is reached then an error message is
 * returned and the list is not affected. A summary of the supported functions
 * is given below:
 *
 * arrayListCreate       - Creates an empty array list with a specified
 *                           max capacity.
 * arrayListCopy         - Creates an exact copy of a specified array list.
 * arrayListDestroy      - Frees all memory resources associated with an array
 *                           list.
 * arrayListClear        - Clears all elements from a specified array list.
 * arrayListAddAt        - Inserts an element at a specified index, elements
 *                           will be shifted to make place.
 * arrayListAddFirst     - Inserts an element at the beginning of the array
 *                           list, elements will be shifted to make place.
 * arrayListAddLast      - Inserts an element at the end of the array list.
 * arrayListRemoveAt     - Removes an element at the specified index, elements
 *                           elements will be shifted as a result.
 * arrayListRemoveFirst  - Removes an element from the beginning of the array
 *                           list, elements will be shifted as a result.
 * arrayListRemoveLast   - Removes an element from the end of the array list
 * arrayListGetAt        - Accesses the element at the specified index.
 * arrayListGetFirst     - Accesses the first element of the array list.
 * arrayListGetLast      - Accesses the last element of the array list.
 * arrayListMaxCapcity   - Returns the maximum capacity of the array list.
 * arrayListSize         - Returns the number of elements in that array list.
 * arrayListIsFull       - Returns if the array list reached its max capacity.
 * arrayListIsEmpty      - Returns true if the array list contains no elements.
 * arrayListIsThreatened - Updates isThreatened field at a specified index.
 */

/**
 * Type for chess moves
 */
typedef struct chess_game_move_t {
	ChessPiecePosition previousPosition;
	ChessPiecePosition currentPosition;
	ChessPiece capturedPiece;
	bool isThreatened;
} ChessMove;

/**
 * Type for an array list of chess moves
 */
typedef struct array_list_t {
	ChessMove* elements;
	int actualSize;
	int maxSize;
} ArrayList;

/**
 * A type used for errors
 */
typedef enum array_list_message_t {
	ARRAY_LIST_SUCCESS,
	ARRAY_LIST_INVALID_ARGUMENT,
	ARRAY_LIST_FULL,
	ARRAY_LIST_EMPTY
} ARRAY_LIST_MESSAGE;

/**
 *  Creates an empty array list with the specified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
ArrayList* arrayListCreate(int maxSize);

/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
ArrayList* arrayListCopy(ArrayList* src);

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void arrayListDestroy(ArrayList* src);

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 * @param src - the source array list
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * ARRAY_LIST_SUCCESS otherwise
 */
ARRAY_LIST_MESSAGE arrayListClear(ArrayList* src);

/**
 * Inserts element at a specified index. The elements residing at and after the
 * specified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE arrayListAddAt(ArrayList* src, ChessMove elem, int index);

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * ARRAY_LIST_SUCCESS - otherwise
 */
 ARRAY_LIST_MESSAGE arrayListAddFirst(ArrayList* src, ChessMove elem);

/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity and error message is returned and the source
 * list is not affected.
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE arrayListAddLast(ArrayList* src, ChessMove elem);

/**
 * Removes an element from a specified index. The elements residing after the
 * specified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param index - The index from where the element will be removed.
 *                The index is 0-based.
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * ARRAY_LIST_EMPTY - if the source array list is empty
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE arrayListRemoveAt(ArrayList* src, int index);

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * ARRAY_LIST_EMPTY - if the source array list is empty
 * ARRAY_LIST_SUCCESS - otherwise
 */
ARRAY_LIST_MESSAGE arrayListRemoveFirst(ArrayList* src);

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * ARRAY_LIST_EMPTY - if the source array list is empty
 * ARRAY_LIST_SUCCESS - otherwise.
 */
ARRAY_LIST_MESSAGE arrayListRemoveLast(ArrayList* src);

/**
 * Returns the element at the specified index. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @param index - the specified index, the index is 0-based.
 * @return
 * Undefined value if either src == NULL or index out of bound.
 * Otherwise, the element at the specified index is returned.
 */
ChessMove arrayListGetAt(ArrayList* src, int index);

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
ChessMove arrayListGetFirst(ArrayList* src);

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
ChessMove arrayListGetLast(ArrayList* src);

/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int arrayListMaxCapacity(ArrayList* src);

/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int arrayListSize(ArrayList* src);

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool arrayListIsFull(ArrayList* src);

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool arrayListIsEmpty(ArrayList* src);

/**
 * Update isThreatened of element in index.
 * @param arr   		- The source array list
 * @param index 		- The element's index
 * @param isThreatened - update value
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT - if arr == NULL or index out of bound
 * ARRAY_LIST_SUCCESS - otherwise.
 */
ARRAY_LIST_MESSAGE arrayListIsThreatened(ArrayList* arr, int index,
		bool isThreatened);

#endif
