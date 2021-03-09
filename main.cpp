#include <vector>
#include <iostream>
#include <algorithm>
#include "MyVector.h"

int main()
{
	try
	{
		MySTL::MyVector<int> vec = { 6, 5, 18, 0, 27, 1 };
		vec.sort(vec.begin() + 2, vec.end());
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