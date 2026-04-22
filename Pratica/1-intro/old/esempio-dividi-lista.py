lista = [ 1, 2, 3, 4, 5 ]

# dividere una lista in 3 (split su dimensione)
divisione_3 = [ 
    lista[0:int(len(lista) / 3)],
    lista[int(len(lista) / 3): 2 * int(len(lista) / 3)],
    lista[2 * int(len(lista) / 3): len(lista)]
]

def dividi_lista(lista, n):
    lista_divisa = []

    grandezza_sotto_lista = int(len(lista) / n)

    for i in range(n):
        lista_divisa.append(
            lista[i * grandezza_sotto_lista: (i+1) * grandezza_sotto_lista]
        )

    return lista_divisa

def ordina_e_dividi_lista(lista, n):
    return dividi_lista(sorted(lista), n)