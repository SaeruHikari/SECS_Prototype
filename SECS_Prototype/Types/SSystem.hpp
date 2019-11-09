/*****************************************************************************************
							 Copyrights   SaeruHikari
Description:					System master class.
Details:
*******************************************************************************************/
#pragma once
#include "..\Managers\SArcheTypeManager.hpp"
#include "..\Managers\SEntityManager.hpp"
#include <functional>
#include <iostream>
#include "Containers\SSystemList.hpp"
// A system should: 
// 1. catch available components fast(Cache ArcheTypes ...)
// 2. catch other systems.
// 3. filter.
namespace SECS
{
	template<typename T>
	struct __SystemRegister 
	{
	public:
		__SystemRegister(std::string SysName, std::string GroupName)
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "REGISTER: " << SysName << " To " << GroupName << std::endl;
#endif // 
			SSystemGroup::AddSystemToRootSystems(GroupName, new T());
		}
	};

#define REGISTRY_SYSTEM_TO_ROOT_GROUP(__className, __groupName)\
	  struct __className##_##__groupName##_SytemRegister_Helper\
	 {\
          private:\
              static __SystemRegister<__className>* m_##__className##_##__groupName##_register;\
	 };\
     __SystemRegister<__className>* __className##_##__groupName##_SytemRegister_Helper::m_##__className##_##__groupName##_register = new __SystemRegister<__className>(#__className, # __groupName);\


#define DECLARE_SYSTEM_COMPONENTS(...) __VA_ARGS__

#define DECLARE_SYSTEM_UPDATE(__className, __funcAddress, __THIS, ...)\
private:\
ComponentSystemData< __className, __VA_ARGS__> SystemData = ComponentSystemData<__className, __VA_ARGS__>(__THIS, __funcAddress);\
public:\
	inline void Update(SEntityManager* EntityManager) \
	{\
		SystemData.__update_Internal(EntityManager);\
	}\
	inline void CollectSystemBoostInfos(SEntityManager* entm, SArcheTypeManager* arcm) \
	{\
		SystemData.__collectInfos_Internal(entm, arcm);\
	}\


	class ___SSystemBase
	{
		inline virtual void Update(SEntityManager* EntityManager) = 0;
		inline virtual void CollectSystemBoostInfos(SEntityManager* entm, SArcheTypeManager* arcm) = 0;
	};


	//REGISTRY_SYSTEM_TO_GROUP(SSystem, TestGroup);
	//DEF_SYSTEM_UPDATE(SSystem, &SSystem::Update_Implementation, this, DEF_SYSTEM_COMPONENTS(ComponentA, ComponentB));
	class SSystem : public ___SSystemBase
	{
		friend class SWorld;
	protected:
		// Template class to hold reflection informations.
		template<class T, typename ... Cs>
		struct ComponentSystemData
		{
			friend class SSystem;
			friend class SWorld;
			friend class SEntityManager;
			friend class SArcheTypeManager;
		public:
			ComponentSystemData(T* __this, void(T::* pFunc)(SEntity, Cs*...))
				:_this(__this), callback(pFunc)
			{
				hashes = TemplatePackUtils::GetHashes<Cs...>();
			}
			inline void __collectInfos_Internal(SEntityManager* entm, SArcheTypeManager* arcm)
			{
				auto res = arcm->CompsGetArcheTypes<Cs...>();
				if(arcm->CompsGetArcheTypes<Cs...>().size() > 0)
				_this->m_Types.insert(_this->m_Types.end(),
					res.begin(), res.end());
			}
			// Callback
			inline void __update_Internal(SEntityManager* EntityManager)
			{
				if(callback != nullptr)
				EntityManager->Each<T, Cs ...>(callback, _this, _this->m_Types);
			}
			// Collect
			inline void __collect_NewTypeInfo(SArcheType* _archetype)
			{
				if (_archetype->ComponentNum < sizeof...(Cs) || _archetype->typeHashes[0] > hashes[0]) return; // Discard.
				// If arche includes hashes:
				if (_archetype->Includes(sizeof...(Cs), hashes))
					_this->m_Types.push_back(_archetype);
			}
		public:
			size_t* hashes; 
			void(T::* callback)(SEntity, Cs*...) = nullptr;
			T* _this = nullptr;
			
		};
	private:
		// Properties
		SArcheTypeList m_Types;
		size_t RefSystemCount = -1;
		SSystem* RefSystems = nullptr;

	public:
		DECLARE_SYSTEM_UPDATE(SSystem, &SSystem::Update_Implementation, this, DECLARE_SYSTEM_COMPONENTS(ComponentA, ComponentB));
		inline void Update_Implementation(SEntity entity, ComponentA* a, ComponentB* b)
		{

		}

	public:
		SSystem()
		{

		}
		SSystem(SArcheTypeManager* _archeTypeManager, SEntityManager* _entityManager)
		{
			SystemData.__collectInfos_Internal(_entityManager, _archeTypeManager);
		}
		virtual ~SSystem() {};
	protected:
		static std::unordered_map<std::string, SSystem*> SystemGroups;
		static std::unordered_map<std::string, SSystem*> InitSystemGroups()
		{
			std::unordered_map<std::string, SSystem*> _Groups;
			return _Groups;
		}
	};
	std::unordered_map<std::string, SSystem*> SSystem::SystemGroups = SSystem::InitSystemGroups();
	//REGISTRY_SYSTEM_TO_ROOT_GROUP(SSystem, SECSDefaultGroup);
}