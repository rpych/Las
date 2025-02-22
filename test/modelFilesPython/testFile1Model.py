#based on AI generated file
class Person:
    """A simple class to represent a person."""
    
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def greet(self):
        print(f"Hello, my name is {self.name} and I am {self.age} years old.")
    
    def have_birthday(self):
        """Method to simulate having a birthday."""
        self.age += 1
        print(f"Happy Birthday, {self.name}! You are now {self.age} years old.")
    
    def is_minor(self):
        return self.age < 18

# Example usage
if __name__ == "__main__":
    # Create an instance of the Person class
    person1 = Person("Alice", 25)

    # Call methods
    person1.greet()
    person1.have_birthday()
    
    # Check if the person is an adult    
    if not person1.is_minor():
        print(f"{person1.name} is an adult.")
    else:
        print(f"{person1.name} is not an adult.")
