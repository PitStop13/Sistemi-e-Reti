try:
    # faccio cose che si possono rompere
    n = int(input("Inserisci un numero: "))
except Exception:
    # gestisco l'eccezione se si presenta
    print("HELP")
else:
    # viene eseguito se nel try va tutto bene
    print("Bravo")
finally:
    # ripulisco (viene eseguito sempre)
    print("Veramente")
