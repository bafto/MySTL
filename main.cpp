#include <vector>
#include <iostream>
#include "MyVector.h"

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	std::vector<int> vec2;

	auto it = vec.begin();

	MySTL::MyVector<std::string> mvec({ "1", "2", "3", "hallo", "5" });
	MySTL::MyVector<int> mvec2({ 6, 7, 8, 9, 10 });

	try
	{
		for (auto it = mvec.cbegin(); it < mvec.cend(); ++it)
		{
			std::cout << *it << " ";
		}

		std::cout << "\n";
		std::cout << mvec.cend() - mvec.cbegin();
	}
	catch (MySTL::MyVector<int>::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}
	return 0;
}