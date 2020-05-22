using Printf
using Statistics

## File I/O

open("text.txt", "w") do file
    write(file, "Here is some text\n")
    write(file, "It is pretty great\n")
end

open("text.txt") do file
    data = read(file, String)
    println(data)
end

open("text.txt") do file
    for line in eachline(file)
        println(line)
    end
end
