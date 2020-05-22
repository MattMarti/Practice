#include "stdafx.h"
#include "fibb_driver.h"
#include <string>
#include <iostream>


int fib::driver()
{
	string str;
	int x = 0;
	int y = 1;
	while (cin >> str && !is_quit(str))
	{
		int z = next(x, y);
		x = y;
		y = z;
		cout << z << endl;
	}
	return 0;
}

int fib::next(int x, int y)
{
	return x + y;
}

bool fib::is_quit(std::string str)
{
	return (str == "q") || (str == "quit") || (str == "exit");
}