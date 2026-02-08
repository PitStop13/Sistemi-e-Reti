#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "testing/test.h"

#include "../node.h"
#include "../sorted_list.h"

#define MAX_LENGTH 100
#define TEST_INSERT_SIZE 5

void testCreateNode();
void testCloneNode();
void testDestroyNode();
void testCreateEmptySortedList();
void testCloneSortedList();
void testDestroySortedList();
void testGetSortedListSize();
void testSortedListContains();
void testInsertNodeInSortedList();
void testPrintSortedList();

int main(void) {
  testCreateNode();
  testCloneNode();
  testDestroyNode();

  testCreateEmptySortedList();
  testCloneSortedList();
  testDestroySortedList();

  testGetSortedListSize();
  testSortedListContains();

  testInsertNodeInSortedList();

  testPrintSortedList();

  return 0;
}

void testCreateNode() {
  openTestGroup("createNode(...)");

  for (int i = 0; i < 5; i++) {
    char buffer[MAX_LENGTH];
    sprintf(buffer, "%d", i);
    Node *result = createNode(buffer);

    char test_name[MAX_LENGTH];
    sprintf(test_name, "createNode(\"%d\")", i);
    assert(test_name, result != NULL && strcmp(result->value, buffer) == 0);

    free(result);
  }

  closeTestGroup();
}

void testCloneNode() {
  openTestGroup("cloneNode(...)");

  Node *result = cloneNode(NULL);
  assert("cloneNode(NULL)", result == NULL);

  for (int i = 0; i < 5; i++) {
    char buffer[MAX_LENGTH];
    sprintf(buffer, "%d", i);
    Node temp = (Node){buffer, NULL};
    result = cloneNode(&temp);

    char test_name[MAX_LENGTH];
    sprintf(test_name, "cloneNode(temp) - Test %d", i);
    assert(test_name, result != NULL &&
                          strcmp(result->value, temp.value) == 0 &&
                          result->next == NULL);

    free(result);
  }

  closeTestGroup();
}

void testDestroyNode() {
  openTestGroup("destroyNode(...)");

  int result = destroyNode(NULL);
  assert("destroyNode(NULL)", result == -1);

  Node *node = malloc(sizeof(Node));
  node->value = (char *)malloc(sizeof(char) * 2);
  strcpy(node->value, "5");
  node->next = NULL;

  result = destroyNode(node);
  assert("destroyNode(node)", result == 0);
  assert("destroyNode(node) - de-allocation", !isAllocated(node));

  closeTestGroup();
}

void testCreateEmptySortedList() {
  openTestGroup("createEmptySortedList(...)");

  SortedList *result = createEmptySortedList();
  assert("createEmptySortedList()", result != NULL && result->first == NULL);

  closeTestGroup();
}

void testCloneSortedList() {
  openTestGroup("cloneSortedList(...)");

  SortedList *result = cloneSortedList(NULL);
  assert("cloneSortedList(NULL)", result == NULL);

  SortedList temp_list = (SortedList){NULL};
  result = cloneSortedList(&temp_list);
  assert("cloneSortedList(empty_list)",
         result != NULL && result->first == NULL);

  for (int i = 0; i < 5; i++) {
    char test_name[MAX_LENGTH];
    sprintf(test_name, "cloneSortedList(list) - Test %d", i);

    char buffer[MAX_LENGTH];
    sprintf(buffer, "%d", i);
    Node temp_node = (Node){buffer, NULL};
    temp_list = (SortedList){&temp_node};
    result = cloneSortedList(&temp_list);
    assert(test_name, result != NULL && result->first != NULL &&
                          strcmp(result->first->value, temp_node.value) == 0 &&
                          result->first->next == temp_node.next);
  }

  closeTestGroup();
}

