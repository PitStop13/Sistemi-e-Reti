# Le tuple, a differenza delle liste, sono immutabili.
#
# Non possono essere modificate, ma possono essere 
# sostituite per intero.
tupla = (1, 2, 3)
tupla = (1, 1, 1)

# Inizializzazione
tupla = (1, 2, 3) # generico
tupla = tuple[int, int, int]([1, 2, 3]) # specifico

# Accesso 
print(tupla[0])
