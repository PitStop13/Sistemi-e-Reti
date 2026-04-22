# Esercizio 1: Threads e Locks in Python 3

> Tempo per svolgere l'esercizio: 45 minuti

Benvenuti nella clinica "Dove colgo, colgo"! In questo esercizio, implementeremo una semplice simulazione di una clinica con pazienti, dottori e segretarie. L'obiettivo è quello di gestire correttamente l'accesso alle risorse condivise (pazienti) utilizzando i thread e i lock in Python.

Il vostro obiettivo è completare il codice fornito in `main.py` e `clinic.py` per garantire che i pazienti vengano gestiti in modo sicuro, senza conflitti o condizioni di gara (e non rimangano eternamente bloccati in attesa).

> Leggete attentamente i commenti nel codice e seguite le istruzioni per completare l'implementazione. 
>
> Il codice attuale non funziona, va completato per farlo funzionare correttamente.

# Idea generale

- Le segretarie aggiungono i pazienti alla coda.
- I dottori consultano i pazienti in coda uno alla volta.

e così via...

> Ribadiamo che i pazienti sono IN CODA.
>
> Qualsiasi dottore può consultare qualsiasi paziente in coda, ma solo un dottore alla volta può consultare un paziente.
>
> Qualsiasi segretaria può aggiungere un paziente alla coda, ma solo una segretaria alla volta può farlo.