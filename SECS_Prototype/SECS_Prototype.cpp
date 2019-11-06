// SECS_Prototype.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#if defined(DEBUG) 
#undef DEBUG
#endif
#if defined(_DEBUG) 
#undef _DEBUG
#endif

#define _ITERATOR_DEBUG_LEVEL 0
#include <iostream>
#include <windows.h>
#include "TemplateUtils/TemplateUtils.hpp"
#include "TemplateUtils/TemplatePackUtils.hpp"
#include "Managers/SEntityManager.hpp"
#include <sysinfoapi.h>
#include <synchapi.h>
#include "Types/SWorld.hpp"


using namespace std;
using namespace SECS;

SWorld* world;
std::vector<Actor*> actors;
void TestNew()
{
	::Sleep(1000);
	double time3 = ::GetTickCount64();
	//
	for (int i = 0; i < 1000; i++)
	{
		actors.push_back(new Actor());
		actors.push_back(new Actor());
		actors.push_back(new Actor());
		actors.push_back(new Actor());
	}
	double time4 = ::GetTickCount64();
	std::cout << time4 - time3 << std::endl;

	for (int i = 0; i < 1000; i++)
	{
		actors.push_back(new Actor());
	}

	for (int i = 0; i < 2000; i++)
	{
		actors.push_back(new Actor(1));
		actors.push_back(new Actor(2));
		actors.push_back(new Actor(3));
		actors.push_back(new Actor(4));
		actors.push_back(new Actor(5));
		actors.push_back(new Actor(6));
		actors.push_back(new Actor(7));
		actors.push_back(new Actor(8));
		actors.push_back(new Actor(9));
		actors.push_back(new Actor(10));
		actors.push_back(new Actor(11));
		actors.push_back(new Actor(12));
		actors.push_back(new Actor(13));
		actors.push_back(new Actor(14));
		actors.push_back(new Actor(15));
	}


	double time = ::GetTickCount64();
	//29000 + 4000 (5000 ava) 16types
	for (int i = 0; i < 1000; i++)
	{
		world->CreateEntity<ComponentA, ComponentB, ComponentC, ComponentD>();
		world->CreateEntity<ComponentA, ComponentB, ComponentC, ComponentD>();
		world->CreateEntity<ComponentA, ComponentB, ComponentC, ComponentD>();
		world->CreateEntity<ComponentA, ComponentB, ComponentC, ComponentD>();
		//SEntityManager::CreateEntity<float>();
	}
	double time2 = ::GetTickCount64();
	std::cout << time2 - time << std::endl << std::endl;
}

void TestDestory()
{
	double time = ::GetTickCount64();
	//29000 + 4000 (5000 ava) 16types
	for (int i = 0, j = 0; i < 4000; j++)
	{
		if (actors[i]->ID == 0)
		{
			delete[] actors[i];
			i++;
		}	
	}
	double time2 = ::GetTickCount64();
	std::cout << time2 - time << std::endl << std::endl;

	time = ::GetTickCount64();
	//29000 + 4000 (5000 ava) 16types
	static int ii = 0;
	world->Each<ComponentA, ComponentB>([=](SEntity entity, ComponentA* a, ComponentB* b) {
		world->DestoryEntity(entity);
		});
	time2 = ::GetTickCount64();
	std::cout << std::endl << time2 - time << std::endl << std::endl;
}

inline void AB(SEntity entity, ComponentA* a, ComponentB* b)
{
	a->x += 5;
	a->y += 5;
	a->z += 5;
}

void TestIterate()
{
	SSystem* sys = new SSystem();

	int IterLoop = 50;

	::Sleep(1000);
	float time = ::GetTickCount64();
	for (int i = 0; i < IterLoop; i++)
	{
		for (int j = 0; j < actors.size(); j++)
		{
			if (actors[j]->ID != 0) continue;
			actors[j]->compA->x += 5;
			actors[j]->compA->y += 5;
			actors[j]->compA->z += 5;
		}
	}
	float time2 = ::GetTickCount64();
	std::cout << time2 - time << std::endl;

	::Sleep(1000);
	time = ::GetTickCount64();
	for (int i = 0; i < IterLoop; i++)
	{
		world->TickSystemGroups();
	}
	 time2 = ::GetTickCount64();
	std::cout << time2 - time << std::endl;
	auto timea = time2 - time;

	
	::Sleep(1000);
	time = ::GetTickCount64();
	for (int i = 0; i < IterLoop; i++)
	{
		world->Each<ComponentA, ComponentB>([=](SEntity entity, ComponentA* a, ComponentB* b) {
			a->x += 5;
			a->y += 5;
			a->z += 5;
		});
	}
	time2 = ::GetTickCount64();
	std::cout << time2 - time << std::endl;
	std::cout << timea/(time2 - time) << std::endl;
}

int main()
{

	// SEntityManager::CreateEntity<CompTs...>();  return a valid entity.
	// Need to do:
	// 1. Entity manager: Create the entity. The entity value(index and generation).
	//    After creation of the index:														√
	// 2. Create or find an archetype. Archetype is a specific combination of components.
	//    This is used for chunk structure and combination infos(just like type).			√
	// 3. After creating the archetype, we will construct it:								√
	//    (1) If we already have one, find a valid chunk.									√
	//    (2) If not, create a new chunk list.												√	
	//    (3) Place entity and create all components of template input in the chunk.		√
	// 4. Finish construction and work with freeChunkList pointer, etc...					√
	world = SWorld::CreateSWorld("TestWorld");

	//TestNew();

	int Loop = 100000;
	for(int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentA, ComponentB, ComponentC, ComponentD>(); //0
	for (int i = 0; i < Loop; i++)
		auto en1 = world->CreateEntity<ComponentA, ComponentD>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentA, double>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentB>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<char, double, std::string, int>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentA, ComponentC, ComponentD>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentC>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<char>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<double>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentD>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentE>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentF>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentG>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentH>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentI>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentB, ComponentB, int>();                         //6


	world->AddSystemGroup("SECSDefautGroup");

	TestIterate();

	//TestDestory();

	//! DO NOT support multi component like this:
	//SEntityManager::CreateEntity<ComponentA, ComponentA, ComponentA>();
	

	//(unimplemented)(4)! Case needs customized constructor: 
	//auto entity = SEntityManager::RegistryEntity<ComponentA, ComponentB, ComponentC>();
	//SEntityManager::ConsturctComponent<ComponentA>(params...);
	//SEntityManager::ConsturctComponent<ComponentB>(params...);

	return 0;
}

