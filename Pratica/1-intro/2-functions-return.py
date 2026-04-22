# io definisco una funzione con la parola chiave "def"

# None -> null, oppure void, a seconda di dove sta

def funzione_vuota():
    # usiamo pass per uscire da una funzione
    # vuota (si usa per fare le interfacce, vedere OOP)
    pass

def stampa_numero(n: int) -> None:
    """
    Questo è un commento di documentazione. Il linguaggio
    di riferimento è Markdown (.md - quello dei README).

    Stampa un numero passato come parametro.

    Args:
      **n** -> Il numero da stampare
    Example:
    ``` python
    stampa_numero(5)
    ```
    """
    print(n)


n = 5
"""
I commenti di documentazione si possono anche
mettere sulle variabili (sotto).

Una variabile intera che dimostra il funzionamento
dei commenti di documentazione.
"""
