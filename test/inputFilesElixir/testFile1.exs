#based on AI generated file
defmodule Greetings do
  #^^las begin^^
  require Logger
  #^^las end^^
  # A function that takes a name and prints a greeting
  def say_hello(name) do
    IO.puts("Hello, #{name}!")
  end

  def say_goodbye(name) do
    #^^las begin^^
    # below line will be erased
    Logger.info("Goodbye from here")
    #^^las end^^
    #^^las sub begin^^
    #IO.puts("Goodbye, #{name}!")
    ##above line will be added
    #^^las sub end^^
  end

  def is_even_or_odd(number) do
    if rem(number, 2) == 0 do
      Logger.info("#{number} is even.")#^^las^^      IO.puts("#{number} is even.")
    else
      IO.puts("#{number} is odd.")
    end
  end
end

# Calling functions from the Greetings module  #^^las^^
Greetings.say_hello("Alice")
Greetings.say_goodbye("Bob")#^^las^^
#^^las sub begin^^
#Greetings.say_goodbye("has Cat")
#^^las sub end^^
Greetings.is_even_or_odd(5)
Greetings.is_even_or_odd(8)
