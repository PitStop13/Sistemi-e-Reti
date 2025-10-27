/******************************************************************************

                    RESORT DI LUSSO - GESTIONE COMPLETA
            Camere → Ospiti (con VIP) → Lettura da file → Camere di lusso

    Questo programma simula la gestione di un resort di lusso:
    - Ospiti con nome, cognome e stato VIP
    - Camere con numero, piano e lista di ospiti
    - Resort con nome e lista di camere
    - Conta le "camere di lusso" (≥3 ospiti VIP)
    - Legge i dati da un file di testo

*******************************************************************************/

#include <stdio.h>      // Per printf, scanf, fopen, fprintf, ecc.
#include <stdlib.h>     // Per malloc, free, exit
#include <stdbool.h>    // Per tipo bool (true/false)
#include <string.h>     // Per strlen, strcpy, strdup

#define MAX_LENGTH 100      // Lunghezza massima stringhe (nomi, file)
#define LUXURY_THRESHOLD 3  // Una camera è "lusso" se ha almeno 3 VIP

// ============================================================================
//                              STRUTTURE DATI
// ============================================================================

/**
 * Struct: Guest
 * Rappresenta un ospite del resort
 * - first_name: nome (es. "Mario")
 * - last_name:  cognome (es. "Rossi")
 * - vip:        true se è un cliente VIP, false altrimenti
 */
typedef struct {
    char *first_name;
    char *last_name;
    bool vip;
} Guest;

/**
 * Struct: Room
 * Rappresenta una camera del resort
 * - number:        numero camera (es. 101, 202)
 * - floor:         piano (es. 1, 2)
 * - total_guests:  numero totale di ospiti nella camera
 * - guests:        array dinamico di ospiti (Guest*)
 */
typedef struct {
    int number;
    int floor;
    int total_guests;
    Guest *guests;
} Room;

/**
 * Struct: Resort
 * Rappresenta l'intero resort
 * - name:         nome del resort (es. "ParadiseBay")
 * - total_rooms:  numero totale di camere
 * - rooms:        array dinamico di camere (Room*)
 */
typedef struct {
    char *name;
    int total_rooms;
    Room *rooms;
} Resort;

// ============================================================================
//                         FUNZIONI DI SUPPORTO
// ============================================================================

/**
 * safe_strdup
 * Copia una stringa in memoria dinamica in modo sicuro
 * @param s: stringa da copiare
 * @return: puntatore alla nuova stringa, NULL se errore
 * 
 * Usa malloc per allocare esattamente la memoria necessaria (+1 per '\0')
 */
char *safe_strdup(const char *s) {
    if (!s) return NULL;                    // Se input è NULL, ritorna NULL
    size_t len = strlen(s);                 // Calcola lunghezza
    char *p = (char *)malloc(len + 1);      // Alloca memoria (len + terminatore)
    if (!p) return NULL;                    // Se malloc fallisce
    strcpy(p, s);                           // Copia la stringa
    return p;                               // Ritorna puntatore
}

// ============================================================================
//                         FUNZIONI OSPITE (Guest)
// ============================================================================

/**
 * createGuest
 * Crea un nuovo ospite con nome e cognome
 * @param first_name: nome
 * @param last_name:  cognome
 * @return: puntatore al nuovo Guest, NULL se errore
 * 
 * Alloca memoria per:
 * - la struct Guest
 * - le stringhe first_name e last_name (copia profonda)
 */
Guest *createGuest(const char *first_name, const char *last_name) {
    // Controlla input non valido
    if (!first_name || strlen(first_name) == 0 || !last_name || strlen(last_name) == 0) {
        return NULL;
    }

    Guest *g = (Guest *)malloc(sizeof(Guest));  // Alloca struct
    if (!g) return NULL;                        // Errore malloc

    // Copia nome
    g->first_name = safe_strdup(first_name);
    if (!g->first_name) { free(g); return NULL; }

    // Copia cognome
    g->last_name = safe_strdup(last_name);
    if (!g->last_name) { free(g->first_name); free(g); return NULL; }

    g->vip = false;  // Nuovo ospite non è VIP
    return g;
}

