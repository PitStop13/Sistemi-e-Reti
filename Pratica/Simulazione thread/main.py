import random
from threading import Thread
from threading import Lock
from time import sleep
from clinic import Patient, Secretary, Doctor

random_names = ["Alice", "Bob", "Charlie", "Diana", "Eve", "Frank", "Grace", "Heidi", "Ivan", "Judy"]

def new_patient():
    """Simulate the arrival of a new patient at the clinic."""
    patient = Patient(random.choice(random_names))
    print(f"New patient {patient} arrived at the clinic.")
    return patient

def secretary_work(secretary):
    """Simulate the work of a secretary at the clinic."""
    while True:
        patient = new_patient()
        secretary.add_patient_to_queue(patient)

        sleep(random.randint(1, 5))

def doctor_work(doctor):
    """Simulate the work of a doctor at the clinic."""
    while True:
        doctor.consult_patient()

        sleep(random.randint(1, 5))

def main():
    print("Welcome to the medicine clinic!")

    patients : list[Patient] = []
    """The patients waiting in the clinic."""

    lock = Lock()

    secretaries = [
        Secretary("Sophie", patients, lock),
        Secretary("Liam", patients, lock),
        Secretary("Olivia", patients, lock),
        Secretary("Noah", patients, lock),
    ]

    doctors = [
        Doctor("Dr. Smith", patients,lock),
        Doctor("Dr. Johnson", patients,lock),
        Doctor("Dr. Williams", patients,lock),
        Doctor("Dr. Brown", patients,lock),
        Doctor("Dr. Jones", patients,lock),
        Doctor("Dr. Miller", patients,lock),
    ]

    secretary_threads = [
        Thread(target=secretary_work, args=[secretary]) for secretary in secretaries
    ]
    doctor_threads = [
        Thread(target=doctor_work, args=[doctor]) for doctor in doctors
    ]

    threads = secretary_threads + doctor_threads

    for thread in threads:
        thread.start()
        
    for thread in threads:
        thread.join()
    if __name__ == "__main__":
        main()
