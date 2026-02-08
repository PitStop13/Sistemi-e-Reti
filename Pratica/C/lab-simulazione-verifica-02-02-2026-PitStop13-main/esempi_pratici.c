/**
 * ESEMPI PRATICI - Come usare le funzioni delle liste ordinate
 * 
 * Questo file contiene esempi concreti di utilizzo delle funzioni
 * implementate in sorted_list.h e node.h
 */

#include <stdio.h>
#include "node.h"
#include "sorted_list.h"

// =============================================================================
// ESEMPIO 1: Creare e popolare una lista
// =============================================================================
void esempio1_creare_lista() {
    printf("=== ESEMPIO 1: Creare e popolare una lista ===\n\n");
    
    // Step 1: Crea una lista vuota
    SortedList *myList = createEmptySortedList();
    if (myList == NULL) {
        printf("Errore: impossibile creare la lista\n");
        return;
    }
    printf("✓ Lista vuota creata\n");
    
    // Step 2: Inserisci alcuni valori (in ordine casuale)
    printf("\nInserisco i nomi in ordine casuale:\n");
    insertNodeInSortedList(myList, "Charlie");
    printf("  Inserito: Charlie\n");
    
    insertNodeInSortedList(myList, "Alice");
    printf("  Inserito: Alice\n");
    
    insertNodeInSortedList(myList, "Bob");
    printf("  Inserito: Bob\n");
    
    insertNodeInSortedList(myList, "David");
    printf("  Inserito: David\n");
    
    // Step 3: Stampa la lista (sarà ordinata!)
    printf("\nLista ordinata automaticamente:\n");
    printSortedList(myList);
    
    // Step 4: Pulisci la memoria
    destroySortedList(myList);
    printf("\n✓ Memoria deallocata\n");
}

// =============================================================================
// ESEMPIO 2: Clonare una lista
// =============================================================================
void esempio2_clonare_lista() {
    printf("\n=== ESEMPIO 2: Clonare una lista ===\n\n");
    
    // Crea lista originale
    SortedList *original = createEmptySortedList();
    insertNodeInSortedList(original, "Uno");
    insertNodeInSortedList(original, "Due");
    insertNodeInSortedList(original, "Tre");
    
    printf("Lista originale:\n");
    printSortedList(original);
    
    // Clona la lista
    SortedList *clone = cloneSortedList(original);
    
    printf("\nLista clonata:\n");
    printSortedList(clone);
    
    // Modifica l'originale
    insertNodeInSortedList(original, "Quattro");
    
    printf("\nDopo aver aggiunto 'Quattro' all'originale:\n");
    printf("Lista originale:\n");
    printSortedList(original);
    
    printf("\nLista clonata (rimane invariata):\n");
    printSortedList(clone);
    
    // Pulisci
    destroySortedList(original);
    destroySortedList(clone);
}

// =============================================================================
// ESEMPIO 3: Cercare valori nella lista
// =============================================================================
void esempio3_cercare_valori() {
    printf("\n=== ESEMPIO 3: Cercare valori ===\n\n");
    
    // Crea lista con frutta
    SortedList *frutta = createEmptySortedList();
    insertNodeInSortedList(frutta, "Mela");
    insertNodeInSortedList(frutta, "Banana");
    insertNodeInSortedList(frutta, "Arancia");
    insertNodeInSortedList(frutta, "Pera");
    
    printf("Lista della frutta:\n");
    printSortedList(frutta);
    
    // Cerca vari valori
    printf("\nRicerche:\n");
    
    if (sortedListContains(frutta, "Mela")) {
        printf("  ✓ 'Mela' è presente\n");
    }
    
    if (sortedListContains(frutta, "Kiwi")) {
        printf("  ✓ 'Kiwi' è presente\n");
    } else {
        printf("  ✗ 'Kiwi' NON è presente\n");
    }
    
    if (sortedListContains(frutta, "Banana")) {
        printf("  ✓ 'Banana' è presente\n");
    }
    
    // Dimensione
    int size = getSortedListSize(frutta);
    printf("\nLa lista contiene %d elementi\n", size);
    
    destroySortedList(frutta);
}

// =============================================================================
// ESEMPIO 4: Rimuovere nodi
// =============================================================================
void esempio4_rimuovere_nodi() {
    printf("\n=== ESEMPIO 4: Rimuovere nodi ===\n\n");
    
    // Crea lista con numeri
    SortedList *numeri = createEmptySortedList();
    insertNodeInSortedList(numeri, "10");
    insertNodeInSortedList(numeri, "20");
    insertNodeInSortedList(numeri, "30");
    insertNodeInSortedList(numeri, "40");
    insertNodeInSortedList(numeri, "50");
    
    printf("Lista iniziale:\n");
    printSortedList(numeri);
    printf("Dimensione: %d\n", getSortedListSize(numeri));
    
    // Rimuovi il nodo all'indice 2 (terzo elemento)
    printf("\nRimuovo l'elemento all'indice 2:\n");
    Node *removed = removeNodeFromSortedList(numeri, 2);
    if (removed != NULL) {
        printf("  Rimosso: %s\n", removed->value);
        destroyNode(removed);  // IMPORTANTE: dealloca il nodo rimosso!
    }
    
    printf("\nLista dopo la rimozione:\n");
    printSortedList(numeri);
    printf("Dimensione: %d\n", getSortedListSize(numeri));
    
    // Rimuovi il primo elemento
    printf("\nRimuovo il primo elemento (indice 0):\n");
    removed = removeNodeFromSortedList(numeri, 0);
    if (removed != NULL) {
        printf("  Rimosso: %s\n", removed->value);
        destroyNode(removed);
    }
    
    printf("\nLista dopo la rimozione:\n");
    printSortedList(numeri);
    
    destroySortedList(numeri);
}

