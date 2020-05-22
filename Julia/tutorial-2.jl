using Printf
using Statistics

## Exception Handling
print("Enter a number: ")
num1 = chomp(readline()) # chomp() gets rid of the newline when user presses "enter"
print("Enter a second number: ")
num2 = chomp(readline())

try
    val = parse(Int32, num1) / parse(Int32, num2)
    if (val == Inf)
        error("Can't Divide by Zero")
    else
        println(val)
    end
catch e
    println(e)
end


