# In Python i dizionari sono coppie chiave-valore

# Inizializzazione
diziomario = {} # generico
diziomario = dict[int, str]() # specifico

# Inizializzazione diretta
diziomario = {
    "chiave": "valore"
} # generico

diziomario = dict[str, str]({
    "chiave": "valore"
}) # specifico

print(diziomario)
print(diziomario["chiave"])
