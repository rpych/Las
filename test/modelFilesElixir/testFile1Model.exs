#based on AI generated file
defmodule Greetings do
  # A function that takes a name and prints a greeting
  def say_hello(name) do
    IO.puts("Hello, #{name}!")
  end

  def say_goodbye(name) do
    IO.puts("Goodbye, #{name}!")
    #above line will be added
  end

  def is_even_or_odd(number) do
    if rem(number, 2) == 0 do
      IO.puts("#{number} is even.")
    else
      IO.puts("#{number} is odd.")
    end
  end
end

Greetings.say_hello("Alice")
Greetings.say_goodbye("has Cat")
Greetings.is_even_or_odd(5)
Greetings.is_even_or_odd(8)
