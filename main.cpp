#include <vector>
#include <iostream>
#include <algorithm>
#include "MyVector.h"

int main()
{
	try
	{
		std::vector<int> svec = { 6, 7, 8, 9, 10 };

		MySTL::MyVector<int> vec = { 1, 2, 3, 4, 5 };
		MySTL::MyVector<int> vec2 = { 1, 2, 3, 4, 5 };
		std::cout << *vec.insert(vec.end(), vec2.begin(), vec2.end()) << "\n";
		for (auto& e : vec)
			std::cout << e << " ";
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}