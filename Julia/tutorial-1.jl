using Printf
using Statistics

s = 0
s = "Hello Dog"
println(s)

# How to function and set an object's datatype
# Int8, Int16, Int64, Float32, UInt16, etc
function changeNum()
    x::Int8 = 10
end

# BigFloat BigInt
function floatpractice()
    bf = 1.111111111111111111
    println(bf + 0.111111111111111111)
    bf2::BigFloat = 1.111111111111111111
    println(bf2 + 0.111111111111111111)
end
floatpractice()

# Characters
c = 'A'

# Cast to different datatypes
c2 = Char(120) # UTF8 unicode
println(c2)

# Cast float to int
i1 = UInt8(trunc(3.14))
println(i1)
f1 = parse(Float64, "1") # Parse string
println(f1)
i2 = parse(Int8, "1")
println(i2)

# Strings
s1 = "Random words\n"
println(length(s1))
println(s1[1])
println(s1[end])
println(s1[1:4])
s2 = string("Yukiteru", "amano")
println(s2)
println("Yno" * " Gasai") # Concatenate strings
i3 = 2
i4 = 3
println("$i3 + $i4 = $(i3 + i4)")

s3 = "This is a\nMulti-line string!\n\n"
print(s3)

# Find a character in a string
println(findfirst(isequal('i'), "Keigo"))

# Find a substring
println(occursin("key", "Monkey"))

## --- Conditionals ---
age = 12
if age >= 5 && age <= 6
    println("They are in kindergarden")
elseif age >= 7 && age <= 13
    println("They are a big kid")
else
    println("They are an adult")
end

# Terenary operator
bool1 = true || false ? "true" : "false"
@printf("true || false = %s\n", bool1)
@printf("!true = %s\n", !true ? "true" : "false")

## --- Loops ---
println("\n\n --- Loops ---\n")
ii = 1
while ii < 20
    if (ii % 2) == 0
        println(ii)
        global ii += 1 # Works with values outside of loop scope
        continue
    end
    global ii += 1
    if ii >= 10
        break
    end
end

# Use a range
for ii = 1:5
    println(ii)
end
for ii in [2,4,6]
    println(ii)
end
for ii = 1:5, jj = 2:2:10
    println((ii, jj))
end


## --- Arrays ---
println("\n\n --- Arrays ---\n")
a1 = zeros(Int32, 2, 2)
a2 = Array{Int32}(undef, 5) # Undefined integer values
a3 = Float64[]
a4 = [1,2,3]
println(a4[1])
println(a1[end])
println(5 in a4)
println(findfirst(isequal(2),a4))

# Generic function
f(a) = (a >= 2) ? true : false
println(f(4))
println(findall(f, a4))
println(count(f, a4))

# Other functions
println("\nSizes of arrays\n")
println(size(a4))
println(length(a4))
println(sum(a4))
println(a4)
splice!(a4, 2:1, [8, 9])
println(a4)
splice!(a4, 2:3)
println(a4)
println(maximum([1,2,3,4,5,6,7]))
println(minimum([1,2,3,4,5,6,7]))
println(a4)
println(a4*2)

a5 = [1, 3.14, "Hello"]

# You can store functions in an array!
a6 = [sin, cos, tan]
for n in a6
    println(n(3.1416/180*90))
end

a7 = [1 2 3; 4 5 6; 7 8 9]
for n = 1:2, m = 1:3
    @printf("%d ", a7[n,m])
end
println()

# Get every row in second column
println(a7[:,2])
println(a7[2,:])
a8 = collect(1:5) # Array from a range of values
a9 = collect(2:2:10)
a9 = collect(4:-1:1)
for n in a9 println(n) end

# Create array with comprehension
a10 = [n^2 for n in 1:5]
println(a10)

# Can add values to arrays
push!(a10, 36)

a11 = [n * m for n in 1:5, m in 1:4]
println(a11)

# Random array
a12 = rand(0:9, 5, 5)
for n = 1:5
    for m = 1:5
        print("$(a12[n,m]) ")
    end
    println()
end

## Tuples
# Most array values can work with tuples, except tuple values cannot be
# changed once they are defined
println("\n--- Tuples ---")
t1 = (1,2,3,4)
println(t1)
println(t1[1])
for i in t1
    println(i)
