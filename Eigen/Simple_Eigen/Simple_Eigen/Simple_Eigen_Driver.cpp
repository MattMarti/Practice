#include "stdafx.h"

#include "Simple_Eigen_Driver.h"
#include <iostream>
#include "Eigen/Dense"

int driver()
{
	using namespace std;
	using Eigen::MatrixXd;

	MatrixXd mat(2, 2);
	mat(0, 0) = 3;
	mat(1, 0) = 2.5;
	mat(0, 1) = -1;
	mat(1, 1) = mat(1, 0) + mat(0, 1);

	cout << mat << endl;

	return 0;
}