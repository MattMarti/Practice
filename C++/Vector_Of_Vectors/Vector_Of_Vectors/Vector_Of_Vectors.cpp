// Vector_Of_Vectors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "iostream"
#include "vector"

int main()
{
	using namespace std;

	size_t row = 10;
	size_t col = 5;
	vector<vector<int>> v = vector<vector<int>>(0);
	for (size_t i = 0; i < row; i++)
	{
		v.push_back(vector<int>(col));
	}

	for (size_t i = 0; i < v.size(); i++)
	{
		for (size_t j = 0; j < v[i].size(); j++)
		{
			v[i][j] = i*i+j*j;
		}
	}

	cout << "Matrix:" << endl;
	for (size_t i = 0; i < v.size(); i++)
	{
		cout << "| ";
		for (size_t j = 0; j < v[i].size(); j++)
		{
			if (v[i][j] < 10) cout << " ";
			cout << v[i][j] << " ";
		}
		cout << "|" << endl;
	}


    return 0;
}

