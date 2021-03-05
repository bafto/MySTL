#include <vector>
#include <iostream>
#include "MyVector.h"

int main()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5 };
	std::vector<int> vec2;

	MySTL::MyVector<int> mvec({ 1, 2, 3, 4, 5 });
	MySTL::MyVector<int> mvec2 = { 6, 7, 8, 9, 10 };

	int& i = mvec[0];
	
	mvec.swap(mvec2);

	std::cout << i;
	return 0;
}