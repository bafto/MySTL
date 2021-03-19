#include <vector>
#include <forward_list>
#include <iostream>
#include <algorithm>
#include "MyVector.h"
#include "MyForwardList.h"

MySTL::MyForwardList<int> l({ 69, 5, 2, 6, 2 });
MySTL::MyForwardList<int> l2 = { 2, 2, 3 };
std::forward_list<int> li = { 1, 2 };

void stdtest()
{
	for (int j = 0; j < 1000; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			li.insert_after(li.before_begin(), i);
		}
		li.clear();
	}
}

void test()
{

	for (int j = 0; j < 1000; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			l2.insert_after(l2.before_begin(), i);
		}
		l2.clear();
	}
}

int main()
{
	try
	{
		li.
		stdtest();
		test();
		for (auto& e : l2)
			std::cout << e << " ";
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}