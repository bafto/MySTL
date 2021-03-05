#include <vector>
#include <iostream>
#include "MyVector.h"

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	std::vector<int> vec2;

	auto it = vec.begin();

	MySTL::MyVector<int> mvec({ 1, 2, 3, 4, 5 });
	MySTL::MyVector<int> mvec2({ 6, 7, 8, 9, 10 });

	mvec.insert(0, &mvec2.front(), &mvec2.back() + 1);
	
	for (int i = 0; i < mvec.size(); i++)
	{
		std::cout << mvec[i] << "\n";
	}

	return 0;
}