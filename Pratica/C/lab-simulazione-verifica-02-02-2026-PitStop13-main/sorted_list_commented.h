#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdbool.h>
#include <stdio.h>

#include "node.h"

/**
 * SortedList - Struttura che rappresenta una lista ordinata
 * 
 * CARATTERISTICHE:
 * - Lista concatenata semplice (singly linked list)
 * - Mantiene i nodi in ordine alfabetico CRESCENTE
 * - first: puntatore al primo nodo della lista (NULL se lista vuota)
 */
typedef struct
{
  Node *first;  // Puntatore al primo nodo (testa della lista)
} SortedList;

/**
 * createEmptySortedList - Crea una nuova lista ordinata vuota
 *
 * @returns Puntatore alla nuova lista, o NULL in caso di errore
 * 
 * FUNZIONAMENTO:
 * 1. Alloca memoria per la struttura SortedList
 * 2. Inizializza il puntatore first a NULL (lista vuota)
 * 3. Restituisce il puntatore alla lista
 * 
 * NOTA: Una lista vuota ha first = NULL
 */
SortedList *createEmptySortedList()
{
  // STEP 1: Alloca memoria per la struttura SortedList
  SortedList *newList = (SortedList *)malloc(sizeof(SortedList));
  
  // CONTROLLO: Verifica successo allocazione
  if (newList == NULL)
  {
    return NULL;  // Memoria esaurita
  }
  
  // STEP 2: Inizializza la lista come vuota
  newList->first = NULL;
  
  return newList;
}

/**
 * cloneSortedList - Crea una copia completa di una lista esistente
 *
 * @param list La lista da clonare
 * @returns Una nuova lista identica, o NULL in caso di errore
 * 
 * FUNZIONAMENTO DETTAGLIATO:
 * 1. Validazione: controlla che list non sia NULL
 * 2. Crea una nuova lista vuota
 * 3. Se la lista originale è vuota, restituisce la lista vuota
 * 4. Clona il primo nodo
 * 5. Itera attraverso tutti i nodi rimanenti e li clona uno per uno
 * 6. Collega i nodi clonati nella stessa sequenza
 * 
 * TECNICA: Usa due puntatori (original e temp) per navigare in parallelo
 */
SortedList *cloneSortedList(SortedList *list)
{
  // CONTROLLO 1: Validazione input
  if (list == NULL)
  {
    return NULL;
  }

  // STEP 1: Crea la struttura della nuova lista
  SortedList *newList = createEmptySortedList();
  if (newList == NULL)
  {
    return NULL;  // Errore di allocazione
  }

  // CASO SPECIALE: Se la lista originale è vuota, restituisci lista vuota
  if (list->first == NULL)
  {
    return newList;
  }

  // STEP 2: Clona il primo nodo
  newList->first = cloneNode(list->first);
  if (newList->first == NULL)
  {
    free(newList);  // Dealloca la lista se la clonazione fallisce
    return NULL;
  }

  // STEP 3: Prepara i puntatori per iterare
  Node *original = list->first;      // Puntatore per navigare la lista originale
  Node *temp = newList->first;       // Puntatore per costruire la nuova lista

  // STEP 4: Itera attraverso tutti i nodi rimanenti
  while (original->next != NULL)
  {
    // Clona il nodo successivo
    temp->next = cloneNode(original->next);
    
    // Avanza entrambi i puntatori
    temp = temp->next;
    original = original->next;
  }

  return newList;
}

/**
 * destroySortedList - Dealloca completamente una lista e tutti i suoi nodi
 *
 * @param list La lista da distruggere
 * @returns 0 se successo, -1 in caso di errore
 * 
 * FUNZIONAMENTO:
 * 1. Validazione: controlla che list non sia NULL
 * 2. Se la lista è vuota, dealloca solo la struttura SortedList
 * 3. Altrimenti, itera attraverso tutti i nodi e li dealloca uno per uno
 * 4. Alla fine, dealloca la struttura SortedList stessa
 * 
 * TECNICA IMPORTANTE: Usa un puntatore 'nextNode' per non perdere
 * il riferimento al nodo successivo dopo aver deallocato il nodo corrente
 */
