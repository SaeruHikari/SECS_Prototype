#pragma once
#include <iostream>

namespace SECS
{
	struct SComponent {
		SComponent() {
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "Data Com: ";
#endif
		}

		virtual ~SComponent() {

		}
	};

	// Debug use!
	struct ComponentA : public SComponent
	{
		ComponentA()
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
#endif
		}
		int a = 5;
	};

	struct ComponentB : public SComponent
	{
		ComponentB()
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
#endif
		}
		float b = 15;
	};

	struct ComponentC : public SComponent
	{
		ComponentC()
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
#endif
		}
		char c[4] = "WTF";
		int i = 8;
		std::string str;
	};

	struct ComponentD : public SComponent
	{
		ComponentD()
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
#endif
		}
		char c[4] = "WTF";
		int i = 8;
		std::string str;
		std::string str1;
		std::string str2;
		std::string str3;
		std::string str4;
		std::string str6;
		std::string str7;
		std::string str213;
		std::string str5;
	};

	struct Actor
	{
#define Actor_Components ComponentA, ComponentB, ComponentC, ComponentD
		Actor()
		{
			compA = new ComponentA();
			compB = new ComponentB();
			compC = new ComponentC();
			compD = new ComponentD();
		}
		ComponentA* compA;
		ComponentB* compB;
		ComponentC* compC;
		ComponentD* compD;
	};
}
