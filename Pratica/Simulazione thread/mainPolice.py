import random
from threading import Thread, Lock
from time import sleep
from police import Suspect, PatrolOfficer, Inspector

names_list = ["Joker", "Pinguino", "Enigmista", "Due Facce", "Harley", "Bane"]

def new_crime():
    """Simulate a new crime and a suspect being caught."""
    return Suspect(random.choice(names_list))

def patrol_work(officer: PatrolOfficer):
    """Simulate the work of a patrol officer."""
    while True:
        suspect = new_crime()
        officer.arrest_suspect(suspect)
        sleep(random.randint(1, 4)) 

def inspector_work(inspector: Inspector):
    """Simulate the work of an inspector."""
    while True:
        inspector.interrogate()
        sleep(random.randint(2, 5)) 

def main():
    print("--- Inizio turno al Distretto di Polizia ---")

    # Le nostre due risorse condivise
    cella: list[Suspect] = []
    verbale: list[str] = []

    # TODO: Crea i Lock necessari per proteggere la cella e il verbale
    cella_lock : Lock = Lock()
    verbale_lock : Lock = Lock()
    
    # TODO: Inizializza gli Agenti (passando i parametri corretti)
    officers = [
        PatrolOfficer("Gordon", cella, verbale, cella_lock, verbale_lock),
        PatrolOfficer("Bullock", cella, verbale, cella_lock, verbale_lock)
    ]

    # TODO: Inizializza gli Ispettori (passando i parametri corretti)
    inspectors = [
        Inspector("Batman", cella, verbale, cella_lock, verbale_lock),
        Inspector("Robin", cella, verbale, cella_lock, verbale_lock)
    ]

    # TODO: Crea i thread per gli Agenti e avviali
    officers_threads = [
        Thread(target=patrol_work, args=[officer]) for officer in officers
    ]
    for thread in officers_threads:
        thread.start()

    # TODO: Crea i thread per gli Ispettori e avviali
    inspectors_threads = [
        Thread(target=inspector_work, args=[inspector]) for inspector in inspectors
    ]
    
    for thread in inspectors_threads:
        thread.start()
    
    # TODO: Fai in modo che il thread principale attenda l'esecuzione
    threads = officers_threads + inspectors_threads
    for thread in threads:
        thread.join()
    
if __name__ == "__main__":
    main()