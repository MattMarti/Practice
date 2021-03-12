#include <iostream>

int main() {

    float* bigArray1 = new float[16384];
    float* bigArray2 = new float[16384];
    float* bigArray3 = new float[16384];

    for (int i = 0; i < 16384; i++) {
        bigArray1[i] = float(rand() % 100);
        bigArray2[i] = 50.0f;
        bigArray3[i] = 0.0f;
    }

    for (int i = 0; i < 16384; i++) {
        if (bigArray1[i] == 23.0f) {
            bigArray3[i] = bigArray1[i];
        }
        else {
            bigArray3[i] = bigArray1[i] + bigArray2[i];
        }
    }

    delete[] bigArray1;
    delete[] bigArray2;
    delete[] bigArray3;

    return 0;
}