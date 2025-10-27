# Esercitazione 3 - Puntatori e Allocazione Dinamica in C

![Soccer Team Pic](.lesson/assets/football.png)

In questa esercitazione occorre implementare tutte le funzioni definite nel file `football.h`.

Si ricorda che non si possono usare indici, ma occorre sfruttare l'aritmetica dei puntatori.

> Il file `testing.h` è una libreria necessaria per i test, non dovete toccarla.
>
> Questo progetto è compatibile sia con l'IDE CLion che con l'IDE Code::Blocks
>
> Per usare questo progetto offline, naturalmente dovete prima scaricarlo
>
> Se usate CLion, aprite il file `Makefile` con Clion (Apri con -> Clion)
>
> Se usate Code::Blocks, aprite il file con estensione `.cbp`

L'unico file che verrà corretto è il file `football.h`

Le funzioni da implementare sono di 5 tipi su 2 entità:

- costruzione di entità (ovvero la creazione di entità a partire da certi parametri)
- clonazione di entità (ovvero la creazione di una nuova entitò a partire da un'altra)
- distruzione di entità (ovvero la deallocazione completa di una entità)
- serializzazione di entità (ovvero il salvataggio su file seguendo un certo formato)
- deserializzazione di entità (ovvero il caricamento da file formattato secondo un certo formato)

Qui c'è l'elenco completo dei test:

```
createPlayer(...)
  createPlayer(NULL, -1)
  createPlayer("Testing", -1)
  createPlayer("", 0)
  createPlayer("Testing", 0)
  createPlayer("Testing", 10)
  createPlayer("Testing", 10) - allocation
  createPlayer("Testing", 10) - name
  createPlayer("Testing", 10) - name (allocation)
  createPlayer("Testing", 10) - number

clonePlayer(...)
  clonePlayer(NULL)
  clonePlayer(player)
  clonePlayer(player) - allocation
  clonePlayer(player) - name
  clonePlayer(player) - name (allocation)
  clonePlayer(player) - number

destroyPlayer(...)
  destroyPlayer(NULL)
  destroyPlayer(player)
  destroyPlayer(player) - de-allocation

serializePlayer(...)
  (before)Test file must be opened for tests to work
  serializePlayer(NULL, NULL)
  serializePlayer(player, NULL)
  serializePlayer(NULL, fp)
  serializePlayer(player, fp)
  serializePlayer(player, fp) - file contents

deserializePlayer(...)
  deserializePlayer(NULL)
  deserializePlayer(fp) - Test file 1
  deserializePlayer(fp) - Test file 2
  deserializePlayer(fp) - Test file 3
  deserializePlayer(fp) - Test file 3 - allocation
  deserializePlayer(fp) - Test file 3 - name
  deserializePlayer(fp) - Test file 3 - name - allocation
  deserializePlayer(fp) - Test file 3 - number

createTeam(...)
  createTeam(NULL, NULL, -1)
  createTeam("", NULL, -1)
  createTeam("Testing", NULL, -1)
  createTeam(NULL, members, -1)
  createTeam(NULL, members, 0)
  createTeam(NULL, members, 1)
  createTeam("", members, -1)
  createTeam("Testing Team", members, 1)
  createTeam("Testing Team", members, 1) - allocation
  createTeam("Testing Team", members, 1) - name
  createTeam("Testing Team", members, 1) - name - allocation
  createTeam("Testing Team", members, 1) - members_count
  createTeam("Testing Team", members, 1) - members[0] check

cloneTeam(...)
  cloneTeam(NULL)
  cloneTeam(team)
  cloneTeam(team) - allocation
  cloneTeam(team) - name - allocation
  cloneTeam(team) - members - allocation
  cloneTeam(team) - name check
  cloneTeam(team) - members_count check
  cloneTeam(team) - members[0] check

destroyTeam(...)
  destroyTeam(NULL)
  destroyTeam(team)
  destroyTeam(team) - de-allocation

serializeTeam(...)
  serializeTeam(NULL, NULL)
  serializeTeam(team, NULL)
  serializeTeam(NULL, fp)
  serializeTeam(team, fp)
  serializeTeam(team, fp) - file contents

deserializeTeam(...)
  deserializeTeam(NULL)
  deserializeTeam(fp) - Test file 1
  deserializeTeam(fp) - Test file 2
  deserializeTeam(fp) - Test file 3
  deserializeTeam(fp) - Test file 4
  deserializeTeam(fp) - Test file 4 - allocation
  deserializeTeam(fp) - Test file 4 - name - allocation
  deserializeTeam(fp) - Test file 4 - contents
```

## FAQ

### Non vedo tutti i test, come mai?

I test compariranno man mano che implementate correttamente le funzioni.

### Quale è il formato dei file per la serializzazione/deserializzazione?

Avete dei file di esempio sia per `Player` che per `Team`

### Posso modificare altri file a parte `football.h`?

No