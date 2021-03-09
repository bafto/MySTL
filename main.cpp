#include <vector>
#include <iostream>
#include <algorithm>
#include "MyVector.h"

int main()
{
	try
	{
		MySTL::MyVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		auto vec2 = vec.subVec(vec.begin(), vec.end() - 5);
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