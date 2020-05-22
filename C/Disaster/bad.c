#include <stdio.h>

/*
This program does a thing in C which is illegal in C++:
It initializes an array with more elements than it requires.
Essentially, the array here doesn't end in 0. Printing it 
will be interesting.
*/
int main() {
    
    char v[5] = "Oscar";
    printf("%s", v);
    
    return 0;
}