from functools import lru_cache

'''
Socratica tutorial on memoization for efficient recursion.
Calculate the Fibonacci Sequence.

Memoization allows you to cache values, so that you don't ahve to 
repeat the function call over and over again.

@author: Matt Marti
@date: 2019-07-04
'''

def main():
    for n in range(2,51):
        #print(n, ":", fibonacci(n));
        #print(n, ":", fibonacci_manual(n));
        #print(n, ":", fibonacci_lru(n));
        print(n, ":", fibonacci_lru(n) / fibonacci_lru(n-1));
        #print(n, ":", fibonacci(n) / fibonacci(n-1));
    #
#

@lru_cache(maxsize = 10000) #1028 values cached by default
def fibonacci_lru(n):
    '''Use LRU Cache utility to speed computation'''
    if n == 0:
        return 0;
    elif n == 1:
        return 1;
    elif n > 1:
        return fibonacci_lru(n-1) + fibonacci_lru(n-2);
    #
#

fibonacci_cache = {};
'''Fibonnaci cache values for manual memoization.'''

def fibonacci_manual(n):
    '''Fibonnaci with manual cache values'''
    
    # If we have cached value, return it. Otherwise compute it
    if n in fibonacci_cache:
        return fibonacci_cache[n];
    #
    
    # Compute the Nth term
    if n == 0:
        value = 0;
    elif n == 1:
        value = 1;
    elif n > 1:
        value = fibonacci_manual(n-1) + fibonacci_manual(n-2);
    #
    
    # Cache value and return it
    fibonacci_cache[n] = value;
    return value;
#

def fibonacci(n):
    '''
    Return the nth term of the Fibonacci Sequence.
    Naive implementation.
    '''
    if n == 0:
        return 0;
    elif n == 1:
        return 1;
    elif n > 1:
        return fibonacci(n-1) + fibonacci(n-2);
    #
#

if __name__ == "__main__":
    main();
#