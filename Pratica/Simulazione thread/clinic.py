from threading import Lock

class Patient:
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return f"Patient {self.name}"


class Doctor:
    name: str
    patients: list[Patient]   # ← PUNTO 1: attributi aggiunti
    patients_lock: Lock       # ← PUNTO 1: attributi aggiunti

    def __init__(self, name: str, patients: list[Patient], patients_lock: Lock):
        # ← PUNTO 2: parametri aggiunti
        self.name = name
        self.patients = patients          # salvo la lista condivisa
        self.patients_lock = patients_lock  # salvo il lock condiviso

    def __str__(self):
        return f"Doctor {self.name}"

    def consult_patient(self):
        # ← PUNTO 4: metodo completato
        if self.patients_lock.acquire(timeout=5):   # prendo il lock
            # Prendo il primo paziente SE la lista non è vuota
            patient = self.patients.pop(0) if len(self.patients) > 0 else None
            self.patients_lock.release()             # rilascio subito il lock

            if patient:   # stampo solo se ho preso un paziente
                print(f"{self} is consulting {patient}.")


class Secretary:
    name: str
    patients: list[Patient]   # ← PUNTO 1: attributi aggiunti
    patients_lock: Lock       # ← PUNTO 1: attributi aggiunti

    def __init__(self, name: str, patients: list[Patient], patients_lock: Lock):
        # ← PUNTO 2: parametri aggiunti
        self.name = name
        self.patients = patients
        self.patients_lock = patients_lock

    def __str__(self):
        return f"Secretary {self.name}"

    def add_patient_to_queue(self, patient: Patient):
        # ← PUNTO 3: metodo completato
        if self.patients_lock.acquire(timeout=5):   # prendo il lock
            self.patients.append(patient)            # aggiungo in fondo alla coda
            self.patients_lock.release()             # rilascio subito il lock

            print(f"{self} is adding {patient} to the queue.")