size = 1000

# lista di tutti i numeri pari da 0 a size (escluso)
pari = []
for n in range(size):
    if n % 2 == 0:
        pari.append(n)
print(pari)

# lista di tutti i numeri pari da 0 a size (escluso)
# in list comprehension

pari = [ n for n in range(size) if n % 2 == 0 ]
print(pari)
pari = [ 2 * n for n in range(size // 2) ]
print(pari)

parole = [ "xzczx", "Aldo", "ccccc", "Bruno", "Carlo", "LLLLL"]
# fare la lista ordinata di tutte le stringhe che contengono almeno una vocale 
# in tutte quelle fornite in input
parole_con_vocale = []
for parola in parole:
    for vowel in "aeiouAEIOU":
        if vowel in parola:
            parole_con_vocale.append(parola)
            break
parole_con_vocale = sorted(parole_con_vocale)

print(parole_con_vocale)

parole_con_vocale = sorted([
    parola for parola in parole if any(letter for letter in parola if ) 
])

print(parole_con_vocale)