void testDestroySortedList() {
  openTestGroup("destroySortedList(...)");

  int result = destroySortedList(NULL);
  assert("destroySortedList(NULL)", result == -1);

  SortedList *temp_list = (SortedList *)malloc(sizeof(SortedList));
  *temp_list = (SortedList){NULL};
  result = destroySortedList(temp_list);
  assert("destroySortedList(empty_list)",
         result == 0 && !isAllocated(temp_list));

  for (int i = 0; i < 5; i++) {
    char test_name[MAX_LENGTH];
    sprintf(test_name, "destroySortedList(list) - Test %d", i);

    Node *temp_node = (Node *)malloc(sizeof(Node));
    char *node_value = (char *)malloc(sizeof(char) * 5);
    strcpy(node_value, "test");
    *temp_node = (Node){node_value, NULL};

    temp_list = (SortedList *)malloc(sizeof(SortedList));
    *temp_list = (SortedList){temp_node};
    result = destroySortedList(temp_list);
    assert(test_name,
           result == 0 && !isAllocated(temp_list) && !isAllocated(temp_node));
  }

  closeTestGroup();
}

void testGetSortedListSize() {
  openTestGroup("getSortedListSize(...)");

  int result = getSortedListSize(NULL);
  assert("getSortedListSize(NULL)", result == -1);

  SortedList temp_list = (SortedList){NULL};
  result = getSortedListSize(&temp_list);
  assert("getSortedListSize(empty_list)", result == 0);

  Node temp_node_3 = (Node){"3", NULL};
  Node temp_node_2 = (Node){"2", &temp_node_3};
  Node temp_node_1 = (Node){"1", &temp_node_2};

  temp_list = (SortedList){&temp_node_1};
  result = getSortedListSize(&temp_list);
  assert("getSortedListSize(list) - Test 1", result == 3);

  temp_node_2.next = NULL;
  result = getSortedListSize(&temp_list);
  assert("getSortedListSize(list) - Test 2", result == 2);

  temp_node_1.next = NULL;
  result = getSortedListSize(&temp_list);
  assert("getSortedListSize(list) - Test 3", result == 1);

  closeTestGroup();
}

void testSortedListContains() {
  openTestGroup("sortedListContains(...)");

  bool result = sortedListContains(NULL, "1");
  assert("sortedListContains(NULL, \"1\")", result == false);

  SortedList temp_list = (SortedList){NULL};
  result = sortedListContains(&temp_list, "1");
  assert("sortedListContains(empty_list, \"1\")", result == false);

  Node temp_node_3 = (Node){"3", NULL};
  Node temp_node_2 = (Node){"2", &temp_node_3};
  Node temp_node_1 = (Node){"1", &temp_node_2};

  temp_list = (SortedList){&temp_node_1};
  result = sortedListContains(&temp_list, "4");
  assert("sortedListContains(list, \"4\") - Test 1", result == false);

  result = sortedListContains(&temp_list, "2");
  assert("sortedListContains(list, \"2\") - Test 1", result == true);

  result = sortedListContains(&temp_list, "1");
  assert("sortedListContains(list, \"1\") - Test 1", result == true);

  result = sortedListContains(&temp_list, "3");
  assert("sortedListContains(list, \"3\") - Test 1", result == true);

  temp_node_2.next = NULL;

  result = sortedListContains(&temp_list, "4");
  assert("sortedListContains(list, \"4\") - Test 2", result == false);

  result = sortedListContains(&temp_list, "2");
  assert("sortedListContains(list, \"2\") - Test 2", result == true);

  result = sortedListContains(&temp_list, "1");
  assert("sortedListContains(list, \"1\") - Test 2", result == true);

  result = sortedListContains(&temp_list, "3");
  assert("sortedListContains(list, \"3\") - Test 2", result == false);

  temp_node_1.next = NULL;

  result = sortedListContains(&temp_list, "4");
  assert("sortedListContains(list, \"4\") - Test 3", result == false);

  result = sortedListContains(&temp_list, "2");
  assert("sortedListContains(list, \"2\") - Test 3", result == false);

  result = sortedListContains(&temp_list, "1");
  assert("sortedListContains(list, \"1\") - Test 3", result == true);

  result = sortedListContains(&temp_list, "3");
  assert("sortedListContains(list, \"3\") - Test 3", result == false);

  closeTestGroup();
}

