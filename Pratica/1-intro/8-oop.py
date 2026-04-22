
# Una interfaccia altri non è che una 
# classe con metodi vuoti
class IPerson():
    def introduce_yourself(self, formal: bool):
        pass
    def greet(self, formal: bool):
        pass

"""
Definisco una classe con la parola chiave class

> Nota: self -> this degli altri linguaggi
"""
class Person(IPerson):
    """
    Il costruttore si chiama sempre __init__
    """
    def __init__(self, first_name: str, last_name: str):
        self.first_name = first_name
        self.last_name = last_name

    def introduce_yourself(self, formal : bool = False):
        if formal:
            intro_message = f"Saluti egregio, mi presento: son {self.last_name}, conosciuto informalmente come {self.first_name}"
        else:
            intro_message = f"Ciao! Sono {self.first_name}"
        
        print(intro_message)

    """
    Saluta l'audience.

    Args:
        formal -> definisce se il saluto è formale o no
                  default: False
    """
    def greet(self, formal : bool = False):
        self.introduce_yourself(formal)

"""
Estendiamo delle classi, passandogliele nelle parentesi
dopo il nome della classe.

E' ammessa l'ereditarietà multipla
"""
class Teacher(Person):
    def __init__(self, first_name: str, last_name: str, degree: str):
        """
        super() prende i metodi della superclasse, equivalente
        a "base" in C#
        """
        super().__init__(first_name, last_name)
        self.degree = degree
    
    def greet(self, formal = False):
        super().greet(formal)
        if formal:
            print("Sono anche docente laureato in {self.degree}.")

class Programmer(Person):
    def __init__(self, first_name: str, last_name: str, programming_languages: list[str]):
        super().__init__(first_name, last_name)
        if programming_languages is [] or programming_languages is None:
            raise Exception("Non sei un vero programmatore")
        self.programming_languages = programming_languages
    
    def greet(self, formal: bool = False): 
        super().greet(False)
        print(f"    Programmo in {",".join(self.programming_languages)}")

class ComputerScienceTeacher(Teacher, Programmer):
    def __init__(
            self, 
            first_name: str, 
            last_name: str, 
            degree: str, 
            programming_languages: list[str],
            subjects: list[str]
        ):
        if subjects is [] or subjects is None:
            raise Exception("Non sei un vero insegnante ITP")
        self.subjects = subjects

        Teacher.__init__(self, first_name, last_name, degree)
        Programmer.__init__(self, first_name, last_name, programming_languages)

    def greet(self, formal : bool = False):
        Teacher.introduce_yourself(self, formal)
        print(f"Insegno {",".join(self.subjects)}")
        print(f"Conosco questi linguaggi: {self.programming_languages}")
        


# docente = Person("Alessandro", "Sanino")
# docente.greet() # docente.greet(formal=False)
# docente.greet(True)
# docente.greet(formal=True)

