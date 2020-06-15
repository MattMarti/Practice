
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex buff_lock;
const size_t BUFF = 2500;
int buff[BUFF] = { 0 };

void make_neg() {
    for (size_t i = 0; i < BUFF; i++) {
        buff[i] = -1 * (int)i;
        std::cout << std::endl;
    }
}

void print() {
    for (size_t i = 0; i < BUFF; i++) {
        std::cout << buff[i];
    }
}

int main() {
    using namespace std;

    for (size_t i = 0; i < BUFF; i++) {
        buff[i] = i;
    }

    thread t(make_neg);
    thread t2(print);
    t.join();
    t2.join();

    cout << "done!" << endl;


    cout << "Starting sanity check" << endl;
    vector<int> v = { 1, 2, 3 };
    int x = v[1];
    x = -2;
    cout << v[1] << endl;
}
