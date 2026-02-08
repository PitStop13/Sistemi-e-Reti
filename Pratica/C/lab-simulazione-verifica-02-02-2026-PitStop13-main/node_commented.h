#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node represents a list node, wrapping a string value
// Struttura che rappresenta un nodo di una lista concatenata
typedef struct Node {
  char *value;        // Puntatore alla stringa contenuta nel nodo
  struct Node *next;  // Puntatore al nodo successivo nella lista
} Node;

/**
 * strdupp - Funzione di utilità per duplicare una stringa
 * 
 * SCOPO: Crea una copia indipendente di una stringa in memoria heap
 * 
 * FUNZIONAMENTO:
 * 1. Controlla se la stringa è NULL (gestione errori)
 * 2. Alloca memoria per la nuova stringa (strlen + 1 per '\0')
 * 3. Copia il contenuto della stringa originale nella nuova
 * 4. Restituisce il puntatore alla nuova stringa
 * 
 * PERCHÉ È IMPORTANTE: 
 * - Evita problemi di memoria condivisa tra nodi
 * - Ogni nodo ha la propria copia indipendente dei dati
 * - Se la stringa originale viene modificata/deallocata, il nodo non è affetto
 */
char* strdupp(char *value)
{
  // CONTROLLO 1: Verifica che il valore non sia NULL
  if (value == NULL)
  {
    return NULL;
  }

  // ALLOCAZIONE: Alloca memoria per la copia della stringa
  // strlen(value) + 1 perché dobbiamo includere il carattere terminatore '\0'
  char *duplicate = (char*) malloc(sizeof(char) * (strlen(value) + 1));
  
  // CONTROLLO 2: Verifica che malloc sia riuscita
  if (duplicate == NULL)
  {
    return NULL;  // Memoria esaurita
  }

  // COPIA: Copia il contenuto della stringa originale nella nuova area di memoria
  strcpy(duplicate, value);

  return duplicate;
}

/**
 * createNode - Crea un nuovo nodo con il valore specificato
 *
 * @param value La stringa da inserire nel nodo
 * @returns Il puntatore al nodo creato, o NULL in caso di errore
 * 
 * FUNZIONAMENTO DETTAGLIATO:
 * 1. Validazione input (value != NULL)
 * 2. Duplicazione della stringa (per indipendenza della memoria)
 * 3. Allocazione memoria per la struttura Node
 * 4. Inizializzazione campi (value = copia, next = NULL)
 * 5. Restituzione del puntatore al nuovo nodo
 */
Node *createNode(char *value) {
  // STEP 1: Controllo validità input
  if (value == NULL)
  {
    return NULL;  // Non possiamo creare un nodo senza valore
  }

  // STEP 2: Duplicazione della stringa
  // È FONDAMENTALE duplicare la stringa per evitare che:
  // - Il chiamante modifichi la stringa e influenzi il nodo
  // - Il chiamante deallochi la stringa e il nodo punti a memoria invalida
  char *duplicate = strdupp(value);
  if (duplicate == NULL)
  {
    return NULL;  // Errore di allocazione memoria
  }

  // STEP 3: Allocazione memoria per la struttura Node
  Node *node = (Node*)malloc(sizeof(Node));
  if (node == NULL)
  {
    // Se l'allocazione fallisce, dobbiamo deallocare la stringa duplicata
    // per evitare memory leak
    free(duplicate);
    return NULL;
  }

  // STEP 4: Inizializzazione dei campi del nodo
  node->value = duplicate;  // Assegna la copia della stringa
  node->next = NULL;        // Il nodo non punta ancora a nessun altro nodo

  // STEP 5: Restituzione del nodo creato
  return node;
}

/**
 * cloneNode - Crea una copia completa di un nodo esistente
 *
 * @param node Il nodo da clonare
 * @returns Un nuovo nodo con lo stesso valore, o NULL in caso di errore
 * 
 * NOTA IMPORTANTE: Clona SOLO il nodo singolo, non l'intera lista
 * Il campo 'next' del nuovo nodo sarà NULL
 * 
 * FUNZIONAMENTO:
 * 1. Verifica che il nodo da clonare non sia NULL
 * 2. Riutilizza createNode passando il valore del nodo originale
 * 3. createNode si occupa di duplicare la stringa e creare il nuovo nodo
 */
Node *cloneNode(Node *node) {
  // CONTROLLO: Il nodo da clonare deve esistere
  if (node == NULL)
  {
    return NULL;
  }

  // DELEGA: Usa createNode per creare un nuovo nodo con lo stesso valore
  // Questo garantisce che anche la stringa venga duplicata correttamente
  return createNode(node->value);
}

/**
 * destroyNode - Dealloca completamente un nodo e libera la memoria
 *
 * @param node Il nodo da distruggere
 * @returns 0 se successo, -1 in caso di errore
 * 
 * ATTENZIONE: Questa funzione dealloca SOLO il nodo specificato
 * NON dealloca i nodi collegati tramite 'next'
 * 
 * FUNZIONAMENTO:
 * 1. Verifica che il nodo non sia NULL
 * 2. Libera la memoria della stringa (value)
 * 3. Libera la memoria della struttura Node
 * 
 * ORDINE IMPORTANTE: Devi prima liberare 'value' e poi 'node'
 * perché una volta liberato 'node' non puoi più accedere a 'node->value'
 */
int destroyNode(Node *node) {
  // CONTROLLO: Non si può deallocare un puntatore NULL
  if (node == NULL)
  {
    return -1;  // Errore: nodo non valido
  }

  // STEP 1: Libera la memoria della stringa contenuta nel nodo
  // Questa memoria era stata allocata da strdupp() in createNode()
  free(node->value);
  
  // STEP 2: Libera la memoria della struttura Node stessa
  // Questa memoria era stata allocata da malloc() in createNode()
  free(node);

  // SUCCESSO: Il nodo è stato completamente deallocato
  return 0;
}

#endif