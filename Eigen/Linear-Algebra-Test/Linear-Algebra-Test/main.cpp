#include <iostream>
#include "Eigen/Dense"

int main() {
    using namespace std;
    using namespace Eigen;

    MatrixXd A = MatrixXd::Zero(3,3);
    VectorXd b = VectorXd::Zero(3);

    A(0, 0) = 3;
    A(0, 1) = 2;
    A(0, 2) = 1;
    A(1, 0) = 4;
    A(1, 1) = 3;
    A(1, 2) = 1;
    A(2, 0) = 5;
    A(2, 1) = 6;

    b[0] = 1;
    b[1] = 2;
    b[2] = 3;

    VectorXd x = A.fullPivLu().solve(b);

    cout << "A = " << endl;
    cout << A << endl << endl;

    cout << "b = " << endl;
    cout << b << endl << endl;

    cout << "x = " << endl;
    cout << x << endl << endl;

}