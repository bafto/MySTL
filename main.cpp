#include <vector>
#include <iostream>
#include <algorithm>
#include "MyVector.h"
#include <map>

int main()
{
	try
	{
		MySTL::MyVector<int> vec = { 1, 2, 3, 4, 5 };
		vec.insert(vec.begin(), 2);
		for (auto& e : vec)
			std::cout << e << " ";
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}