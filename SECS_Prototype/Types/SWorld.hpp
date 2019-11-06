#pragma once
#include "..\Managers\SArcheTypeManager.hpp"
#include "..\Managers\SEntityManager.hpp"
#include "Types/DebugSystem.hpp"



namespace SECS
{
	class SWorld
	{
		friend SSystemGroup;
	public:
		SWorld() {};
		SWorld(std::string _Name)
			:Name(_Name)
		{
			ArcheTypeManager = new SArcheTypeManager();
			EntityManager = new SEntityManager();
		}

		static SWorld* CreateSWorld(std::string _Name)
		{
			SWorld::Worlds[_Name] = new SWorld(_Name);
			return Worlds[_Name];
		}

		static SWorld* GetWorld(std::string _Name)
		{
			return Worlds[_Name];
		}

		template<typename ... Cs>
		inline static SEntity CreateEntity(SWorld* world)
		{
			return world->CreateEntity<Cs...>();
		}

		template<typename ... Cs>
		inline SEntity CreateEntity()
		{
			SEntity _entity = EntityManager->CreateEntity<Cs...>(ArcheTypeManager);
			_entity.world = this;
			return _entity;
		}

		template<typename ... Cs>
		inline void Each(std::function<void(SEntity, Cs* ...)> func) noexcept
		{
			EntityManager->Each(func, ArcheTypeManager);
		}

		inline bool DestoryEntity(SEntity& entity) 
		{
			return EntityManager->DestoryEntity(entity);
		}

		inline bool IsEntityAlive(const SEntity& entity)
		{
			return EntityManager->IsEntityAlive(entity);
		}

		inline SArcheType* FindArcheType(const SEntity& entity)
		{
			return EntityManager->FindArcheType(entity);
		}

		template<typename T>
		inline T* FindComponent(const SEntity& entity)
		{
			return EntityManager->FindComponent<T>(entity);
		}

		inline SEntityManager* GetEntityManager()
		{
			return EntityManager;
		}
		inline SArcheTypeManager* GetArcheTypeManager() 
		{
			return ArcheTypeManager;
		}

		template<typename S>
		inline void AddSystem()
		{
			SSystem* sys = new S();
			sys->SystemData.__collectInfos_Internal(EntityManager, ArcheTypeManager);
			ActiveSystemsLists.push_back(sys);
		}


		inline bool AddSystemGroup(std::string SystemGroupName)
		{
			 SSystemList* listPtr= SSystemGroup::GetSystemGroupList(SystemGroupName);
#if defined(DEBUG) || defined(_DEBUG)
			 if (listPtr == nullptr) std::cout << "System group trying to load do NOT EXIST." << std::endl;
#endif
			 if (listPtr == nullptr) return false;
			 SystemGroups.push_back(SystemGroupName);
#if defined(DEBUG) || defined(_DEBUG)
			 std::cout << "World " << Name << " added system group: " << SystemGroupName << " into tick group!" << std::endl;
#endif
			 for (int i = 0; i < listPtr->size(); i++)
			 {
				 (*listPtr)[i]->CollectSystemBoostInfos(EntityManager, ArcheTypeManager);
			 }
			 return true;
		}

		// Tick function, merge it into your message loop.
		inline void TickSystemGroups()
		{
			for (int i = 0; i < SystemGroups.size(); i++)
			{
				for (int j = 0; j < SSystemGroup::SystemGroups[SystemGroups[i]].size(); j++)
				{
					SSystemGroup::SystemGroups[SystemGroups[i]][j]->Update(EntityManager);
				}
			}
		}

	private:
		static std::unordered_map<std::string, SWorld*> Worlds;
		static std::unordered_map<std::string, SWorld*> __InitWorldMap()
		{
			std::unordered_map<std::string, SWorld*> _worlds;
			return _worlds;
		}
	private:
		std::string Name;
		SArcheTypeManager* ArcheTypeManager = nullptr;
		SEntityManager* EntityManager = nullptr;
		std::vector<std::string> SystemGroups;
	};
	std::unordered_map<std::string, SWorld*> SWorld::Worlds = SWorld::__InitWorldMap();
}