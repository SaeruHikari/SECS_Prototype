#pragma once
#include <map>
#include "SSystem.hpp"

namespace SECS
{
	class SSystemGroup : public SSystem
	{
#define ADD_SYSTEM_SUBGROUP(__className, __groupName)\
	SubSys[__groupName] = new __className();\

		friend SWorld;
	public:
		SSystemGroup()
		{
			ADD_SYSTEM_SUBGROUP(SSystem, "MSystem");
			ADD_SYSTEM_SUBGROUP(SDebugSystem, "DSystem");
		}

	private:
		static SSystem* GetRootSystems(std::string GroupName)
		{
			if (SystemGroups.find(GroupName) != SystemGroups.end())
				return SystemGroups[GroupName];
			else return nullptr;
		}
	public:
		static void AddSystemToRootSystems(std::string GroupName, SSystem* sys)
		{
			SystemGroups[GroupName] = sys;
		}
		inline void Update(SEntityManager* _entityManager)
		{
			auto iter = SubSys.begin();
			while (iter != SubSys.end())
			{
				iter->second->Update(_entityManager);
				iter++;
			}
		}
		inline void CollectSystemBoostInfos(SEntityManager* entm, SArcheTypeManager* arcm) 
		{
			auto iter = SubSys.begin();
			while (iter != SubSys.end())
			{
				iter->second->CollectSystemBoostInfos(entm, arcm);
				iter++;
			}
		}
	private:
		std::unordered_map<std::string, SSystem*> SubSys;
	};
	REGISTRY_SYSTEM_TO_ROOT_GROUP(SSystemGroup, SECSDefaultGroup);
}


