// Main_Solution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Foobar.h"
#include "Boo.h"
#include <iostream>

int main()
{
	using namespace std;
	//using namespace foobar;

	cout << "Foo:" << endl;
	cout << foo(0) << endl;
	cout << foo(1) << endl;
	cout << foo(2) << endl;
	cout << foo(3) << endl;
	cout << foo(4) << endl;
	cout << foo(5) << endl;
	cout << foo(6) << endl;

	cout << endl << "Boo:" << endl;
	cout << boo(0) << endl;
	cout << boo(1) << endl;
	cout << boo(2) << endl;
	cout << boo(3) << endl;
	cout << boo(4) << endl;
	cout << boo(5) << endl;
	cout << boo(6) << endl;

    return 0;
}

