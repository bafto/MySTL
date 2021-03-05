#include <vector>
#include <iostream>
#include "MyVector.h"

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	std::vector<int> vec2(std::move(vec));
	MySTL::MyVector<int> mvec;
	MySTL::MyVector<int> mvec2(4);
	for (int i = 0; i < mvec2.size(); i++)
	{
		mvec2[i] = i;
	}

	mvec = mvec2;
	mvec = std::move(mvec2);

	MySTL::MyVector<int> mvec3(mvec);

	mvec[0] = std::move(mvec3[1]);
	for (int i = 0; i < mvec.size(); i++)
	{
		std::cout << mvec[i] << "\n";
	}
	std::cout << "\n";
	for (int i = 0; i < mvec3.size(); i++)
	{
		std::cout << mvec3[i] << "\n";
	}

	try
	{
		std::cout << mvec[7];
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}