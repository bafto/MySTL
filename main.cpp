#include <vector>
#include <iostream>
#include <algorithm>
#include "MyVector.h"

int main()
{
	try
	{
		MySTL::MyVector<int> vec = { 1, 2, 2, 2, 3, 4, 5 };
		MySTL::MyVector<int> vec2 = { 5, 7, 8, 9, 10 };
		vec2.insert(vec2.begin(), vec.begin(), vec.end());
		vec2.reverse();
		vec2.forEach([](int& e) {
			std::cout << e << " ";
			});
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}