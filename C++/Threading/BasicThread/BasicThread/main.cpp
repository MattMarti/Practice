#include <iostream>
#include <thread>

void foo();
void bar();

int main() {
    using namespace std;

    cout << "Starting first thread" << endl;

    thread thread1(foo);
    thread1.join();

    cout << endl << "Starting two threads" << endl;

    thread thread2(foo);
    thread thread3(bar);
    thread2.join();
    thread3.join();

    cout << endl << "Ending Program" << endl;


    return 0;
}

void foo() {
    using namespace std;

    for (int i = 0; i < 50; i++) {
        cout << "t1-" << i << endl;
    }
}

void bar() {
    using namespace std;

    for (int i = 0; i < 100; i++) {
        cout << "t2-" << i << endl;
    }
}