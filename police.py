from threading import Lock

class Suspect:
    """A suspect brought into the station."""
    name: str

    def __init__(self, name: str):
        self.name = name

    def __str__(self):
        return f"Sospettato {self.name}"

class PatrolOfficer:
    """An officer who arrests suspects."""
    name: str
    
    # TODO: Add attributes for the cell queue, the logbook (verbale), and their respective locks
    celle : list[Suspect] = []
    cella_lock : Lock

    verbali : list[str] =  []
    verbale_lock : Lock

    def __init__(self, name: str, celle : list[Suspect], verbali : list[str], cella_lock, verbale_lock): # TODO: add required parameters
        self.name = name
        self.celle = celle
        self.verbali = verbali
        self.cella_lock = cella_lock
        self.verbale_lock = verbale_lock

    def __str__(self):
        return f"Agente {self.name}"

    def arrest_suspect(self, suspect: Suspect):
        """Put suspect in the cell and log the action."""
        
        # TODO: Acquire the necessary locks safely (Remember the lock ordering rule!)
        if self.cella_lock.acquire(timeout=5):
            if self.verbale_lock.acquire(timeout=5):
                # TODO: Add suspect to the cell queue
                self.celle.append(suspect)

                # TODO: Append a string to the logbook (e.g., f"{self} ha arrestato {suspect}")
                self.verbali.append(f"{self} ha arrestato {suspect}")
                self.verbale_lock.release()

                # Stampa a video l'avvenuto arresto (fuori dai lock)
                print(f"[{self}] Arresto completato per {suspect}")
            self.cella_lock.release()

class Inspector:
    """An inspector who interrogates suspects."""
    name: str
    
    # TODO: Add attributes for the cell queue, the logbook (verbale), and their respective locks
    celle : list[Suspect] = []
    cella_lock : Lock

    verbali : list[str] =  []
    verbale_lock : Lock

    def __init__(self, name: str, celle : list[Suspect], verbali : list[str], cella_lock, verbale_lock): # TODO: add required parameters
        self.name = name
        self.celle = celle
        self.verbali = verbali
        self.cella_lock = cella_lock
        self.verbale_lock = verbale_lock

    def __str__(self):
        return f"Ispettore {self.name}"

    def interrogate(self):
        """Take a suspect from the cell and log the interrogation."""
        
        # TODO: Acquire the necessary locks safely (MUST use the exact same order as the Officer!)
        if self.cella_lock.acquire(timeout=5):
            # TODO: Check if the cell queue is not empty
            # TODO: If not empty, pop the first suspect and assign to target_suspect
            # TODO: Append a string to the logbook (e.g., f"{self} sta interrogando {target_suspect}")
            if self.verbale_lock.acquire(timeout=5) and  len(self.celle) > 0:
                target_suspect = self.celle.pop(0)
                self.verbali.append(f"{self} sta interrogando {target_suspect}")
                self.verbale_lock.release()
            self.cella_lock.release()
        
        # Stampa a video (fuori dai lock)
        if target_suspect:
            print(f"[{self}] Interrogatorio iniziato con {target_suspect}")
        else:
            pass # Nessun sospettato in cella