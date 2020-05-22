#include <iostream>
#include <cmath>

float quicksin(float x);
float quicksin2(float x);
float quicksin3(float x);

/*
We want an approximation of sine that is fast and simple, that we
can implement on an embedded system.

We usually do sine like this:
    y = A * sin(2*pi*f*t)
or 
    y = sin(x)

Watch out! For large numbers, floating point precision can work against you.
*/
int main() {
    using namespace std;
    cout << "Sin approximation" << endl;

    for (float i = -4; i < 4; i += 0.1) {
        if (abs(i) < 0.05) { i = 0; }
        cout << "sin(" << i << ") = " << sin(i) << ",\t";
        cout << "quicksin(" << i << ") = " << quicksin(i) << ",\t";
        cout << "quicksin2(" << i << ") = " << quicksin2(i) << ",\t";
        cout << "quicksin3(" << i << ") = " << quicksin3(i) << endl;
    }

    return 0;
}

// This uses a 2nd order polynomial
float quicksin(float x) {
    const float oo2pi = 0.5 / 3.1416;
    float x_norm = x * oo2pi;
    if (x_norm < 0) { (x_norm *= -1) += 0.5; }
    x_norm = x_norm - (int)x_norm;

    // Approximate with parabolas
    // y = -16*(x-0)*(x-0.5) = -16*x*x-8*x = 1<<4 and 1<<3
    // y = 16*(x-0.5)*(x-1) = 16*(x*x-0.5*x-1x+0.5) = 16*x*x - 8*x - 16*x + 8

    if (x_norm < 0.5) {
        return -16 * x_norm*x_norm + 8 * x_norm;
    }
    else {
        return 16 * x_norm*x_norm - 16 * x_norm - 8 * x_norm + 8;
    }
}

// This uses a 3rd order polynomial
float quicksin2(float x) {
    const float oo2pi = 0.5 / 3.1416;
    float x_norm = x * oo2pi;
    if (x_norm < 0) { (x_norm *= -1) += 0.5; }
    x_norm = x_norm - (int)x_norm;

    // Approximate with 3rd order polynomial
    // Roots at 0, 0.5, 1

    return 20.785f*(x_norm - 0.0f)*(x_norm - 0.5f)*(x_norm - 1.0f);
}

// This one uses a MacLaurin series approximation. More precise, might take longer
float quicksin3(float x) {
    const float oo2pi = 0.5 / 3.1416;
    float x_norm = x * oo2pi;
    if (x_norm < 0) { (x_norm *= -1) += 0.5; }
    x_norm = x_norm - (int)x_norm;

    // Compute powers
    float x_norm_2 = x_norm * x_norm;
    float x_norm_3 = x_norm * x_norm_2;
    float x_norm_5 = x_norm_3 * x_norm_2;
    float x_norm_7 = x_norm_5 * x_norm_2;
    float x_norm_9 = x_norm_7 * x_norm_2;

    return x_norm - (1 / 6)*x_norm_3 + (1 / 120)*x_norm_5 - (1 / 5040)*x_norm_7 + (1 / 362880)*x_norm_9;
}