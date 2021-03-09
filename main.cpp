#include <vector>
#include <iostream>
#include "MyVector.h"

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	std::vector<int> vec2;
	std::vector<std::string> vec3 = { "Hunde", "Katzen", "Mäuse" };
	MySTL::MyVector<std::string> mvec({ "1", "2", "3", "4", "5" });
	MySTL::MyVector<std::string> mvec2({ "Thunfisch", "Sardinen", "Lachs" });

	try
	{
		std::cout << *mvec.erase(mvec.begin(), mvec.end()) << "\n";
		for (auto& e : mvec)
			std::cout << e << " ";
		std::cout << "\n" << mvec.size();
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}