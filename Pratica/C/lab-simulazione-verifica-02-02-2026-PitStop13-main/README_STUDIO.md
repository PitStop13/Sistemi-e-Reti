# 📦 MATERIALE DI STUDIO - Liste Concatenate Ordinate

## 📚 File Creati

### 1. **node_commented.h**
Header file con implementazione completa delle funzioni per gestire i singoli nodi.
Include commenti dettagliati su:
- `strdupp()` - Duplicazione sicura delle stringhe
- `createNode()` - Creazione di un nuovo nodo
- `cloneNode()` - Clonazione di un nodo
- `destroyNode()` - Deallocazione di un nodo

**Concetti chiave**: Gestione memoria, duplicazione stringhe, allocazione/deallocazione

---

### 2. **sorted_list_commented.h**
Header file con tutte le funzioni per gestire liste ordinate, con commenti approfonditi:
- `createEmptySortedList()` - Creazione lista vuota
- `cloneSortedList()` - Clonazione intera lista
- `destroySortedList()` - Deallocazione lista e tutti i nodi
- `getSortedListSize()` - Calcolo dimensione
- `sortedListContains()` - Ricerca ottimizzata
- `insertNodeInSortedList()` - Inserimento mantenendo ordine
- `removeNodeFromSortedList()` - Rimozione per indice
- `printSortedList()` - Stampa contenuto

**Concetti chiave**: Algoritmi di inserimento ordinato, ottimizzazioni, navigazione liste

---

### 3. **GUIDA_COMPLETA.md**
Guida teorica completa che include:
- ✅ Concetti fondamentali delle liste concatenate
- ✅ Differenza tra lista normale e ordinata
- ✅ 5 concetti chiave da ricordare (memoria, stringhe, deallocazione, ecc.)
- ✅ Algoritmi fondamentali spiegati passo-passo
- ✅ Pattern comuni di programmazione
- ✅ Errori comuni da evitare (con esempi)
- ✅ Tabella complessità temporale
- ✅ Checklist per ogni funzione
- ✅ Esercizi consigliati

**Perfetto per**: Studio teorico, ripasso pre-verifica, capire i concetti

---

### 4. **esempi_pratici.c**
File C eseguibile con 7 esempi pratici commentati:
1. **Creare e popolare una lista** - Operazioni base
2. **Clonare una lista** - Indipendenza delle copie
3. **Cercare valori** - Uso di contains e getSize
4. **Rimuovere nodi** - Rimozione da varie posizioni
5. **Gestione errori** - Test con NULL e liste vuote
6. **Rubrica telefonica** - Caso d'uso reale
7. **Ottimizzazione ricerca** - Confronto prestazioni

**Perfetto per**: Vedere il codice in azione, capire l'uso pratico

---

## 🎯 Come Usare Questo Materiale

### Per Studiare la Teoria
1. Leggi **GUIDA_COMPLETA.md** dall'inizio alla fine
2. Focalizzati sui "Concetti Chiave da Ricordare"
3. Studia gli algoritmi visualizzati
4. Memorizza gli errori comuni

### Per Capire il Codice
1. Apri **node_commented.h** e leggi i commenti
2. Segui la logica di ogni funzione
3. Apri **sorted_list_commented.h** e studia le implementazioni
4. Nota i pattern che si ripetono (es. controlli NULL)

### Per Praticare
1. Compila ed esegui **esempi_pratici.c**
2. Modifica gli esempi e sperimenta
3. Prova a scrivere le tue funzioni
4. Usa la checklist della guida per verificare il tuo codice

### Per la Verifica
1. **1 giorno prima**: Rileggi GUIDA_COMPLETA.md, sezione "Errori Comuni"
2. **La mattina**: Ripassa gli algoritmi principali (inserimento, rimozione)
3. **Durante**: Usa la checklist mentalmente per ogni funzione

---

## 📋 Checklist Pre-Verifica

Prima della verifica, assicurati di saper:

- [ ] Spiegare cos'è una lista concatenata
- [ ] Differenziare lista ordinata da lista normale
- [ ] Usare correttamente `malloc` e `free`
- [ ] Duplicare stringhe con `strdupp` o equivalente
- [ ] Usare `strcmp` per confrontare stringhe
- [ ] Implementare inserimento in ordine alfabetico
- [ ] Rimuovere nodi senza perdere riferimenti
- [ ] Gestire casi NULL e lista vuota
- [ ] Iterare una lista con while e puntatori
- [ ] Deallocare correttamente (prima contenuto, poi contenitore)

---

## 🔍 Domande Frequenti

### D: Perché duplicare le stringhe?
**R**: Per evitare che modifiche/deallocazioni esterne influenzino i nodi della lista.

### D: Quando uso strcmp?
**R**: SEMPRE quando devi confrontare il contenuto di due stringhe. Mai usare `==`.

### D: Come gestisco la memoria?
**R**: Chi alloca con `malloc`, deve deallocare con `free`. Ordine: prima il contenuto, poi il contenitore.

### D: Cosa succede se dimentico free?
**R**: Memory leak - la memoria resta allocata ma inutilizzabile.

### D: Perché fermarsi a index-1 nella rimozione?
**R**: Perché devi modificare il campo `next` del nodo PRECEDENTE.

### D: Come ottimizzare la ricerca?
**R**: In una lista ordinata, fermati appena superi il valore cercato.

---

## 💡 Suggerimenti Finali

1. **Disegna sempre**: Le liste sono strutture visive, disegnarle aiuta enormemente
2. **Testa i limiti**: NULL, vuoto, un elemento - sono i casi che rivelano bug
3. **Leggi i commenti**: Ogni commento spiega il PERCHÉ, non solo il COME
4. **Prova gli esempi**: Compila esempi_pratici.c e osserva l'output
5. **Non memorizzare**: Capisci la logica, il codice viene di conseguenza

---

## 🚀 Risorse Utili

### Visualizzatori Online
- [VisuAlgo](https://visualgo.net/en/list) - Visualizza operazioni su liste
- [Python Tutor](http://pythontutor.com/) - Esegui codice C passo-passo

### Per Approfondire
- Prova a implementare: merge di due liste ordinate
- Esercizio: invertire una lista ordinata
- Sfida: implementare una lista doppiamente concatenata

---

## ✅ Verifica la Tua Comprensione

Prova a rispondere senza guardare il codice:

1. Come inserisci un nodo in una lista ordinata?
2. Cosa succede se non duplichi le stringhe?
3. Perché devi salvare `next` prima di deallocare un nodo?
4. Come ottimizzi la ricerca in una lista ordinata?
5. Qual è la differenza tra `cloneNode` e `cloneSortedList`?

Se sai rispondere a queste domande, sei pronto! 🎓

---

**Buona fortuna per la verifica! 🍀**

*Ricorda: la pratica rende perfetti. Più scrivi codice, più diventa naturale.*
