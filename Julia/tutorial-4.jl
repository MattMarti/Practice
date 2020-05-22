using Printf
using Statistics

## Macros

macro doMore(n, exp)
    quote
        for i = 1:$(esc(n)) # "esc" hides what it contains until everything is ready to be executed
            $(esc(exp))
        end
    end
end

@doMore(2, println("Hello"))


# A custom do while loop
macro doWhile(exp)
    @assert exp.head == :while
    esc(quote # Represents body of the loop
        $(exp.args[2])
        $exp
        end
    )
end

z = 0
@doWhile while z < 10
    global z += 1
    println(z)
end
