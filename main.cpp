#include <vector>
#include <forward_list>
#include <iostream>
#include <algorithm>
#include "MyVector.h"
#include "MyForwardList.h"

MySTL::MyForwardList<int> l = { 7, 8, 9, 10 };
MySTL::MyForwardList<int> l2 = { 0, 1, 2, 3, 4, 5 };
std::forward_list<int> li = { 7, 5, 6, 2, 38, 22, 34 };
std::forward_list<int> li2 = { 5, 6,7, 2, 3, 2, 7, 22, 1, 33 };

void stdtest()
{
	for (int j = 0; j < 1000; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			li.insert_after(li.before_begin(), i);
		}
	}
}

void test()
{
	for (int i = 0; i < 10; i++)
		l2.insert_after(l2.before_begin(), i);
	l2.erase_after(l2.begin(), l2.end());
}

int main()
{
	try
	{
		
		for (auto& e : l2)
			std::cout << e << " ";
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}