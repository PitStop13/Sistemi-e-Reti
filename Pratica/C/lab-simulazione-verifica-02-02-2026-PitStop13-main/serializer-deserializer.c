/**
 * ============================================================================
 * SERIALIZZAZIONE E DESERIALIZZAZIONE DI LISTE ORDINATE
 * ============================================================================
 * 
 * CONCETTI FONDAMENTALI:
 * 
 * SERIALIZZAZIONE = Convertire una struttura dati in memoria in un formato
 *                   che può essere salvato su file o inviato in rete
 * 
 * DESERIALIZZAZIONE = Ricostruire la struttura dati originale partendo
 *                     dal formato serializzato
 * 
 * Nel nostro caso:
 * - Serializziamo una SortedList in un file di testo
 * - Ogni riga del file contiene il valore di un nodo
 * - La deserializzazione legge il file e ricostruisce la lista
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "sorted_list.h"

// Dimensione massima per le righe lette dal file
#define MAX_LINE_LENGTH 1024

/**
 * serializeSortedList - Salva una lista ordinata su file
 * 
 * @param list La lista da serializzare
 * @param filename Il nome del file dove salvare
 * @returns 0 se successo, -1 in caso di errore
 * 
 * FORMATO DEL FILE:
 * Ogni riga contiene il valore di un nodo, nell'ordine della lista.
 * 
 * Esempio di file generato:
 * Alice
 * Bob
 * Charlie
 * David
 * 
 * FUNZIONAMENTO DETTAGLIATO:
 * 1. Validazione: controlla che list e filename non siano NULL
 * 2. Apertura file in modalità scrittura ("w")
 * 3. Iterazione attraverso tutti i nodi
 * 4. Scrittura di ogni valore su una riga separata
 * 5. Chiusura del file
 * 
 * GESTIONE ERRORI:
 * - Lista NULL → errore -1
 * - Filename NULL → errore -1
 * - Impossibile aprire file → errore -1
 * - Lista vuota → file vuoto (successo 0)
 * 
 * NOTE IMPORTANTI:
 * - Il file viene sovrascritto se esiste già (modalità "w")
 * - I valori vengono scritti uno per riga
 * - Non viene salvata alcuna informazione sulla struttura (è implicita)
 */
int serializeSortedList(SortedList *list, const char *filename)
{
    // ========================================================================
    // STEP 1: VALIDAZIONE INPUT
    // ========================================================================
    
    // Controlla che la lista non sia NULL
    if (list == NULL)
    {
        fprintf(stderr, "Errore: lista NULL in serializeSortedList\n");
        return -1;
    }
    
    // Controlla che il filename non sia NULL
    if (filename == NULL)
    {
        fprintf(stderr, "Errore: filename NULL in serializeSortedList\n");
        return -1;
    }
    
    // ========================================================================
    // STEP 2: APERTURA FILE IN MODALITÀ SCRITTURA
    // ========================================================================
    
    // Apri il file in modalità scrittura ("w")
    // "w" = write mode
    // - Crea il file se non esiste
    // - SOVRASCRIVE il file se esiste già
    // - Permette solo scrittura
    FILE *file = fopen(filename, "w");
    
    // Controlla se l'apertura è riuscita
    if (file == NULL)
    {
        fprintf(stderr, "Errore: impossibile aprire il file '%s' per la scrittura\n", filename);
        return -1;
    }
    
    // ========================================================================
    // STEP 3: CASO SPECIALE - LISTA VUOTA
    // ========================================================================
    
    // Se la lista è vuota, crea un file vuoto e termina con successo
    if (list->first == NULL)
    {
        fclose(file);  // Chiudi il file
        printf("Lista vuota serializzata: file '%s' vuoto creato\n", filename);
        return 0;
    }
    
    // ========================================================================
    // STEP 4: ITERAZIONE E SCRITTURA
    // ========================================================================
    
    // Inizia dal primo nodo
    Node *current = list->first;
    int count = 0;  // Contatore per debug/logging
    
    // Itera attraverso tutti i nodi della lista
    while (current != NULL)
    {
        // Scrivi il valore del nodo corrente nel file
        // fprintf funziona come printf, ma scrive su file
        // Formato: "%s\n" = stringa seguita da newline
        fprintf(file, "%s\n", current->value);
        
        count++;  // Incrementa il contatore
        
        // Passa al nodo successivo
        current = current->next;
    }
    
    // ========================================================================
    // STEP 5: CHIUSURA FILE E CONFERMA
    // ========================================================================
    
    // IMPORTANTE: Chiudi sempre il file dopo averlo usato
    // Questo garantisce che:
    // - I dati vengano effettivamente scritti sul disco (flush del buffer)
    // - Le risorse del sistema operativo vengano liberate
    fclose(file);
    
    // Messaggio di conferma
    printf("Lista serializzata con successo: %d nodi salvati in '%s'\n", count, filename);
    
    return 0;  // Successo
}

