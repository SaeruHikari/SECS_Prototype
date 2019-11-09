#pragma once
#include "SSystem.hpp"

namespace SECS
{
	class SDebugSystem : public SSystem
	{
		DECLARE_SYSTEM_UPDATE(SDebugSystem, &SDebugSystem::Update_Implementation, this, DECLARE_SYSTEM_COMPONENTS(ComponentA, ComponentB, ComponentC));
	public:
		SDebugSystem()
		{
			
		}
		inline void Update_Implementation(SEntity entity, ComponentA* a, ComponentB* b, ComponentC* c)
		{
			 c->i += 15;
		}

	};
	REGISTRY_SYSTEM_TO_ROOT_GROUP(SDebugSystem, SECSDefaultGroup1);

}
