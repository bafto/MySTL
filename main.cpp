#include <vector>
#include <iostream>
#include <algorithm>
#include "MyVector.h"

int main()
{
	try
	{
		MySTL::MyVector<int> vec = { 1, 3, 3, 6, 5 };
		std::sort(vec.begin(), vec.end());
		vec.forEach([](int& c) {
			std::cout << c << " ";
			});
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}