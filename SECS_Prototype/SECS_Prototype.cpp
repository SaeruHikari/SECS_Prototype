// SECS_Prototype.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "SEntity.hpp"

using namespace std;

int main()
{
	int i = 0;
	double d = 0;
	SEntity entity;
	SEntity* sEN = new SEN();
	SEN* sss = new SSS();
	cout << typeid(i).name() << endl;
	cout << typeid(d).name() << endl;
	cout << typeid(entity).name() << endl;
	cout << typeid(sEN).name() << endl;
	cout << typeid(*sEN).name() << endl;
	cout << typeid(sss).name() << endl;
	cout << typeid(*sss).name() << endl;
}

