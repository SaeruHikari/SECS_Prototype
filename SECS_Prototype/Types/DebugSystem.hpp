#pragma once
#include "SSystem.hpp"

namespace SECS
{
	class SDebugSystem : public SSystem
	{
		DEF_SYSTEM_UPDATE(SDebugSystem, &SDebugSystem::Update_Implementation, this, DEF_SYSTEM_COMPONENTS(ComponentA, ComponentB, ComponentC));
	public:
		SDebugSystem()
		{
			
		}
		 void Update_Implementation(SEntity entity, ComponentA* a, ComponentB* b, ComponentC* c)
		{
			//std::cout << a->x << "   " << c->c << std::endl;
		}

	};
	REGISTRY_SYSTEM_TO_GROUP(SDebugSystem, TestGroup);


	class SDebugChildSystem : public SDebugSystem
	{
		DEF_SYSTEM_UPDATE(SDebugChildSystem, &SDebugChildSystem::Update_Implementation, this, DEF_SYSTEM_COMPONENTS(ComponentA, ComponentB, ComponentC));
	public:
		SDebugChildSystem()
		{

		}
		void Update_Implementation(SEntity entity, ComponentA* a, ComponentB* b, ComponentC* c) 
		{
			std::cout << a->x << "   " << c->i << std::endl;
		}
	};
	REGISTRY_SYSTEM_TO_GROUP(SDebugChildSystem, TestGroup);
}
