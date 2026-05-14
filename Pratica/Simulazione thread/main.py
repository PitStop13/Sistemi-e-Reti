import random
from threading import Thread, Lock
from time import sleep
from clinic import Patient, Secretary, Doctor

random_names = ["Alice", "Bob", "Charlie", "Diana", "Eve",
                "Frank", "Grace", "Heidi", "Ivan", "Judy"]

def new_patient():
    patient = Patient(random.choice(random_names))
    print(f"New patient {patient} arrived at the clinic.")
    return patient

def secretary_work(secretary):
    while True:
        patient = new_patient()
        secretary.add_patient_to_queue(patient)
        sleep(random.randint(1, 5))

def doctor_work(doctor):
    while True:
        doctor.consult_patient()
        sleep(random.randint(1, 5))

def main():
    print("Welcome to the medicine clinic!")

    patients: list[Patient] = []   # lista condivisa
    lock: Lock = Lock()            # ← PUNTO 5: lock creato QUI, uno solo per tutti

    secretaries = [
        Secretary("Sophie", patients, lock),   # ← PUNTO 6: lock passato
        Secretary("Liam",   patients, lock),
        Secretary("Olivia", patients, lock),
        Secretary("Noah",   patients, lock),
    ]

    doctors = [
        Doctor("Dr. Smith",    patients, lock),   # ← PUNTO 6: lock passato
        Doctor("Dr. Johnson",  patients, lock),
        Doctor("Dr. Williams", patients, lock),
        Doctor("Dr. Brown",    patients, lock),
        Doctor("Dr. Jones",    patients, lock),
        Doctor("Dr. Miller",   patients, lock),
    ]

    # ← PUNTO 7: creazione e avvio thread
    secretary_threads = [Thread(target=secretary_work, args=[s]) for s in secretaries]
    doctor_threads    = [Thread(target=doctor_work,    args=[d]) for d in doctors]

    threads = secretary_threads + doctor_threads

    for t in threads:
        t.start()    # avvio tutti i thread

    for t in threads:
        t.join()     # aspetto che finiscano (non finiranno mai per il while True)

if __name__ == "__main__":
    main()