/**
 * deserializeSortedList - Carica una lista ordinata da file
 * 
 * @param filename Il nome del file da cui caricare
 * @returns La lista ricostruita, o NULL in caso di errore
 * 
 * FORMATO DEL FILE ATTESO:
 * Ogni riga contiene un valore da inserire nella lista.
 * I valori vengono inseriti mantenendo l'ordine alfabetico.
 * 
 * Esempio di file:
 * Alice
 * Bob
 * Charlie
 * 
 * FUNZIONAMENTO DETTAGLIATO:
 * 1. Validazione: controlla che filename non sia NULL
 * 2. Apertura file in modalità lettura ("r")
 * 3. Creazione di una lista vuota
 * 4. Lettura riga per riga dal file
 * 5. Inserimento di ogni valore nella lista (automaticamente ordinato)
 * 6. Chiusura del file
 * 7. Restituzione della lista ricostruita
 * 
 * GESTIONE ERRORI:
 * - Filename NULL → NULL
 * - File non trovato → NULL
 * - Errore allocazione memoria → NULL (dealloca tutto prima)
 * 
 * NOTE IMPORTANTI:
 * - I valori vengono inseriti con insertNodeInSortedList
 * - Questo garantisce che la lista sia ordinata indipendentemente
 *   dall'ordine delle righe nel file
 * - Gli spazi bianchi alla fine delle righe vengono rimossi
 * - Le righe vuote vengono ignorate
 */
