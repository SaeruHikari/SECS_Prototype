#pragma once
#include "..\Managers\SArcheTypeManager.hpp"
#include "..\Managers\SEntityManager.hpp"
#include "..\Types\Containers\SSystemList.hpp"

namespace SECS
{
	class SWorld
	{
	public:
		SWorld()
		{
			ArcheTypeManager = SArcheTypeManager::Get();
			EntityManager = SEntityManager::Get();
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
		inline void Each(std::function<void(SEntity, Cs* ...)> func)
		{
			EntityManager->Each(func, ArcheTypeManager);
		}

		inline bool DestoryEntity(SEntity entity)
		{
			return EntityManager->DestoryEntity(entity);
		}

		inline bool IsEntityAlive(SEntity entity)
		{
			return EntityManager->IsEntityAlive(entity);
		}

		inline SEntityManager* GetEntityManager()
		{
			return EntityManager;
		}
		inline SArcheTypeManager* GetArcheTypeManager() 
		{
			return ArcheTypeManager;
		}
		template<typename ... Cs>
		inline SSystem* AddSystem()
		{

		}
	private:
		SArcheTypeManager* ArcheTypeManager = nullptr;
		SEntityManager* EntityManager = nullptr;
		SSystemList ActiveSystems;
		SSystemList DeactiveSystems;
	};
}