#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>

#ifndef _TESTING_H_
#define TESTING_H

// by undefining malloc and free we can
// attach later our testing macros
#undef malloc
#undef free

#define MAX_ALLOCATED 100

static void *allocated[MAX_ALLOCATED];
static int currently_allocated = 0;

static bool group_open = false;

/**
 * applyIndent applies the indentation in case of test groups
 */
static void applyIndent() {
  if (group_open) {
    printf("  ");
  }
}

/**
 * passTest prints that a specified test has been passed successfully.
 *
 * @param test_name The test name to print
 */
void passTest(const char *test_name) {
  applyIndent();
  printf("✔ PASSED - %s\n", test_name);
}

/**
 * failTest prints that a specified test has failed.
 *
 * @param test_name The test name to print
 */
void failTest(const char *test_name) {
  applyIndent();
  printf("❌ FAILED - %s\n", test_name);
}

/**
 * assert prints the result of a test, based on the provided expression.
 *
 * @param test_name The test name to print
 * @param expression The expression to verify to decide if the test has passed
 */
void assert(const char *test_name, bool expression) {
  if (expression) {
    passTest(test_name);
  } else {
    failTest(test_name);
  }
}

/**
 * require prints the result of a test, based on the provided expression.
 * Additionally, it exits the program if the test fails.
 *
 * @param test_name The test name to print
 * @param expression The expression to verify to decide if the test has passed
 */
void require(const char *test_name, bool expression) {
  if (expression) {
    passTest(test_name);
  } else {
    failTest(test_name);
    exit(-1);
  }
}

void openTestGroup(const char *);
void closeTestGroup();

/**
 * openTestGroup opens a new test group and allows to apply indent to better
 * organize tests.
 *
 * @param test_group_name The name of the test group
 */
void openTestGroup(const char *test_group_name) {
  if (group_open) {
    closeTestGroup();
  }
  group_open = true;

  printf("%s\n", test_group_name);
}

/**
 * closeTestGroup closes a previously open test group.
 */
void closeTestGroup() {
  if (group_open) {
    group_open = false;

    printf("\n");
  }
}

/**
 * testMalloc is a mock of the malloc function that allows to test
 * dynamic allocation behaviors.
 *
 * @see malloc
 */
void *testMalloc(size_t size, const char *file, int line, const char *func) {
  void *ptr = malloc(size);

  allocated[currently_allocated++] = ptr;

  return ptr;
}

/**
 * testFree is a mock of the free function that allows to test
 * dynamic allocation behaviors.
 *
 * @see free
 */
void testFree(void *ptr) {
  int last_allocated = currently_allocated - 1;
  for (int i = last_allocated; i >= 0; i--) {
    if (ptr == allocated[i]) {
      allocated[i] = NULL;

      if (i < last_allocated) {
        allocated[i] = allocated[last_allocated];
      }

      currently_allocated--;
      break;
    }
  }

  free(ptr);
}

/**
 * isAllocated returns true if the memory area pointed by ptr is allocated
 * by malloc or not. It is mainly used to test memory allocations in a program
 * and works only if malloc and free have been mocked with test_malloc and
 * test_free.
 *
 * @param ptr The pointer to check
 *
 * @returns True if the memory area pointed is allocated via malloc, False
 * otherwise.
 *
 * @see test_malloc
 *
 * @see test_free
 */
bool isAllocated(const void *ptr) {
  for (int i = 0; i < currently_allocated; i++) {
    if (ptr == allocated[i]) {
      return true;
    }
  }

  return false;
}

#define malloc(X) testMalloc(X, __FILE__, __LINE__, __FUNCTION__)
#define free(X) testFree(X)

#endif