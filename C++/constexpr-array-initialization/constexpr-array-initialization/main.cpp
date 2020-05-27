/*

https://www.educative.io/edpresso/how-to-create-an-n-element-constexpr-array-in-cpp

*/


#include <iostream>
using namespace std;

template<int size>
struct ConstArray {
    int arr[size];

    // 'constexpr' constructor:
    constexpr ConstArray() :arr() {
        arr[0] = 0;
        arr[1] = 1;
        for (int i = 2; i < size; i++)
            arr[i] = arr[i-1] + arr[i-2];
    }

    // This member function should have 'const':
    void print() const {
        for (int i = 0; i < size; i++)
            cout << arr[i] << endl;
    }
};

int main() {
    constexpr int n = 20;
    constexpr ConstArray<n> arr = ConstArray<n>();
    arr.print();

    return 0;
}