/**
 * makeVip
 * Rende un ospite VIP
 * @param guest: puntatore all'ospite
 * @return: 0 se successo, -1 se errore
 */
int makeVip(Guest *guest) {
    if (!guest) return -1;      // Controllo sicurezza
    guest->vip = true;          // Imposta VIP
    return 0;
}

/**
 * destroyGuest
 * Libera tutta la memoria di un ospite
 * @param guest: puntatore all'ospite
 * @return: 0 se successo, -1 se errore
 */
int destroyGuest(Guest *guest) {
    if (!guest) return -1;
    free(guest->first_name);    // Libera nome
    free(guest->last_name);     // Libera cognome
    free(guest);                // Libera struct
    return 0;
}

// ============================================================================
//                         FUNZIONI CAMERA (Room)
// ============================================================================

/**
 * createRoom
 * Crea una nuova camera con numero, piano e lista ospiti
 * @param number:        numero camera
 * @param floor:         piano
 * @param guests:        array di ospiti da copiare
 * @param guests_count:  numero ospiti
 * @return: puntatore alla nuova Room, NULL se errore
 * 
 * Fa una COPIA PROFONDA di tutti gli ospiti
 */
Room *createRoom(int number, int floor, Guest *guests, int guests_count) {
    // Controlli input
    if (number <= 0 || floor < 0 || !guests || guests_count <= 0) {
        return NULL;
    }

    Room *r = (Room *)malloc(sizeof(Room));     // Alloca struct Room
    if (!r) return NULL;

    // Imposta campi base
    r->number = number;
    r->floor = floor;
    r->total_guests = guests_count;

    // Alloca array di ospiti
    r->guests = (Guest *)malloc(sizeof(Guest) * guests_count);
    if (!r->guests) { free(r); return NULL; }

    // COPIA PROFONDA di ogni ospite
    for (int i = 0; i < guests_count; i++) {
        Guest *src = &guests[i];        // Ospite originale
        Guest *dst = &r->guests[i];     // Ospite destinazione

        dst->first_name = safe_strdup(src->first_name);
        if (!dst->first_name) goto cleanup_room;

        dst->last_name = safe_strdup(src->last_name);
        if (!dst->last_name) { free(dst->first_name); goto cleanup_room; }

        dst->vip = src->vip;  // Copia stato VIP
    }

    return r;  // Tutto ok

cleanup_room:
    // In caso di errore, libera tutto ciò che è stato allocato
    for (int i = 0; i < guests_count; i++) {
        free(r->guests[i].first_name);
        free(r->guests[i].last_name);
    }
    free(r->guests);
    free(r);
    return NULL;
}

/**
 * getVipGuestsCount
 * Conta quanti ospiti VIP ci sono in una camera
 * @param room: puntatore alla camera
 * @return: numero VIP, -1 se errore
 */
int getVipGuestsCount(const Room *room) {
    if (!room || !room->guests || room->total_guests < 0) {
        return -1;
    }

    int count = 0;
    for (int i = 0; i < room->total_guests; i++) {
        if (room->guests[i].vip) {
            count++;
        }
    }
    return count;
}

/**
 * destroyRoom
 * Libera tutta la memoria di una camera
 * @param room: puntatore alla camera
 * @return: 0 se successo, -1 se errore
 */
int destroyRoom(Room *room) {
    if (!room) return -1;

    // Libera ogni ospite
    if (room->guests) {
        for (int i = 0; i < room->total_guests; i++) {
            destroyGuest(&room->guests[i]);
        }
        free(room->guests);  // Libera array ospiti
    }
    free(room);  // Libera struct Room
    return 0;
}

// ============================================================================
//                         FUNZIONI RESORT
// ============================================================================

/**
 * createResort
 * Crea un nuovo resort con nome e lista camere
 * @param name:        nome resort
 * @param rooms:       array di camere da copiare
 * @param rooms_count: numero camere
 * @return: puntatore al nuovo Resort, NULL se errore
 * 
 * Fa COPIA PROFONDA di tutte le camere
 */
