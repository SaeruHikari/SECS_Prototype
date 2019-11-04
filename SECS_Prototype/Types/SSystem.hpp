/*****************************************************************************************
							 Copyrights   SaeruHikari
Description:					System master class.
Details:
*******************************************************************************************/
#pragma once
#include "..\Managers\SArcheTypeManager.hpp"
#include "..\Managers\SEntityManager.hpp"
#include <functional>

namespace SECS
{
	// A system should: 
	// 1. catch available components fast(Cache ArcheTypes ...)
	// 2. catch other systems.
	// 3. filter.
#define DEF_SYSTEM_COMPONENTS(...) __VA_ARGS__
#define DEF_SYSTEM_UPDATE(__className, __funcAddress, __THIS, ...)\
private:\
ComponentSystemData<__className, __VA_ARGS__> SystemData = ComponentSystemData<__className, __VA_ARGS__>(__THIS, __funcAddress);\
public:\

	class SSystem
	{
		friend class SEntityManager;
		friend class SArcheTypeManager;
	private:
		SArcheTypeManager* ArcheTypeManager = nullptr;
		SEntityManager* EntityManager = nullptr;
	public:
		SSystem(SArcheTypeManager* _archeTypeManager, SEntityManager* _entityManager)
			:ArcheTypeManager(_archeTypeManager),
			EntityManager(_entityManager)
		{
			SystemData.InitTypeArray(_archeTypeManager);
		}
		virtual ~SSystem() {};
	private:
		// Template class to hold reflection informations.
		template<class T, typename ... Cs>
		struct ComponentSystemData
		{
			friend class SSystem;
			friend class SEntityManager;
			friend class SArcheTypeManager;
		public:
			ComponentSystemData(T* __this, void(T::* pFunc)(SEntity, Cs*...))
				:_this(__this), callback(pFunc)
			{

			}
			inline void InitTypeArray(SArcheTypeManager* arcm)
			{
				m_Types = arcm->CompsGetArcheTypes<Cs...>();
			}
			// Callback
			inline void __update_Internal()
			{
				_this->EntityManager->Each<T, Cs ...>(callback, _this, m_Types);
			}
		public:
			std::function<void(T*, SEntity, Cs *...)> callback;
			T* _this = nullptr;
			// To fetch at initial time.
			SArcheTypeList m_Types;
			size_t RefSystemCount = -1;
			SSystem* RefSystems = nullptr;
		};

		DEF_SYSTEM_UPDATE(SSystem, &SSystem::Update_Implementation, this, DEF_SYSTEM_COMPONENTS(ComponentA, ComponentB));

	public:
		inline void Update_Implementation(SEntity entity, ComponentA* a, ComponentB* b)
		{
			//std::cout << typeid(*a).name() << "    " << typeid(*b).name();
		}

		inline void Update()
		{
			SystemData.__update_Internal();
		}
	};
}