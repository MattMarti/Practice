from math import pi;

def circle_area(r):
    if type(r) not in [int, float]:
        raise TypeError("The radius is not a non-negative real number");
    if r < 0:
    
        raise ValueError("Input radius is negative")
    #
    return pi*(r**2);
#

# Test the function
if __name__ == "__main__":
    radii = [2, 0, -3, 2+5j, True, "radius"]
    message = "Area of circles with r = {radius} is {area}.";
    
    for r in radii:
        A = circle_area(r);
        print(message.format(radius = r, area = A));
    #
#