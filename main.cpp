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
		MySTL::MyList<int> li({ 1, 2 ,3 , 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
		MySTL::MyList<int> li2 = { 18, 73, 44, 55 };
		size_t i = 0;
		li.swap(li2);
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