int destroySortedList(SortedList *list)
{
  // CONTROLLO: Validazione input
  if (list == NULL)
  {
    return -1;
  }

  // CASO SPECIALE: Lista vuota, dealloca solo la struttura
  if (list->first == NULL)
  {
    free(list);
    return 0;
  }

  // STEP 1: Prepara i puntatori per l'iterazione
  Node *current = list->first;
  Node *nextNode;
  
  // STEP 2: Itera attraverso tutti i nodi e deallocali
  while (current != NULL)
  {
    // IMPORTANTE: Salva il riferimento al prossimo nodo PRIMA di deallocare
    // Altrimenti, dopo destroyNode(current), non puoi più accedere a current->next
    nextNode = current->next;
    
    // Dealloca il nodo corrente (sia la stringa che la struttura Node)
    destroyNode(current);
    
    // Avanza al nodo successivo
    current = nextNode;
  }
  
  // STEP 3: Dealloca la struttura SortedList stessa
  free(list);
  
  return 0;
}
/**
 * getSortedListSize - Calcola il numero di nodi nella lista
 *
 * @param list La lista di cui calcolare la dimensione
 * @returns Il numero di nodi, o -1 in caso di errore
 * 
 * FUNZIONAMENTO:
 * 1. Validazione: controlla che list non sia NULL
 * 2. Se la lista è vuota (first == NULL), restituisce 0
 * 3. Itera attraverso tutti i nodi contandoli
 * 4. Restituisce il conteggio finale
 * 
 * COMPLESSITÀ: O(n) - deve visitare tutti i nodi
 */
int getSortedListSize(SortedList *list)
{
  // CONTROLLO 1: Validazione input
  if (list == NULL)
  {
    return -1;  // Errore: lista non valida
  }
  
  // CASO SPECIALE: Lista vuota
  if (list->first == NULL)
  {
    return 0;
  }

  // STEP 1: Inizializza il contatore
  int count = 0;
  
  // STEP 2: Prepara il puntatore per l'iterazione
  Node *current = list->first;
  
  // STEP 3: Itera attraverso tutti i nodi
  while (current != NULL)
  {
    count++;                  // Incrementa il contatore
    current = current->next;  // Passa al nodo successivo
  }
  
  return count;
}

/**
 * sortedListContains - Verifica se un valore è presente nella lista
 *
 * @param list La lista in cui cercare
 * @param value Il valore da cercare
 * @returns true se trovato, false altrimenti
 * 
 * FUNZIONAMENTO:
 * 1. Validazione: controlla che list e value non siano NULL
 * 2. Itera attraverso i nodi confrontando i valori con strcmp
 * 3. Se trova una corrispondenza, restituisce true
 * 4. OTTIMIZZAZIONE: Se il valore corrente è maggiore di quello cercato,
 *    si ferma (la lista è ordinata, quindi non lo troverà più avanti)
 * 
 * COMPLESSITÀ: 
 * - Caso medio: O(n/2) grazie all'ottimizzazione
 * - Caso peggiore: O(n) se l'elemento è l'ultimo o non esiste
 */
bool sortedListContains(SortedList *list, char *value)
{
  // CONTROLLO 1: Validazione input
  if (list == NULL || value == NULL)
  {
    return false;
  }
  
  // CONTROLLO 2: Lista vuota
  if (list->first == NULL)
  {
    return false;
  }
  
  // STEP 1: Inizia dal primo nodo
  Node *current = list->first;
  
  // STEP 2: Itera attraverso i nodi
  while (current != NULL)
  {
    // strcmp restituisce:
    //   0 se le stringhe sono uguali
    //   <0 se current->value < value (alfabeticamente)
    //   >0 se current->value > value (alfabeticamente)
    int cmp = strcmp(current->value, value);
    
    // CASO 1: Trovato!
    if (cmp == 0)
    {
      return true;
    }
    
    // OTTIMIZZAZIONE: Se siamo oltre il valore cercato, fermati
    // La lista è ordinata, quindi non lo troveremo più avanti
    // Esempio: cerco "b" ma sono arrivato a "c" -> non serve continuare
    if (cmp > 0)
    {
      return false;
    }
    
    // CASO 3: current->value < value, continua a cercare
    current = current->next;
  }

  // Se siamo arrivati alla fine senza trovare il valore
  return false;
}

