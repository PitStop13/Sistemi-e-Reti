n = int(input("Inserire N: "))
lista_numeri = []
for i in range(n):
    lista_numeri.append(
        int(
            input(f"Inserisci il numero {i}: ")
        )
    )

x = int(input("Inserisci numero da cercare: "))
if x in lista_numeri:
    print("Trovato!")
else:
    print("Non c'è")