
print("Using a normal function I get:")
def f(x):
    return 3*x + 1;
#
print(f(2));

print("Using a lambda expression I get:")
g = lambda x: 3*x+1;
print(g(2))

full_name = lambda fn, ln: fn.strip().title() + " " + ln.strip().title();
name = full_name("    leonhard", "EULER")
print(name)

# Write a function that returns functions
def build_quadratic_function(a, b, c):
    """Returns the function f(x) = ax^2 + bx + c"""
    return lambda x: a*x**2 + b*x + c;
#

f = build_quadratic_function(2, 3, -5)
print(f(0))
print(f(1))
print(f(2))
print(build_quadratic_function(3,0,1)(2)) # lambda expression not given a name, but is used