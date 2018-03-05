#include "unit_test_util.h"
#include "ArrayList.h"
#include <stdbool.h>
#include <stdio.h>

#define CAPACITY_SIZE 3

const ChessMove one = { .previousPosition = { .row = 0, .column = 1 } };
const ChessMove two = { .previousPosition = { .row = 1, .column = 2 } };
const ChessMove three = { .previousPosition = { .row = 2, .column = 3 } };
ChessMove arr[] = { { .previousPosition = { .row = 0, .column = 1 } }, {
		.previousPosition = { .row = 1, .column = 2 } }, { .previousPosition = {
		.row = 2, .column = 3 } } };

static bool compMove(ChessMove m1, ChessMove m2) {
	return chessGameIsPositionEquals(m1.previousPosition, m2.previousPosition);
}

static bool ArrayListBasicGetTest() {
	ArrayList* list = arrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list != NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListAddLast(list, arr[i]) == ARRAY_LIST_SUCCESS);
	}
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(compMove(arrayListGetAt(list, i), arr[i]));
	}
	arrayListDestroy(list);
	return true;
}

static bool ArrayListBasicRemoveTest() {
	ArrayList* list = arrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list!=NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListAddLast(list, arr[i]) == ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(arrayListSize(list) == CAPACITY_SIZE);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListRemoveFirst(list) == ARRAY_LIST_SUCCESS);
		if (i < CAPACITY_SIZE - 1)
			ASSERT_TRUE(compMove(arrayListGetFirst(list), arr[i + 1]));
		ASSERT_TRUE(arrayListSize(list) == CAPACITY_SIZE - i - 1);
	}
	ASSERT_TRUE(arrayListIsEmpty(list));
	arrayListDestroy(list);

	list = arrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list!=NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListAddLast(list, arr[i]) == ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(arrayListSize(list) == CAPACITY_SIZE);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListRemoveLast(list) == ARRAY_LIST_SUCCESS);
		if (i < CAPACITY_SIZE - 1)
			ASSERT_TRUE(
					compMove(arrayListGetLast(list), arr[CAPACITY_SIZE - i - 2]));
		ASSERT_TRUE(arrayListSize(list) == CAPACITY_SIZE - i - 1);
	}
	ASSERT_TRUE(arrayListIsEmpty(list));
	arrayListDestroy(list);

	return true;
}

static bool ArrayListBasicAddTest() {
	//First!
	ArrayList* list = arrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list!=NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListAddFirst(list, arr[i]) == ARRAY_LIST_SUCCESS);
		if (i > 0)
			ASSERT_TRUE(compMove(arrayListGetAt(list, 1), arr[i - 1]));
		ASSERT_TRUE(compMove(arrayListGetFirst(list), arr[i]));
		ASSERT_TRUE(arrayListSize(list) == i + 1);
	}
	ASSERT_TRUE(arrayListSize(list) == CAPACITY_SIZE);
	arrayListDestroy(list);
	//Last!
	list = arrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list!=NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListAddLast(list, arr[i]) == ARRAY_LIST_SUCCESS);
		if (i > 0)
			ASSERT_TRUE(compMove(arrayListGetAt(list, i - 1), arr[i - 1]));
		ASSERT_TRUE(compMove(arrayListGetLast(list), arr[i]));
		ASSERT_TRUE(arrayListSize(list) == i + 1);
	}
	ASSERT_TRUE(arrayListSize(list) == CAPACITY_SIZE);
	arrayListDestroy(list);
	return true;
}

static bool ArrayListCreateTest() {
	ArrayList* list = arrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list != NULL);
	arrayListDestroy(list);
	return true;
}

static bool ArrayListBasicCopyTest() {
	ArrayList* list = arrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list!=NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(arrayListAddFirst(list, arr[i]) == ARRAY_LIST_SUCCESS);
	}
	ArrayList* copyList = arrayListCopy(list);
	ASSERT_FALSE(list->elements == copyList->elements);
	ASSERT_FALSE(
			list->maxSize != copyList->maxSize
					|| list->actualSize != copyList->actualSize);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(
				compMove(arrayListGetAt(copyList, i), arrayListGetAt(list, i)));
	}
	arrayListDestroy(list);
	arrayListDestroy(copyList);
	return true;
}

int main1234() {
	RUN_TEST(ArrayListCreateTest);
	RUN_TEST(ArrayListBasicCopyTest);
	RUN_TEST(ArrayListBasicAddTest);
	RUN_TEST(ArrayListBasicRemoveTest);
	RUN_TEST(ArrayListBasicGetTest);
	return 0;
}