/**
 * insertNodeInSortedList - Inserisce un nuovo nodo mantenendo l'ordine
 *
 * @param list La lista in cui inserire
 * @param value Il valore da inserire
 * @returns 0 se successo, -1 in caso di errore
 * 
 * LOGICA DI INSERIMENTO:
 * La lista deve rimanere ORDINATA in ordine ALFABETICO CRESCENTE
 * Esempio: "alice" < "bob" < "charlie" < "david"
 * 
 * CASI DA GESTIRE:
 * 1. Lista vuota → inserisci come primo elemento
 * 2. Valore minore del primo → inserisci in testa
 * 3. Valore nel mezzo → trova la posizione corretta e inserisci
 * 
 * FUNZIONAMENTO DETTAGLIATO:
 * 1. Validazione input e creazione del nuovo nodo
 * 2. Gestione caso lista vuota
 * 3. Gestione caso inserimento in testa
 * 4. Ricerca della posizione corretta (tra due nodi)
 * 5. Inserimento mantenendo i collegamenti
 */
int insertNodeInSortedList(SortedList *list, char *value)
{
    // CONTROLLO 1: Validazione input
    if (list == NULL || value == NULL)
    {
      return -1;
    }

    // STEP 1: Crea il nuovo nodo da inserire
    Node *newNode = createNode(value);
    if (newNode == NULL)
    {
      return -1;  // Errore di allocazione
    }

    // CASO 1: Lista vuota - il nuovo nodo diventa il primo
    if (list->first == NULL)
    {
      list->first = newNode;
      return 0;
    }

    // CASO 2: Inserimento in testa
    // Se il nuovo valore è alfabeticamente minore del primo nodo
    // Esempio: inserisco "alice" in una lista che inizia con "bob"
    if (strcmp(value, list->first->value) < 0)
    {
      newNode->next = list->first;  // Il nuovo nodo punta al vecchio primo
      list->first = newNode;        // Il nuovo nodo diventa il primo
      return 0;
    }

    // CASO 3: Inserimento nel mezzo o in coda
    // Trova la posizione corretta: tra current e current->next
    
    Node *current = list->first;
    
    // Continua finché:
    // - Non hai raggiunto la fine (current->next != NULL)
    // - Il valore da inserire è maggiore del prossimo nodo
    //
    // Esempio: Lista ["alice", "charlie", "david"], inserisco "bob"
    // - current = "alice", next = "charlie"
    // - strcmp("bob", "charlie") < 0 → FERMATI QUI
    // - Inserisci "bob" tra "alice" e "charlie"
    while (current->next != NULL && strcmp(value, current->next->value) > 0)
    {
      current = current->next;
    }

    // STEP 2: Inserisci il nuovo nodo
    // current → newNode → current->next
    newNode->next = current->next;  // newNode punta a dove puntava current
    current->next = newNode;        // current ora punta a newNode

    return 0;
}
/**
 * removeNodeFromSortedList - Rimuove un nodo dalla lista dato l'indice
 *
 * @param list La lista da cui rimuovere il nodo
 * @param index L'indice del nodo da rimuovere (0-based)
 * @returns Il nodo rimosso, o NULL in caso di errore
 * 
 * IMPORTANTE: 
 * - Gli indici partono da 0 (0 = primo nodo, 1 = secondo, ecc.)
 * - Il nodo viene RIMOSSO dalla lista ma NON deallocato
 * - Il chiamante è responsabile di deallocare il nodo restituito
 * 
 * FUNZIONAMENTO:
 * 1. Validazione: controlla input e lista non vuota
 * 2. CASO SPECIALE: Rimozione del primo nodo (index == 0)
 * 3. CASO GENERALE: Naviga fino al nodo precedente a quello da rimuovere
 * 4. Scollega il nodo dalla lista
 * 5. Restituisce il nodo rimosso
 * 
 * ESEMPIO VISIVO:
 * Lista: A → B → C → D → NULL
 * removeNodeFromSortedList(list, 2) rimuove C
 * Risultato: A → B → D → NULL
 * Ritorna: C (con next = NULL)
 */