// =============================================================================
// ESEMPIO 5: Gestione errori
// =============================================================================
void esempio5_gestione_errori() {
    printf("\n=== ESEMPIO 5: Gestione errori ===\n\n");
    
    // Test con NULL
    printf("Test con parametri NULL:\n");
    
    SortedList *list = NULL;
    int size = getSortedListSize(list);
    printf("  getSortedListSize(NULL) = %d (dovrebbe essere -1)\n", size);
    
    bool contains = sortedListContains(list, "test");
    printf("  sortedListContains(NULL, ...) = %s (dovrebbe essere false)\n", 
           contains ? "true" : "false");
    
    int result = insertNodeInSortedList(list, "test");
    printf("  insertNode(NULL, ...) = %d (dovrebbe essere -1)\n", result);
    
    // Test con lista vuota
    printf("\nTest con lista vuota:\n");
    list = createEmptySortedList();
    
    size = getSortedListSize(list);
    printf("  getSortedListSize(lista_vuota) = %d (dovrebbe essere 0)\n", size);
    
    contains = sortedListContains(list, "test");
    printf("  sortedListContains(lista_vuota, ...) = %s (dovrebbe essere false)\n", 
           contains ? "true" : "false");
    
    Node *removed = removeNodeFromSortedList(list, 0);
    printf("  removeNode(lista_vuota, 0) = %s (dovrebbe essere NULL)\n", 
           removed == NULL ? "NULL" : "NON NULL");
    
    destroySortedList(list);
}

// =============================================================================
// ESEMPIO 6: Caso d'uso reale - Rubrica telefonica
// =============================================================================
void esempio6_rubrica() {
    printf("\n=== ESEMPIO 6: Rubrica telefonica ===\n\n");
    
    SortedList *rubrica = createEmptySortedList();
    
    printf("Aggiungo contatti alla rubrica:\n");
    insertNodeInSortedList(rubrica, "Mario Rossi");
    insertNodeInSortedList(rubrica, "Luigi Bianchi");
    insertNodeInSortedList(rubrica, "Anna Verdi");
    insertNodeInSortedList(rubrica, "Giuseppe Neri");
    
    printf("\nRubrica ordinata alfabeticamente:\n");
    printSortedList(rubrica);
    
    printf("\nCerco 'Mario Rossi':\n");
    if (sortedListContains(rubrica, "Mario Rossi")) {
        printf("  ✓ Trovato!\n");
    }
    
    printf("\nCerco 'Paolo Gialli':\n");
    if (!sortedListContains(rubrica, "Paolo Gialli")) {
        printf("  ✗ Non trovato\n");
        printf("  Aggiungo 'Paolo Gialli' alla rubrica...\n");
        insertNodeInSortedList(rubrica, "Paolo Gialli");
    }
    
    printf("\nRubrica aggiornata:\n");
    printSortedList(rubrica);
    
    printf("\nNumero totale di contatti: %d\n", getSortedListSize(rubrica));
    
    destroySortedList(rubrica);
}

// =============================================================================
// ESEMPIO 7: Confronto prestazioni con/senza ottimizzazione
// =============================================================================
void esempio7_ottimizzazione() {
    printf("\n=== ESEMPIO 7: Ottimizzazione della ricerca ===\n\n");
    
    SortedList *lista = createEmptySortedList();
    
    // Crea una lista lunga
    printf("Creando lista con 1000 elementi...\n");
    for (int i = 1; i <= 1000; i++) {
        char buffer[20];
        sprintf(buffer, "Elemento_%04d", i);
        insertNodeInSortedList(lista, buffer);
    }
    
    printf("Lista creata con %d elementi\n", getSortedListSize(lista));
    
    printf("\nCerco 'Elemento_0100' (presente):\n");
    if (sortedListContains(lista, "Elemento_0100")) {
        printf("  ✓ Trovato! (si ferma dopo ~100 confronti)\n");
    }
    
    printf("\nCerco 'Elemento_1500' (NON presente):\n");
    if (!sortedListContains(lista, "Elemento_1500")) {
        printf("  ✗ Non trovato (si ferma dopo ~1000 confronti)\n");
        printf("  SENZA ottimizzazione avrebbe fatto 1000 confronti\n");
        printf("  CON ottimizzazione si ferma appena supera il valore\n");
    }
    
    printf("\nCerco 'Elemento_0001' (primo):\n");
    if (sortedListContains(lista, "Elemento_0001")) {
        printf("  ✓ Trovato subito! (1 solo confronto)\n");
    }
    
    destroySortedList(lista);
}

// =============================================================================
// MAIN: Esegui tutti gli esempi
// =============================================================================
int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  ESEMPI PRATICI - Liste Concatenate Ordinate in C         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    esempio1_creare_lista();
    esempio2_clonare_lista();
    esempio3_cercare_valori();
    esempio4_rimuovere_nodi();
    esempio5_gestione_errori();
    esempio6_rubrica();
    esempio7_ottimizzazione();
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Fine esempi - Tutti i test completati con successo!      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}
