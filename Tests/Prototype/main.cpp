// Copyright(c) 2021 Maxime "mrouffet" ROUFFET.All Rights Reserved.

#include <iostream>

#define LOG(_str) std::cout << _str << std::endl;

int main()
{
	LOG("Hello, World!");

#if !SA_CI

	std::cin.get();

#endif

	return 0;
}