Node *removeNodeFromSortedList(SortedList *list, int index)
{
  // CONTROLLO 1: Validazione input
  if (list == NULL || index < 0)
  {
    return NULL;  // Errore: lista non valida o indice negativo
  }

  // CONTROLLO 2: Lista vuota
  if (list->first == NULL)
  {
    return NULL;  // Non c'è nulla da rimuovere
  }

  // Variabile per il nodo da rimuovere
  Node *toRemove = NULL;

  // CASO SPECIALE: Rimozione del primo nodo (indice 0)
  if (index == 0)
  {
    toRemove = list->first;           // Salva il riferimento al primo nodo
    list->first = list->first->next;  // Il secondo nodo diventa il primo
    toRemove->next = NULL;            // Scollega il nodo rimosso dalla lista
    return toRemove;
  }

  // CASO GENERALE: Rimozione di un nodo che non è il primo
  
  // STEP 1: Naviga fino al nodo PRECEDENTE a quello da rimuovere
  // Dobbiamo fermarci a index-1 perché dobbiamo modificare il campo 'next'
  // del nodo precedente
  
  Node *current = list->first;
  int currentIndex = 0;

  // Continua finché:
  // - Non hai raggiunto la fine (current->next != NULL)
  // - Non sei arrivato al nodo precedente (currentIndex < index - 1)
  //
  // Esempio: rimuovi indice 3
  // - Devi fermarti all'indice 2 (il nodo precedente)
  // - Così puoi fare: nodo[2]->next = nodo[3]->next
  while (current->next != NULL && currentIndex < index - 1)
  {
    current = current->next;
    currentIndex++;
  }

  // STEP 2: Verifica se l'indice è valido
  // Se current->next è NULL, significa che l'indice è oltre la fine della lista
  // Esempio: lista con 3 elementi [0,1,2], ma chiedi indice 5
  if (current->next == NULL)
  {
    return NULL;  // Indice fuori range
  }

  // STEP 3: Rimuovi il nodo
  // Situazione: current → toRemove → next
  // Vogliamo:   current → next (saltando toRemove)
  
  toRemove = current->next;          // Salva il nodo da rimuovere
  current->next = toRemove->next;    // current salta toRemove e punta direttamente al successivo
  toRemove->next = NULL;             // Scollega toRemove dalla lista

  // STEP 4: Restituisci il nodo rimosso
  // NOTA: Il nodo è ancora allocato in memoria, ma non fa più parte della lista
  // Il chiamante deve deallocarlo con destroyNode() quando ha finito
  return toRemove;
}

/**
 * printSortedList - Stampa tutti i valori della lista, uno per riga
 *
 * @param list La lista da stampare
 * 
 * FUNZIONAMENTO:
 * 1. Validazione: controlla che la lista non sia NULL
 * 2. Se la lista è vuota, non stampa nulla
 * 3. Itera attraverso tutti i nodi stampando ogni valore
 * 
 * FORMATO OUTPUT:
 * Ogni valore viene stampato su una riga separata
 * Esempio per lista ["alice", "bob", "charlie"]:
 * alice
 * bob
 * charlie
 * 
 * COMPLESSITÀ: O(n) - deve visitare tutti i nodi
 */
void printSortedList(SortedList *list)
{
  // CONTROLLO 1: Validazione input
  if (list == NULL)
  {
    return;  // Niente da stampare
  }

  // CONTROLLO 2: Lista vuota
  if (list->first == NULL)
  {
    return;  // Niente da stampare
  }

  // STEP 1: Inizia dal primo nodo
  Node *current = list->first;
  
  // STEP 2: Itera attraverso tutti i nodi e stampa i valori
  while (current != NULL)
  {
    // Stampa il valore del nodo corrente seguito da newline
    printf("%s\n", current->value);
    
    // Passa al nodo successivo
    current = current->next;
  }
}

#endif