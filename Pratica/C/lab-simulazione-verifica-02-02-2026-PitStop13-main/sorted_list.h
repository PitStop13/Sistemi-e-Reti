#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdbool.h>
#include <stdio.h>

#include "node.h"

typedef struct
{
  Node *first;
} SortedList;

/**
 * createEmptySortedList creates a new SortedList entity with no first node
 * (empty list).
 *
 * @returns The created SortedList, or NULL in case of any error.
 */
SortedList *createEmptySortedList()
{
  // implement function logic here...
  SortedList *newList = (SortedList *)malloc(sizeof(SortedList));
  if (newList == NULL)
  {
    return NULL;
  }
  newList->first = NULL;
  return newList;
}

/**
 * cloneSortedList creates a new SortedList entity by cloning another list
 * nodes.
 *
 * @param list The sorted list to clone, must not be NULL.
 *
 * @returns The cloned SortedList, or NULL in case of any error.
 */
SortedList *cloneSortedList(SortedList *list)
{
  // implement function logic here...
  if (list == NULL)
  {
    return NULL;
  }

  SortedList *newList = createEmptySortedList();
  if (newList == NULL)
  {
    return NULL;
  }

  if (list->first == NULL)
  {
    return newList;
  }

  newList->first = cloneNode(list->first);
  if (newList->first == NULL)
  {
    return NULL;
  }

  Node *original = list->first;
  Node *temp = newList->first;

  while (original->next != NULL)
  {
    temp->next = cloneNode(original->next);

    temp = temp->next;
    original = original->next;
  }

  return newList;
}

/**
 * destroySortedList deallocates a SortedList entity resources.
 *
 * @param list The list to destroy, must not be NULL.
 *
 * @returns 0 if successful, -1 on any error.
 */
int destroySortedList(SortedList *list)
{
  // implement function logic here...
  if (list == NULL)
  {
    return -1;
  }

  if (list->first == NULL)
  {
    free(list);
    return 0;
  }

  Node *current = list->first;
  Node *nextNode;
  while (current != NULL)
  {
    nextNode = current->next;
    destroyNode(current);
    current = nextNode;
  }
  free(list);
  return 0;
}

/**
 * getSortedListSize gets the size of the list, in terms of number of nodes.
 *
 * @param list The list to get size from.
 *
 * @returns The number of nodes in the list, or -1 in case of any error.
 */
int getSortedListSize(SortedList *list)
{
  // implement function logic here...
  if (list == NULL)
  {
    return -1;
  }
  if (list->first == NULL)
  {
    return 0;
  }

  int count = 0;
  Node *current = list->first;
  while (current != NULL)
  {
    count++;
    current = current->next;
  }
  return count;
}

/**
 * sortedListContains tells if the specified sorted list contains a node with
 * the specified value. It must be optimized, since the list is sorted.
 *
 * @param list The list to scan.
 *
 * @param value The value to search for.
 *
 * @returns true if the list contains the value, false otherwise.
 */
bool sortedListContains(SortedList *list, char *value)
{
  // implement function logic here...
  if (list == NULL || value == NULL)
  {
    return false;
  }
  if (list->first == NULL)
  {
    return false;
  }
  Node *current = list->first;
  while (current != NULL)
  {
    int cmp = strcmp(current->value, value);
    if (cmp == 0)
    {
      return true;
    }
    // Ottimizzazione: se siamo oltre il valore cercato, possiamo fermarci
    if (cmp > 0)
    {
      return false;
    }
    current = current->next;
  }

  return false;
}

/**
 * insertNode inserts a new node in the sorted list, at the right index, by
 * right-shifting the subsequents.
 *
 * @param list The list to scan.
 *
 * @param element The element to add.
 *
 * @returns 0 if successful, -1 on any error.
 */
int insertNodeInSortedList(SortedList *list, char *value)
{
    // implement function logic here...
    if (list == NULL || value == NULL)
    {
      return -1;
    }

    Node *newNode = createNode(value);
    if (newNode == NULL)
    {
      return -1;
    }

    // Lista vuota: inserisci come primo elemento
    if (list->first == NULL)
    {
      list->first = newNode;
      return 0;
    }

    // Inserimento in testa se il nuovo valore è minore del primo
    if (strcmp(value, list->first->value) < 0)
    {
      newNode->next = list->first;
      list->first = newNode;
      return 0;
    }

    // Cerca la posizione corretta nella lista
    Node *current = list->first;
    while (current->next != NULL && strcmp(value, current->next->value) > 0)
    {
      current = current->next;
    }

    // Inserisci il nodo nella posizione corretta
    newNode->next = current->next;
    current->next = newNode;

    return 0;
}

/**
 * removeNodeFromSortedList removes a Node from the specified list at the
 * specified index, left-shifting the subsequent nodes.
 *
 * @param list The list to scan.
 *
 * @returns The removed node if successfull, NULL on any error.
 */
Node *removeNodeFromSortedList(SortedList *list, int index)
{
  // implement function logic here...
  if (list == NULL || index < 0)
  {
    return NULL;
  }

  if (list->first == NULL)
  {
    return NULL;
  }

  Node *toRemove = NULL;

  // Rimozione del primo nodo (indice 0)
  if (index == 0)
  {
    toRemove = list->first;
    list->first = list->first->next;
    toRemove->next = NULL; // Scollega il nodo dalla lista
    return toRemove;
  }

  // Cerca il nodo all'indice specificato
  Node *current = list->first;
  int currentIndex = 0;

  while (current->next != NULL && currentIndex < index - 1)
  {
    current = current->next;
    currentIndex++;
  }

  // Verifica se l'indice è valido
  if (current->next == NULL)
  {
    return NULL;
  }

  // Rimuovi il nodo
  toRemove = current->next;
  current->next = toRemove->next;
  toRemove->next = NULL; // Scollega il nodo dalla lista

  return toRemove;
}

/**
 * printSortedList prints the values of each node in the list, one per line.
 *
 * @param list The list to print
 */
void printSortedList(SortedList *list)
{
  // implement function logic here...
  if (list == NULL)
  {
    return;
  }

  if (list->first == NULL)
  {
    return;
  }

  Node *current = list->first;
  while (current != NULL)
  {
    printf("%s\n", current->value);
    current = current->next;
  }
}

#endif
