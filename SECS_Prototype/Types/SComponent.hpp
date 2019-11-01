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
#if defined(DEBUG) || defined(_DEBUG)
	// Debug use!
	struct ComponentA : public SComponent
	{
		ComponentA()
		{
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
		}
		int a = 5;
	};

	struct ComponentB : public SComponent
	{
		ComponentB()
		{
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
		}
		float b = 5;
	};

	struct ComponentC : public SComponent
	{
		ComponentC()
		{
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
		}
		char c[4] = "WTF";
		int i = 8;
		std::string str;
	};

	struct ComponentD : public SComponent
	{
		ComponentD()
		{
			std::cout << typeid(*this).name() << " Size: " << sizeof(*this) << std::endl;
		}
		char c[4] = "WTF";
		int i = 8;
		std::string str;
		std::string str1;
		std::string str2;
		std::string str3;
	};
#endif
}
