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
		std::list<int> slist = { 1, 2, 3, 4, 5 };
		MySTL::MyList<int> li({ 1, 2, 69, 69, 69, 69, 69, 3, 4, 5, 69, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
		MySTL::MyList<int> li2 = { 18, 69, 73, 44, 55, 69, 69 };
		size_t i = 0;
		li.splice(li.end(), li2);
		li.unique();
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