#include <vector>
#include <bitset>
#include <string>
#include <iostream>


int main() {
    using namespace std;

    bitset<3> b1(string("101"));
    cout << b1 << endl;

    cout << "---" << endl;
    vector<bitset<3>> v = { bitset<3>("101"), bitset<3>("010"), bitset<3>("111") };
    for (auto b : v) {
        cout << b << endl;
    }

    return 0;
}