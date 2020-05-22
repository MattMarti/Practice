'''
Socratica list comprehension video
Lists can have a mix of types

@author: Matt Marti
@date: 2019-06-29
'''

# List of squares of 100 positive integers
squares = [];
for i in range(1, 101):
    squares.append(i**2);
#
print(squares)

# You can do the same with a list comprehension
squares2 = [i**2 for i in range(1,101)];
print(squares2)

# Find remainders when you divide by 5
remainders5 = [x**2 % 5 for x in range(1,101)]
print(remainders5)

# List comprehension with finding strings
movies = ["Star Wars", "Gandhi", "Casablanca", "Shawshank Redemption",
    "Toy Story", "Gone with the Wind", "Citizen Kane", \
    "It's a Wonderful Life", "The Wizard of Oz", "Gattaca", 
    "Rear Window", "Ghostbusters", "To Kill A Mockingbird", 
    "Good Will Hunting", "2001: A Space Odyssey", 
    "Raiders of the Lost Ark", "Groundhog Day",
    "Close Encounters of the Third Kind"]
#
print(" ")
print("Movies that start with G:")

# Make movies that start with g
gmovies = []
for title in movies:
    if title.startswith("G"):
        gmovies.append(title)
    #
#
print(gmovies)

# List comprehension way
gmovies2 = [title for title in movies if title.startswith("G")];
print(gmovies2)

# Find movies released before 2000
movies = [("Citizen Kane", 1941), ("Spirited Away", 2001),
    ("It's a Wonderful Life", 1946), ("Gattaca", 1997), 
    ("No Country for Old Men", 2007), ("Rear Window", 1954),
    ("The Lord of the Rings: The Fellowship of the Ring", 2001),
    ("Groundhog Day", 1993), 
    ("Close Encounters of the Third Kind", 1997),
    ("The Royall Tenenbaums", 2001), ("The Aviator", 2004),
    ("Raiders of the Lost Ark", 1981)]

print(" ")
print("Movies before 2000:")
pre2k = [title for (title, year) in movies if year < 2000]
print(pre2k)

# Multiply each number by 4
v = [2, -3, 1]
print(" ")
print("Multiply array:")
print("this is wrong")
print(4*v) # 4*v = v+v+v+v, which concatenates the list
print("This is right")
print([4*x for x in v])

# Cartesian product of sets
# If A and B are sets, then the Cartesian product
# is the set of pairs (a,b) where 'a' is in A and
# 'b' is in B
A = [1,3,5,7]
B = [2,4,6,8]
cartesian_product = [(a,b) for a in A for b in B]
print(" ")
print("Cartesian product")
print(cartesian_product)


