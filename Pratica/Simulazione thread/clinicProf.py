from threading import Lock

class Patient:
    """A patient in the clinic."""

    name: str
    """The name of the patient."""

    def __init__(self, name):
        """Initialize a new patient with the given name."""
        self.name = name

    def __str__(self):
        """Return a string representation of the patient."""
        return f"Patient {self.name}"

class Doctor:
    """A doctor in the clinic."""

    name: str
    """The name of the doctor."""
    
    patients: list[Patient]
    """The list of patients in queue"""

    patients_lock: Lock
    """The lock for the list of patients"""

    def __init__(self, name: str, patients: list[Patient], patients_lock: Lock):
        """Initialize a new doctor with the given name and list of patients."""
        self.name = name
        self.patients = patients
        self.patients_lock = patients_lock

    def __str__(self):
        """Return a string representation of the doctor."""
        return f"Doctor {self.name}"

    def consult_patient(self):
        """Consult the next patient in the queue."""

        if self.patients_lock.acquire(timeout=5):
            patient = self.patients.pop(0) if len(self.patients) > 0 else None
            self.patients_lock.release()

            if patient:
                print(f"{self} is consulting {patient}.")

class Secretary:
    """A secretary in the clinic."""

    name: str
    """The name of the secretary."""
    
    patients: list[Patient]
    """The list of patients"""

    patients_lock: Lock
    """The lock for the list of patients"""

    def __init__(self, name: str, patients: list[Patient], patients_lock: Lock):
        """Initialize a new secretary with the given name and list of patients."""
        self.name = name
        self.patients = patients
        self.patients_lock = patients_lock

    def __str__(self):
        """Return a string representation of the secretary."""
        return f"Secretary {self.name}"

    def add_patient_to_queue(self, patient : Patient):
        """Add a patient to the queue."""
        if self.patients_lock.acquire(timeout=5):
            self.patients.append(patient)
            self.patients_lock.release()

            print(f"{self} is adding {patient} to the queue.")