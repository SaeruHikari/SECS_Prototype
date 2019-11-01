// SECS_Prototype.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "TemplateUtils/TemplateUtils.hpp"
#include "TemplateUtils/TemplatePackUtils.hpp"
#include "Managers/SEntityManager.hpp"

using namespace std;
using namespace SECS;


int main()
{
	// SEntityManager::CreateEntity<CompTs...>();  return a valid entity.
	// Need to do:
	// 1. Entity manager: Create the entity. The entity value(index and generation).
	//    After creation of the index:
	// 2. Create or find an archetype. Archetype is a specific combination of components.
	//    This is used for chunk structure and combination infos(just like type).
	// 3. After creating the archetype, we will construct it:
	//    (1) If we already have one, find a valid chunk.
	//    (2) If not, create a new chunk list.
	//    (3) Place entity and create all components of template input in the chunk.
	// 4. Finish construction and work with freeChunkList pointer, etc... 
	SEntityManager::CreateEntity<ComponentA, ComponentB, ComponentC>();


	//(unimplemented)(4)! Case needs customized constructor: 
	//auto entity = SEntityManager::RegistryEntity<ComponentA, ComponentB, ComponentC>();
	//SEntityManager::ConsturctComponent<ComponentA>(params...);
	//SEntityManager::ConsturctComponent<ComponentB>(params...);

	return 0;
}