SortedList *deserializeSortedList(const char *filename)
{
    // ========================================================================
    // STEP 1: VALIDAZIONE INPUT
    // ========================================================================
    
    // Controlla che il filename non sia NULL
    if (filename == NULL)
    {
        fprintf(stderr, "Errore: filename NULL in deserializeSortedList\n");
        return NULL;
    }
    
    // ========================================================================
    // STEP 2: APERTURA FILE IN MODALITÀ LETTURA
    // ========================================================================
    
    // Apri il file in modalità lettura ("r")
    // "r" = read mode
    // - Il file DEVE esistere (altrimenti errore)
    // - Permette solo lettura
    FILE *file = fopen(filename, "r");
    
    // Controlla se l'apertura è riuscita
    if (file == NULL)
    {
        fprintf(stderr, "Errore: impossibile aprire il file '%s' per la lettura\n", filename);
        fprintf(stderr, "       Il file potrebbe non esistere o non essere accessibile\n");
        return NULL;
    }
    
    // ========================================================================
    // STEP 3: CREAZIONE LISTA VUOTA
    // ========================================================================
    
    // Crea una nuova lista vuota che verrà popolata con i dati dal file
    SortedList *list = createEmptySortedList();
    
    // Controlla se la creazione è riuscita
    if (list == NULL)
    {
        fprintf(stderr, "Errore: impossibile creare la lista in deserializeSortedList\n");
        fclose(file);  // Chiudi il file prima di uscire
        return NULL;
    }
    
    // ========================================================================
    // STEP 4: LETTURA E INSERIMENTO
    // ========================================================================
    
    // Buffer per leggere le righe dal file
    char buffer[MAX_LINE_LENGTH];
    int count = 0;  // Contatore per debug/logging
    
    // Leggi il file riga per riga
    // fgets legge una riga (fino a \n o EOF) e la salva in buffer
    // Ritorna NULL quando raggiunge la fine del file
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL)
    {
        // PULIZIA DELLA RIGA LETTA:
        // fgets include il carattere newline (\n) nella stringa letta
        // Dobbiamo rimuoverlo insieme ad altri spazi bianchi alla fine
        
        // Trova la lunghezza della stringa
        size_t len = strlen(buffer);
        
        // Rimuovi caratteri di fine riga (\n, \r) e spazi alla fine
        // Parte dalla fine della stringa e torna indietro
        while (len > 0 && (buffer[len-1] == '\n' || 
                          buffer[len-1] == '\r' || 
                          buffer[len-1] == ' ' || 
                          buffer[len-1] == '\t'))
        {
            buffer[len-1] = '\0';  // Sostituisci con terminatore
            len--;                 // Decrementa la lunghezza
        }
        
        // IGNORA RIGHE VUOTE:
        // Se dopo la pulizia la riga è vuota, passa alla successiva
        if (len == 0)
        {
            continue;  // Salta questa iterazione
        }
        
        // INSERIMENTO NELLA LISTA:
        // Inserisci il valore nella lista ordinata
        // insertNodeInSortedList si occupa di mantenere l'ordine
        int result = insertNodeInSortedList(list, buffer);
        
        // Controlla se l'inserimento è riuscito
        if (result == -1)
        {
            fprintf(stderr, "Errore: impossibile inserire '%s' nella lista\n", buffer);
            
            // In caso di errore, dealloca tutto e esci
            destroySortedList(list);
            fclose(file);
            return NULL;
        }
        
        count++;  // Incrementa il contatore
    }
    
    // ========================================================================
    // STEP 5: CHIUSURA FILE E CONFERMA
    // ========================================================================
    
    // IMPORTANTE: Chiudi sempre il file
    fclose(file);
    
    // Messaggio di conferma
    printf("Lista deserializzata con successo: %d nodi caricati da '%s'\n", count, filename);
    
    // ========================================================================
    // STEP 6: RESTITUZIONE DELLA LISTA
    // ========================================================================
    
    return list;  // Restituisci la lista ricostruita
}

/**
 * ============================================================================
 * FUNZIONE DI TEST - Esempio di utilizzo
 * ============================================================================
 */
