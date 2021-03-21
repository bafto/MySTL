#include <vector>
#include <forward_list>
#include <iostream>
#include <algorithm>
#include "MyVector.h"
#include "MyForwardList.h"

MySTL::MyForwardList<int> l = { 1, 2, 7, 5, 1, 1, 23, 15, 69, 1 };
MySTL::MyForwardList<int> l2 = { 3, 1, 18, 9 };
std::forward_list<int> li = { 7, 5, 6, 2, 38, 22, 34 };
std::forward_list<int> li2 = { 5, 6,7, 2, 3, 2, 7, 22, 1, 33 };

void stdtest()
{
	for (int j = 0; j < 100000; j++)
	{
		li.insert_after(li.begin(), 69);
	}
	li.erase_after(li.before_begin(), li.end());
}

void test()
{
	for (int i = 0; i < 100000; i++)
	{
		l.insert_after(l.before_begin(), 69);
	}
	while (!l.empty())
	{
		l.pop_front();
	}
}

int main()
{
	try
	{
		char c = 'n';
		while (c == 'n')
		{
			test();
			test();
			test();
			test();
			for (auto& e : l)
				std::cout << e << " ";
			std::cout << "\n";
			std::cin >> c;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "\n" << e.what() << "\n";
	}

	return 0;
}