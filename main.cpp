#include <vector>
#include <forward_list>
#include <list>
#include <iostream>
#include <algorithm>
#include "MyVector.h"
#include "MyForwardList.h"
#include "MyList.h"

void stdtest()
{
}

void test()
{
}

int main()
{
	try
	{
		std::vector<int> vec = { 1, 2, 3, 69, 17, 420 };
		MySTL::MyList<int> li(vec.begin(), vec.end());
		MySTL::MyList<int> li2 = { 18, 73, 44, 55 };
		for (auto& e : li)
			std::cout << e << " ";
		std::cout << "\n";
		for (auto& e : li2)
			std::cout << e << " ";
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}