void printSortedList();
static int testInsert(int expected[TEST_INSERT_SIZE],
                      int actual[TEST_INSERT_SIZE]) {
  SortedList temp_list = (SortedList){NULL};
  for (int i = 0; i < TEST_INSERT_SIZE; i++) {
    char buffer[MAX_LENGTH];
    sprintf(buffer, "%d", actual[i]);
    int result = insertNodeInSortedList(&temp_list, buffer);
    if (result == -1) {
      return -1;
    }
  }

  int i = 0;
  Node *iterator = temp_list.first;
  while (iterator != NULL && i < TEST_INSERT_SIZE) {
    char expected_buffer[MAX_LENGTH];
    sprintf(expected_buffer, "%d", expected[i]);
    if (strcmp(iterator->value, expected_buffer) != 0) {
      return -1;
    }

    iterator = iterator->next;
    i++;
  }

  if (i != TEST_INSERT_SIZE) {
    return -1;
  }

  return 0;
}

void testInsertNodeInSortedList() {
  openTestGroup("insertNode(...)");

  int result = insertNodeInSortedList(NULL, "1");
  assert("insertNodeInSortedList(NULL, \"1\")", result == -1);

  int expected[TEST_INSERT_SIZE] = {1, 2, 3, 4, 5};
  // sorted insertion test
  int test_insert_values[TEST_INSERT_SIZE] = {1, 2, 3, 4, 5};
  result = testInsert(expected, test_insert_values);
  assert("insertNodeInSortedList - test already sorted", result == 0);

  // reverse insertion test
  test_insert_values[0] = 5;
  test_insert_values[1] = 4;
  test_insert_values[2] = 3;
  test_insert_values[3] = 2;
  test_insert_values[4] = 1;

  result = testInsert(expected, test_insert_values);
  assert("insertNodeInSortedList - test reverse sorted", result == 0);

  // // unsorted insertion tests
  test_insert_values[0] = 3;
  test_insert_values[1] = 1;
  test_insert_values[2] = 5;
  test_insert_values[3] = 4;
  test_insert_values[4] = 2;

  result = testInsert(expected, test_insert_values);
  assert("insertNodeInSortedList - test randomly unsorted", result == 0);

  closeTestGroup();
}

void printSortedListNULLCallback() {
  // NULL list check
  printSortedList(NULL);
}

void printSortedListEmptyCallback() {
  // empty list check
  SortedList temp_list = (SortedList){NULL};

  printSortedList(&temp_list);
}

void printSortedListCallback() {
  // normal list check
  Node temp_node_2 = (Node){"2", NULL};
  Node temp_node_1 = (Node){"1", &temp_node_2};
  SortedList temp_list = (SortedList){&temp_node_1};

  printSortedList(&temp_list);
}

void testPrintSortedList() {
  openTestGroup("printSortedList(...)");

  printSortedListNULLCallback();
  char **captured = getLastCapturedPrints(1);
  char *captured_output = (captured && captured[0]) ? captured[0] : "";
  assert("printSortedList(NULL)", strcmp(captured_output, "") == 0);
  if (captured) free(captured);

  printSortedListEmptyCallback();
  captured = getLastCapturedPrints(1);
  captured_output = (captured && captured[0]) ? captured[0] : "";
  assert("printSortedList(empty_list)", strcmp(captured_output, "") == 0);
  if (captured) free(captured);

  printSortedListCallback();
  captured = getLastCapturedPrints(2);
  if (captured) {
    char combined[MAX_LENGTH] = "";
    strcat(combined, captured[0]);
    strcat(combined, captured[1]);
    assert("printSortedList(list)", strcmp(combined, "1\n2\n") == 0);
    free(captured);
  }

  closeTestGroup();
}
