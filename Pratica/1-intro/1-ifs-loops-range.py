# In Python si usa l'indentazione per i blocchi
# di codice.

n = int(input("Inserisci un numero intero: "))

if n < 10:
    print(f"{n} è minore di 10")
elif n < 20:
    print(f"{n} è minore di 20")
else:
    print(f"{n} è maggiore o uguale a 20")

for i in range(5):
    # stampa ogni numero da 0 a 4, 
    # uno per riga
    print(i)

# Il range si può creare in 3 modi
# range(min_incluso = 0, max_escluso, step = 1)
# range(max_escluso)
#    -> range(0, max_escluso, 1)
# range(min_incluso, max_escluso)
#    -> range(min_incluso, max_escluso, 1)
# Il range può anche andare al contrario
# range(10, 0, -1) # da 10 da 1, step -1

# I range si possono assegnare a delle variabili 
# e riutilizzare
ints_less_20 = range(20)
for i in ints_less_20:
    print(i)
for i in ints_less_20:
    print(i)

# In Python non esiste il ciclo do...while
# Si usa solo while

i = 0
print(i)
while i < 20:
    print(i)
    i = i + 1

# for + break + else
for i in range(10):
    # in questo caso andrò sempre 
    # nel ramo else
    if i == 11:
        print("Trovato")
        break
# se non vado mai a finire nel ramo break
# il ramo else viene eseguito
# Utile per le ricerche, ad esempio.
else:
    print("Non trovato")
