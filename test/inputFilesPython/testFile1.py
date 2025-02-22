#based on AI generated file
class Person:
    """A simple class to represent a person."""
    
    def __init__(self, name, age):
        #^^las begin^^
        """Constructor to initialize the person's name and age."""
        #^^las end^^
        self.name = name
        self.age = age
    
    def greet(self):
        """Method to greet the user."""#^^las^^
        print(f"Hello, my name is {self.name} and I am {self.age} years old.")
    
    def have_birthday(self):
        """Method to simulate having a birthday."""
        self.age += 1
        print(f"Happy Birthday, {self.name}! You are now {self.age} years old.")
    
    #^^las begin^^
    def is_adult(self):
        """Method to check if the person is an adult."""
        return self.age >= 18
    #^^las end^^
    """^^las sub begin^^
    def is_minor(self):
        return self.age < 18
    ^^las sub end^^ """

# Example usage
if __name__ == "__main__":
    # Create an instance of the Person class
    person1 = Person("Alice", 25)

    # Call methods
    person1.greet()
    person1.have_birthday()
    
    # Check if the person is an adult    
    #^^las begin^^
    if person1.is_adult():
        print(f"{person1.name} is an adult.")
    #^^las end^^
    #^^las sub begin^^ 
    #if not person1.is_minor():
    #    print(f"{person1.name} is an adult.")
    #^^las sub end^^ 
    else:
        print(f"{person1.name} is not an adult.")
