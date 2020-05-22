#include <iostream>
#include <cmath>

float quicksqrt(float x);

/*
Let's write the fast square root function!
*/
int main() {
    using namespace std;
    cout << "Sin approximation" << endl;

    for (float i = 0; i < 10; i += 0.1) {
        if (abs(i) < 0.05) { i = 0; }
        cout << "invsqrt(" << i << ") = " << 1/sqrt(i) << ",\t";
        cout << "quicksqrt(" << i << ") = " << quicksqrt(i) << endl;
    }

    return 0;
}

// This uses a 2nd order polynomial
float quicksqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;                       // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);             // what the fuck? 
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}