#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 100
#define MIN_RANDOM 1
#define MAX_RANDOM 100

typedef struct
{
  char *name;
  int numbers_count;
  int *sorted_numbers;
} Player;

// Funzione di confronto per qsort
int compare(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}

Player *createPlayerFromInput()
{
  char buffer[MAX_LENGTH];
  printf("Inserisci il nome del giocatore: ");
  scanf("%s", buffer);
  if (strlen(buffer) == 0)
  {
    return NULL;
  }

  int estrazioni;
  printf("Inserisci il numero di estrazioni da fare: ");
  scanf("%d", &estrazioni);
  if (estrazioni <= 0)
  {
    return NULL;
  }

  int *numeri = (int *)malloc(sizeof(int) * estrazioni);
  if (numeri == NULL)
  {
    return NULL;
  }

  // Genera numeri nel range corretto [MIN_RANDOM, MAX_RANDOM]
  for (int i = 0; i < estrazioni; i++)
  {
    numeri[i] = MIN_RANDOM + rand() % (MAX_RANDOM - MIN_RANDOM + 1);
  }

  // Ordina i numeri
  qsort(numeri, estrazioni, sizeof(int), compare);

  Player *p = (Player *)malloc(sizeof(Player));
  if (p == NULL)
  {
    free(numeri);
    return NULL;
  }

  p->name = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
  if (p->name == NULL)
  {
    free(numeri);
    free(p);
    return NULL;
  }
  strcpy(p->name, buffer);

  p->numbers_count = estrazioni;
  p->sorted_numbers = numeri;

  return p;
}

int destroyPlayer(Player **player)
{
  // ORDINE CORRETTO: prima controlla player, poi *player
  if (player == NULL || *player == NULL)
  {
    return -1;
  }

  // Libera il nome (se esiste)
  if ((*player)->name != NULL)
  {
    free((*player)->name);
  }

  // Libera l'array di numeri (se esiste)
  if ((*player)->sorted_numbers != NULL)
  {
    free((*player)->sorted_numbers);
  }

  // Libera la struttura Player
  free(*player);

  // Imposta il puntatore a NULL
  *player = NULL;

  return 0;
}

int main()
{
  srand(time(NULL)); // Inizializza il generatore random

  Player *p = createPlayerFromInput();

  if (p == NULL)
  {
    printf("Errore nella creazione del giocatore!\n");
    return 1;
  }

  // Stampa i risultati
  printf("\nGiocatore: %s\n", p->name);
  printf("Numeri estratti e ordinati (%d): ", p->numbers_count);
  for (int i = 0; i < p->numbers_count; i++)
  {
    printf("%d ", p->sorted_numbers[i]);
  }
  printf("\n");

  // Libera la memoria
  if (destroyPlayer(&p) == 0)
  {
    printf("Memoria liberata correttamente!\n");
  }
  else
  {
    printf("Errore nella liberazione!\n");
  }

  return 0;
}