void esempio_serializzazione()
{
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  ESEMPIO: Serializzazione e Deserializzazione             ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // ========================================================================
    // PARTE 1: Creare e serializzare una lista
    // ========================================================================
    
    printf("--- PARTE 1: Serializzazione ---\n\n");
    
    // Crea una lista con alcuni nomi
    printf("1. Creazione lista originale...\n");
    SortedList *originalList = createEmptySortedList();
    
    insertNodeInSortedList(originalList, "Mario");
    insertNodeInSortedList(originalList, "Luigi");
    insertNodeInSortedList(originalList, "Peach");
    insertNodeInSortedList(originalList, "Bowser");
    insertNodeInSortedList(originalList, "Yoshi");
    
    printf("   Lista creata con %d elementi\n\n", getSortedListSize(originalList));
    
    printf("2. Contenuto lista originale:\n");
    printSortedList(originalList);
    printf("\n");
    
    // Serializza la lista su file
    printf("3. Serializzazione su file 'lista_personaggi.txt'...\n");
    int result = serializeSortedList(originalList, "lista_personaggi.txt");
    
    if (result == 0)
    {
        printf("   ✓ Serializzazione completata!\n\n");
    }
    else
    {
        printf("   ✗ Errore durante la serializzazione\n\n");
        return;
    }
    
    // ========================================================================
    // PARTE 2: Deserializzare la lista
    // ========================================================================
    
    printf("--- PARTE 2: Deserializzazione ---\n\n");
    
    // Carica la lista dal file
    printf("4. Deserializzazione da file 'lista_personaggi.txt'...\n");
    SortedList *loadedList = deserializeSortedList("lista_personaggi.txt");
    
    if (loadedList == NULL)
    {
        printf("   ✗ Errore durante la deserializzazione\n\n");
        destroySortedList(originalList);
        return;
    }
    
    printf("   ✓ Deserializzazione completata!\n\n");
    
    printf("5. Contenuto lista caricata:\n");
    printSortedList(loadedList);
    printf("\n");
    
    // ========================================================================
    // PARTE 3: Verifica che le liste siano identiche
    // ========================================================================
    
    printf("--- PARTE 3: Verifica ---\n\n");
    
    printf("6. Confronto delle due liste...\n");
    
    // Controlla che abbiano la stessa dimensione
    int originalSize = getSortedListSize(originalList);
    int loadedSize = getSortedListSize(loadedList);
    
    if (originalSize != loadedSize)
    {
        printf("   ✗ Le liste hanno dimensioni diverse!\n");
        printf("   Originale: %d, Caricata: %d\n\n", originalSize, loadedSize);
    }
    else
    {
        printf("   ✓ Le liste hanno la stessa dimensione: %d elementi\n\n", originalSize);
        
        // Confronta elemento per elemento
        Node *orig = originalList->first;
        Node *load = loadedList->first;
        bool identical = true;
        int position = 0;
        
        while (orig != NULL && load != NULL)
        {
            if (strcmp(orig->value, load->value) != 0)
            {
                printf("   ✗ Differenza alla posizione %d:\n", position);
                printf("     Originale: '%s'\n", orig->value);
                printf("     Caricata: '%s'\n\n", load->value);
                identical = false;
                break;
            }
            
            orig = orig->next;
            load = load->next;
            position++;
        }
        
        if (identical)
        {
            printf("   ✓ Le liste sono identiche! Tutti i valori corrispondono.\n\n");
        }
    }
    
    // ========================================================================
    // PARTE 4: Pulizia memoria
    // ========================================================================
    
    printf("--- PARTE 4: Pulizia ---\n\n");
    
    printf("7. Deallocazione delle liste...\n");
    destroySortedList(originalList);
    destroySortedList(loadedList);
    printf("   ✓ Memoria deallocata correttamente\n\n");
    
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Test completato con successo!                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
}

/**
 * ============================================================================
 * ESEMPIO 2: Gestione errori
 * ============================================================================
 */
void esempio_gestione_errori_serializzazione()
{
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  ESEMPIO: Gestione Errori                                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Test 1: Serializzare lista NULL\n");
    int result = serializeSortedList(NULL, "test.txt");
    printf("Risultato: %d (dovrebbe essere -1)\n\n", result);
    
    printf("Test 2: Serializzare con filename NULL\n");
    SortedList *list = createEmptySortedList();
    result = serializeSortedList(list, NULL);
    printf("Risultato: %d (dovrebbe essere -1)\n\n", result);
    
    printf("Test 3: Deserializzare file inesistente\n");
    SortedList *loaded = deserializeSortedList("file_che_non_esiste.txt");
    printf("Risultato: %s (dovrebbe essere NULL)\n\n", loaded == NULL ? "NULL" : "NON NULL");
    
    printf("Test 4: Serializzare lista vuota\n");
    result = serializeSortedList(list, "lista_vuota.txt");
    printf("Risultato: %d (dovrebbe essere 0)\n", result);
    
    printf("\nVerifica file creato:\n");
    loaded = deserializeSortedList("lista_vuota.txt");
    if (loaded != NULL)
    {
        printf("Lista caricata con %d elementi (dovrebbe essere 0)\n\n", getSortedListSize(loaded));
        destroySortedList(loaded);
    }
    
    destroySortedList(list);
}

/**
 * ============================================================================
 * MAIN: Esegui gli esempi
 * ============================================================================
 */
int main()
{
    // Esegui l'esempio principale
    esempio_serializzazione();
    
    // Esegui l'esempio di gestione errori
    esempio_gestione_errori_serializzazione();
    
    printf("\n✅ Tutti gli esempi completati!\n\n");
    printf("📁 File creati:\n");
    printf("   - lista_personaggi.txt (esempio principale)\n");
    printf("   - lista_vuota.txt (test lista vuota)\n\n");
    printf("💡 Puoi aprire questi file con un editor di testo per vedere il formato\n\n");
    
    return 0;
}