end
t2 = ((1,2,), (3,4))
println(t2[1][1])

t3 = (sue = ("Sue" ,100), paul = ("Paul", 23))
println(t3.sue)

## Dictionaries
# The key must be unique
d1 = Dict("pi" => 3.14, "e" => 2.718)
println(d1["pi"])
d1["golden"] = 1.618
delete!(d1, "pi")
println(haskey(d1, "pi"))
println(in("pi"=>3.14))
println(keys(d1))
println(values(d1))

for kv in d1
    println(kv)
end

for (key, value) in d1
    println(key, " : ", value)
end


## Sets
# Every value must be unique
st1 = Set(["Jim", "Pam", "Jim"])
push!(st1, "Michael")
println(in("Dwight", st1))
st2 = Set(["Stanley", "Meredith"])
println(union(st1, st2))
println(intersect(st1, st2))
println(setdiff(st1,st2))



## Functions
println("\n")
println("--- Functions ---")

getSum(x,y) = x + y # A signle expression function
x, y = 1, 2
@printf("%d + %d = %d\n", x, y, getSum(x,y))

function canIVote(age=16) # Multiple expression function
    if age >= 18
        println("You can vote")
    else
        println("Can't vote")
    end
end
canIVote(43)

# Pass by value function
v1 = 5
function changeV1(v1)
    v1 = 10
end
changeV1(v1)
println("v1 = 10? v1 = $v1")

# Multiple arguments
function getSum2(x, args...)
    sum = x
    for a in args
        sum += a
    end
    sum
end
println("sum = $(getSum2(1,2,3,4,5,6))")

# Return multiple values
function next2(val)
    return (val + 1, val + 2)
end
println(next2(4))

# Functoin that returns a function
function makeMultiplier(num)
    return function(x) return x*num end
end
mult3 = makeMultiplier(3)
println(mult3(6))

# Specify types
function getSum3(num1::Number, num2::Number)
    return num1 + num2
end
function getSum3(num1::String, num2::String)
    return parse(Int32, num1) + parse(Int32, num2)
end
println("5 + 4 = ", getSum3(5,4))
println("5 + 4 = ", getSum3("5", "4"))

# Anonymous function
v2 = map(x -> x * x, [1,2,3])
println(v2)
v3 = map((x,y) -> x + y, [1,2], [3,4])
println(v3)
v4 = reduce(+, 1:100) # Reduce uses a function multiple times to finally get to one result
println(v4)

# Longest word in sentence using reduction
sentence = "This is a sentence wahoo"
sArray = split(sentence)
longest = reduce((x,y) -> length(x) > length(y) ? x : y, sArray)
println(longest)

# Cool thing with math
x = 5
println(2x) # You don't even need to specify to multiply
a13 = [1,2,3]
println(a13 .* 2)


## Enums
println("\n")
println("--- Enums ---")
@enum Color begin
    red = 1
    blue = 2
    green = 3
end
favColor = green::Color
println(favColor)


## Symbols
# Mutable strings that represent variables
:Derek
println(:Derek)

# Commonly used as keys with dictionaries
d2 = Dict(:pi=>3.14, :e=>2.718)
println(d2[:pi])


## Structs
# Composite types, a type that contains many fields
struct Customer
    name::String
    balance::Float32
    id::Int
end
bob = Customer("Bob Smith", 10.50, 123)
println(bob.name)
# Structs are immutable by default
#bob.name = "Sue Smith" # Returns and Error!

# Creates a mutable struct
mutable struct Customer2
    name::String
    balance::Float32
    id::Int
end
bob = Customer2("Bob Smith", 10.50, 123)
println(bob.name)
bob.name = "Sue Smith"
println(bob.name)


## Abstract types
# These can't be instantiated like structs, but can have subtypes
println("\n")
println("--- Abstract Types ---")

abstract type Animal end
struct Dog <: Animal
    name::String
    bark::String
end
struct Cat <: Animal
    name::String
    meow::String
end
bowser = Dog("Bowser", "Ruff")
muffin = Cat("Muffin", "Meow")
function makeSound(animal::Dog) # This function can refer to either one
    println("$(animal.name) says $(animal.bark)")
end
function makeSound(animal::Cat) # This function can refer to either one
    println("$(animal.name) says $(animal.meow)")
end
makeSound(bowser)
makeSound(muffin)


