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


void TestDestory()
{
	float time = ::GetTickCount64();
	float time2 = ::GetTickCount64();

	time = ::GetTickCount64();
	//29000 + 4000 (5000 ava) 16types
	world->Each<ComponentA, ComponentB>([=](SEntity entity, ComponentA* a, ComponentB* b) {
		world->DestoryEntity(entity);
		});
	time2 = ::GetTickCount64();
	std::cout << std::endl << time2 - time << std::endl << std::endl;
}


void TestIterate()
{
	SSystem* sys = new SSystem();

	int IterLoop = 50;
	::Sleep(1000);
	float time = ::GetTickCount64();
	float time2 = ::GetTickCount64();

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

	static int ii = 0;
	world->Each<ComponentA, ComponentB>([=](SEntity entity, ComponentA* a, ComponentB* b) {
		ii++;
		world->DestoryEntity(entity);
		//std::cout << entity.GetIndex() << std::endl;
		});
	world->Each<ComponentA, ComponentB>([=](SEntity entity, ComponentA* a, ComponentB* b) {
		ii++;
		//world->DestoryEntity(entity);
		//std::cout << entity.GetIndex() << std::endl;
		});
	world->Each<int, double>([=](SEntity entity, int* a, double* b) {
		ii++;
		world->DestoryEntity(entity);
		});

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


	int Loop = 2000;
	for(int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentA, ComponentB, ComponentC, ComponentD>(); //0
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentA, ComponentD>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentB>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentA, ComponentC, ComponentD>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<ComponentC>();
	for (int i = 0; i < Loop; i++)
		world->CreateEntity<char>();
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


	auto en0 = world->CreateEntity<ComponentA, ComponentB, ComponentC, int>();                         
	auto en1 = world->CreateEntity<ComponentB, int>();                         
	auto en2 = world->CreateEntity<double>();                         

	auto arc0 = world->FindArcheType(en0);
	auto arc1 = world->FindArcheType(en1);
	auto arc2 = world->FindArcheType(en2);


	world->AddSystemGroup("SECSDefaultGroup");

	world->GetEntityManager()->AddComponent<double>(en1, world->GetArcheTypeManager());

	world->TickSystemGroups();

	SDebugSystem* sys = new SDebugSystem();
	SSystemGroup::AddSystemToGroupList("SECSDefaultGroup", sys);

	world->TickSystemGroups();

	std::cout << arc0->Includes(arc1) << std::endl << std::endl;
	std::cout << arc2->Included(arc1) << std::endl << std::endl;

	

	TestIterate();


	//! DO NOT support multi component like this:
	//SEntityManager::CreateEntity<ComponentA, ComponentA, ComponentA>();
	

	//(unimplemented)(4)! Case needs customized constructor: 
	//auto entity = SEntityManager::RegistryEntity<ComponentA, ComponentB, ComponentC>();
	//SEntityManager::ConsturctComponent<ComponentA>(params...);
	//SEntityManager::ConsturctComponent<ComponentB>(params...);

	return 0;
}