Resort *createResort(const char *name, Room *rooms, int rooms_count) {
    if (!name || strlen(name) == 0 || !rooms || rooms_count <= 0) {
        return NULL;
    }

    Resort *resort = (Resort *)malloc(sizeof(Resort));
    if (!resort) return NULL;

    resort->name = safe_strdup(name);
    if (!resort->name) { free(resort); return NULL; }

    resort->total_rooms = rooms_count;
    resort->rooms = (Room *)malloc(sizeof(Room) * rooms_count);
    if (!resort->rooms) { free(resort->name); free(resort); return NULL; }

    // Copia ogni camera
    for (int i = 0; i < rooms_count; i++) {
        Room *src = &rooms[i];
        Room *dst = createRoom(src->number, src->floor, src->guests, src->total_guests);
        if (!dst) goto cleanup_resort;

        resort->rooms[i] = *dst;  // Copia struct
        free(dst);  // Non serve più (già copiato)
    }

    return resort;

cleanup_resort:
    // Pulizia in caso di errore
    for (int i = 0; i < rooms_count; i++) {
        if (resort->rooms[i].guests) {
            destroyRoom(&resort->rooms[i]);
        }
    }
    free(resort->rooms);
    free(resort->name);
    free(resort);
    return NULL;
}

/**
 * getLuxuryRoomsCount
 * Conta quante camere sono "di lusso" (≥3 VIP)
 * @param resort: puntatore al resort
 * @return: numero camere lusso, -1 se errore
 */
int getLuxuryRoomsCount(const Resort *resort) {
    if (!resort || !resort->rooms || resort->total_rooms <= 0) {
        return -1;
    }

    int luxury = 0;
    for (int i = 0; i < resort->total_rooms; i++) {
        int vip_count = getVipGuestsCount(&resort->rooms[i]);
        if (vip_count == -1) return -1;
        if (vip_count >= LUXURY_THRESHOLD) {
            luxury++;
        }
    }
    return luxury;
}

/**
 * destroyResort
 * Libera tutta la memoria del resort
 * @param resort: puntatore al resort
 * @return: 0 se successo, -1 se errore
 */
int destroyResort(Resort *resort) {
    if (!resort) return -1;

    // Libera ogni camera
    if (resort->rooms) {
        for (int i = 0; i < resort->total_rooms; i++) {
            destroyRoom(&resort->rooms[i]);
        }
        free(resort->rooms);
    }
    free(resort->name);  // Libera nome
    free(resort);        // Libera struct
    return 0;
}

// ============================================================================
//                         LETTURA DA FILE
// ============================================================================

/**
 * createResortFromFile
 * Legge i dati del resort da un file di testo
 * @param fp: puntatore al file aperto in lettura
 * @return: puntatore al Resort, NULL se errore
 * 
 * Formato file atteso:
 *   ParadiseBay 2
 *   101 1 3
 *   Mario Rossi
 *   Luigi Verdi
 *   Anna Bianchi
 *   202 2 4
 *   Giulia Neri
 *   ...
 */
