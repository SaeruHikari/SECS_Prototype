// SECS_Prototype.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "Types/SArcheType.hpp"
#include "TemplateUtils/TemplateUtils.hpp"

using namespace std;

int main()
{
	// Test Arche Type.
	SArcheType* Arche = new SArcheType();
	Arche->Init<float, double, std::string>();

	// Test Arche Template Util.
	TemplateUtils::ArcheTemplate<float, int, float> matcher;
	std::cout << std::endl << matcher.match<int, float, float>();

	return 0;
}

