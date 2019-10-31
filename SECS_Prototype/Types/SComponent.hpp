#pragma once

struct SComponent {
	SComponent() {
		std::cout << "Data Com: ";
	}

	virtual ~SComponent() {

	}
};

// Debug
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
};