Resort *createResortFromFile(FILE *fp) {
    if (!fp) return NULL;

    Resort *resort = (Resort *)malloc(sizeof(Resort));
    if (!resort) return NULL;

    char resort_name[MAX_LENGTH];
    int total_rooms;

    // Prima riga: nome resort + numero camere
    if (fscanf(fp, "%99s %d", resort_name, &total_rooms) != 2 || total_rooms <= 0) {
        free(resort);
        return NULL;
    }

    resort->name = safe_strdup(resort_name);
    if (!resort->name) { free(resort); return NULL; }

    resort->total_rooms = total_rooms;
    resort->rooms = (Room *)malloc(sizeof(Room) * total_rooms);
    if (!resort->rooms) { free(resort->name); free(resort); return NULL; }

    // Per ogni camera
    for (int r = 0; r < total_rooms; r++) {
        int room_number, floor, total_guests;

        // Leggi: numero, piano, numero_ospiti
        if (fscanf(fp, "%d %d %d", &room_number, &floor, &total_guests) != 3 ||
            room_number <= 0 || floor < 0 || total_guests < 0) {
            goto cleanup;
        }

        Room *room = &resort->rooms[r];
        room->number = room_number;
        room->floor = floor;
        room->total_guests = total_guests;
        room->guests = (Guest *)malloc(sizeof(Guest) * total_guests);
        if (!room->guests) goto cleanup;

        // Leggi ogni ospite
        for (int g = 0; g < total_guests; g++) {
            char first[MAX_LENGTH], last[MAX_LENGTH];
            if (fscanf(fp, "%99s %99s", first, last) != 2) {
                goto cleanup;
            }

            Guest *guest = createGuest(first, last);
            if (!guest) goto cleanup;

            room->guests[g] = *guest;  // Copia struct
            free(guest);               // Non serve più
        }
    }

    return resort;

cleanup:
    // Pulizia in caso di errore
    for (int r = 0; r < total_rooms; r++) {
        destroyRoom(&resort->rooms[r]);
    }
    free(resort->rooms);
    free(resort->name);
    free(resort);
    return NULL;
}

// ============================================================================
//                                MAIN
// ============================================================================

/**
 * main
 * Funzione principale: crea file di test, legge, modifica, stampa, pulisce
 */
int main() {
    // === 1. CREA FILE DI TEST ===
    FILE *fp = fopen("resort.txt", "w");
    if (!fp) {
        perror("Impossibile creare file");
        return 1;
    }

    // Scrive dati di esempio
    fprintf(fp, "ParadiseBay 2\n");
    fprintf(fp, "101 1 3\n");
    fprintf(fp, "Mario Rossi\n");
    fprintf(fp, "Luigi Verdi\n");
    fprintf(fp, "Anna Bianchi\n");
    fprintf(fp, "202 2 4\n");
    fprintf(fp, "Giulia Neri\n");
    fprintf(fp, "Paolo Gialli\n");
    fprintf(fp, "Sara Blu\n");
    fprintf(fp, "Luca Arancio\n");
    fclose(fp);

    // === 2. LEGGE DA FILE ===
    fp = fopen("resort.txt", "r");
    if (!fp) {
        perror("Impossibile aprire file");
        return 1;
    }

    Resort *resort = createResortFromFile(fp);
    fclose(fp);

    if (!resort) {
        printf("Errore nella creazione del resort!\n");
        return 1;
    }

    // === 3. SIMULA VIP (rendi 3 ospiti VIP nella camera 202) ===
    if (resort->total_rooms >= 2) {
        Room *luxury_room = &resort->rooms[1];  // Seconda camera
        for (int i = 0; i < 3 && i < luxury_room->total_guests; i++) {
            makeVip(&luxury_room->guests[i]);
        }
    }

    // === 4. STAMPA RISULTATI ===
    printf("=== RESORT: %s ===\n", resort->name);
    printf("Totale camere: %d\n\n", resort->total_rooms);

    for (int i = 0; i < resort->total_rooms; i++) {
        Room *r = &resort->rooms[i];
        int vip_count = getVipGuestsCount(r);
        printf("Camera %d (piano %d)\n", r->number, r->floor);
        printf("  Ospiti: %d | VIP: %d%s\n", r->total_guests, vip_count,
               (vip_count >= LUXURY_THRESHOLD) ? " [LUSSO]" : "");
        for (int g = 0; g < r->total_guests; g++) {
            printf("    - %s %s%s\n", r->guests[g].first_name, r->guests[g].last_name,
                   r->guests[g].vip ? " (VIP)" : "");
        }
        printf("\n");
    }

    int luxury = getLuxuryRoomsCount(resort);
    printf("Camere di lusso (>= %d VIP): %d\n", LUXURY_THRESHOLD, luxury);

    // === 5. PULIZIA MEMORIA ===
    destroyResort(resort);
    printf("\nMemoria liberata correttamente.\n");

    return 0;
}
