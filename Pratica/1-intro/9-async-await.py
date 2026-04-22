import time
import asyncio

"""
Questa è una funzione "sincrona"

sincrono => che segue le logiche di sincronizzazione della CPU
         => sequenziale
"""
def sync_function(value: str):
    time.sleep(1)
    print(value)

sync_function("ciao")
sync_function("o")
sync_function("o")
sync_function("o")

"""
Funzione asincrona: viene eseguita "in parallelo"
quindi non c'è mai garanzia di ordine se viene richiamata in
sequenza.
"""
async def async_function(value: str):
    # la parola chiave await "aspetta che una funzione async termini"
    await asyncio.sleep(1)
    print(value)

async def main(): # serve un main async per gestire logiche async
    # non c'è alcuna garanzia che stampi
    # 1 2 3 4 in questo ordine
    await asyncio.gather(
        async_function("1"),
        async_function("2"),
        async_function("3"),
        async_function("4"),
    )


# se sono nel main e non sto importando questo file
if __name__ == "__main__":
    asyncio.run(main()) # abilito la concorrenza
