# le liste in Python sono "liste modificabili
# (non-immutabili) di elementi"
# 
# Si possono inizializzare in 2/3 modi
lista = []
# oppure
lista = list()
# oppure, se vogliamo essere specifici
lista = list[int]()

# una lista può essere inizializzata anche con elementi
lista = [1, 2, 3] # generico
lista = list[int]([1, 2, 3]) #specifico

lista = list([1, 2, 3]) # ridondante, poco usato

# per scorrere una lista usiamo "for"
for elemento in lista:
    print(elemento)

# Accesso 
print(lista[0])

# è possibile ottenere sotto-liste da liste
sotto_lista = lista[0:1] # [1]
sotto_lista = lista[0:2] # [1, 2]
sotto_lista = lista[2:len(lista)] # [3]

sotto_lista = lista[0:len(lista):2] # [1, 3]
sotto_lista = lista[0:len(lista):2] # [1, 3]
sotto_lista = lista[0::2] # [1, 3]
sotto_lista = lista[::2] # [1, 3]

# Indici negativi -> parte dalla fine
sotto_lista = lista[-2:] # [2, 3]
sotto_lista = lista[:-2] # [1]

# Posso clonare una lista in questo modo
lista_clone = lista[0:len(lista)] # [1, 2, 3]
lista_clone = lista[:] # [1, 2, 3]

# Operazioni comuni sulle liste
lista_ordinata = sorted(lista)
lista_invertita = reversed(lista)
lista_con_aggiunta = lista.append(1)

def numeri_pari():
    """
    Crea un generatore di numeri pari, a partire
    da zero.

    Example:
    ``` python
    generatore = numeri_pari()
    print(next(generatore))
    ```
    """
    start = 0
    while True:
        # Permette la creazione di un generatore.
        # Si usa al posto del return per bloccare 
        # lo stato della funzione e proseguire da 
        # qui alla chiamata successiva.
        # Ottimo per iterazioni finite in liste
        # infinite.
        yield start 
            
        start = start + 2

generatore_numeri_pari = numeri_pari()

print(next(generatore_numeri_pari))
print(next(generatore_numeri_pari))
print(next(generatore_numeri_pari))
print(next(generatore_numeri_pari))