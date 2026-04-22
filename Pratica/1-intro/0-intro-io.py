# Questo è un commento mono-linea

"""
Questo è un commento multi
linea
"""

'''
Anche questo è un commento
multi
linea
'''

# Input
stringa = input()

# Input numero intero
# in python il cast è una vera
# e propria conversione, non un
# "vedere lo stesso dato da una
# prospettiva diversa"
n = int(input())

# Output
print("Stringa normale")

# Queste due notazioni sono uguali
print(f"Stringa formattata con n = {n}")
print("Altra stringa formattata con n = {}".format(n))

# Operazioni possibili:
# + -> somma di numeri, concatenazione di stringhe, date e liste
# - -> differenza fra numeri, date
# * -> prodotto fra numeri
# / -> divisione fra numeri (quoziente)
# // -> divisione intera fra numeri (quoto)
# % -> modulo fra numeri (resto divisione intera)
# . -> accede a proprietà e metodi interni di un 
#      oggetto python, se presenti ("{}".format(n))


valore = "Questa è una stringa su una riga\nPer andare a capo devo usare\\n"
valore = """
Anche questa 
è una stringa
su più righe
"""
valore = '''
E pure questa
già che ci siamo
'''