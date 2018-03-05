#include <stdio.h>
#include <stdlib.h>
#include "ArrayList.h"
#include "ChessErrorHandler.h"

/**
 * Basic check if arr is not null and index not out of bound
 */
bool isValidIndex(ArrayList* arr, int index) {
	return arr == NULL || index < 0 || arr->actualSize < index;
}
/**
 *  Creates an empty array list with the specified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
ArrayList* arrayListCreate(int maxSize) {
	if (maxSize <= 0)
		return NULL ;
	ArrayList* list = malloc(sizeof(ArrayList));
	if (list == NULL ) {
		hadMemoryFailure();
		return NULL ;
	}

	list->maxSize = maxSize;
	list->actualSize = 0;
	list->elements = malloc(maxSize * sizeof(ChessMove));
	if (list->elements == NULL ) {
		arrayListDestroy(list);
		hadMemoryFailure();
		return NULL ;
	}
	return list;
}

/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
ArrayList* arrayListCopy(ArrayList* src) {
	if (src == NULL )
		return NULL ;
	ArrayList* list = arrayListCreate(src->maxSize);
	if (list == NULL )
		return NULL ;
	list->actualSize = src->actualSize;
	for (int i = 0; i < list->actualSize; i++)
		list->elements[i] = src->elements[i];
	return list;
}

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void arrayListDestroy(ArrayList* src) {
	if (src == NULL )
		return;
	if (src->elements != NULL )
		free(src->elements);
	free(src);
}

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 * @param src - the source array list
 * @return
 * ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * ARRAY_LIST_SUCCESS otherwise
 */
ARRAY_LIST_MESSAGE arrayListClear(ArrayList* src) {
	if (src == NULL )
		return ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize = 0;
	return ARRAY_LIST_SUCCESS;
}

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
ARRAY_LIST_MESSAGE arrayListAddAt(ArrayList* src, ChessMove elem, int index) {
	if (isValidIndex(src, index))
		return ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == src->maxSize)
		return ARRAY_LIST_FULL;
	for (int i = src->actualSize - 1; i >= index; i--) {
		src->elements[i + 1] = src->elements[i];
	}
	src->elements[index] = elem;
	src->actualSize++;
	return ARRAY_LIST_SUCCESS;

}

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
ARRAY_LIST_MESSAGE arrayListAddFirst(ArrayList* src, ChessMove elem) {
	return arrayListAddAt(src, elem, 0);
}

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
ARRAY_LIST_MESSAGE arrayListAddLast(ArrayList* src, ChessMove elem) {
	return arrayListAddAt(src, elem, src->actualSize);
}

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
ARRAY_LIST_MESSAGE arrayListRemoveAt(ArrayList* src, int index) {
	if (isValidIndex(src, index))
		return ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == 0)
		return ARRAY_LIST_EMPTY;
	for (int i = index; i < src->actualSize - 1; i++) {
		src->elements[i] = src->elements[i + 1];
	}
	src->actualSize--;
	return ARRAY_LIST_SUCCESS;
}

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
ARRAY_LIST_MESSAGE arrayListRemoveFirst(ArrayList* src) {
	return arrayListRemoveAt(src, 0);
}

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
ARRAY_LIST_MESSAGE arrayListRemoveLast(ArrayList* src) {
	return arrayListRemoveAt(src, src->actualSize - 1);
}

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
ChessMove arrayListGetAt(ArrayList* src, int index) {
	ChessMove move;
	if (src == NULL || src->actualSize <= index)
		return move;
	return src->elements[index];
}

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
ChessMove arrayListGetFirst(ArrayList* src) {
	return arrayListGetAt(src, 0);
}

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
ChessMove arrayListGetLast(ArrayList* src) {
	return arrayListGetAt(src, src->actualSize - 1);
}

/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int arrayListMaxCapacity(ArrayList* src) {
	if (src == NULL )
		return -1;
	return src->maxSize;
}

/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int arrayListSize(ArrayList* src) {
	if (src == NULL )
		return -1;
	return src->actualSize;
}

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */bool arrayListIsFull(ArrayList* src) {
	return src != NULL && src->actualSize == src->maxSize;
}

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */bool arrayListIsEmpty(ArrayList* src) {
	return src != NULL && src->actualSize == 0;
}

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
		bool isThreatened) {
	if (isValidIndex(arr, index))
		return ARRAY_LIST_INVALID_ARGUMENT;
	arr->elements[index].isThreatened = isThreatened;
	return ARRAY_LIST_SUCCESS;

}
