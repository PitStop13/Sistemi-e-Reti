# definiamo come lambda una funzione mono-uso che posso 
# anche assegnare a una variabile
#
# Per il resto è UGUALE a una funzione definita con def

is_pari = lambda x : x % 2 == 0

def is_pari(x: int) -> bool:
    return x % 2 == 0

print(is_pari(4))

# Il caso di uso più comune è dentro ai costrutti di aggregazione,
# i più comunemente usati (anche se ce ne sono molti altri!!) sono
# - filter
# - map

lista_pari = filter(is_pari, [1, 2, 3, 4, 5])
print(lista_pari)

graaaande_csv = """
nome;cognome;anno_nascita
Aldo;Baglio;01/01/2001
Bruno;Baldoni;02/02/2002
Carlo;Olio;03/03/2003
Mario;Dizio;04/04/2004
"""

split_commas = lambda line: line.split(";")
matr = map(split_commas, graaaande_csv.split("\n"))


lista_diziomario = list[dict[str,str]]([
    {
        "nome": "Aldo",
        "cognome":"Baglio",
        "anno_nascita": "01/01/2001"
    },
    {
        "nome": "█████████",
        "cognome":"█████████",
        "anno_nascita": "███████████"
    },
])

def csv_line(diziomario_singolo: dict[str, str]):
    line = ""
    for value in diziomario_singolo.values():
        line += f"{value};"
    return line[:-1] # tolgo l'ultimo ;

result = map(csv_line, lista_diziomario)
print(list(result))

from functools import reduce

lista_numeri = list[int]([1, 2, 3, 4, 5, 6])

def funzione_di_riduzione(accumulatore: int, elemento: int) -> int:
    return accumulatore + elemento

somma_numeri = reduce(funzione_di_riduzione, lista_numeri, 0)

lista_numeri = list[int]([1, 2, 3, 4, 5, 6])
# "4 16 36"

# step 1: filter
lista_pari = filter(lambda x : x % 2 == 0, lista_numeri)

#step 2: map per quadrato
lista_quadrati = map(lambda x: x ** 2, lista_pari)

# step 3: reduce per stringification
risultato_finale = reduce(
    lambda acc, x: acc + str(x),
    lista_quadrati, 
    ""
)
