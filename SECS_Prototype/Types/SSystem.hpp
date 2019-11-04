/*****************************************************************************************
							 Copyrights   SaeruHikari
Description:					System master class.
Details:
*******************************************************************************************/
#pragma once
#include "..\Managers\SArcheTypeManager.hpp"
#include "..\Managers\SEntityManager.hpp"

namespace SECS
{
	// A system should: 
	// 1. catch available components fast(Cache ArcheTypes ...)
	// 2. catch other systems.
	// 3. filter.
	class SSystem
	{
	public:
		SSystem(){	}
		virtual ~SSystem() {};
	public:
		virtual void SUpdate() = 0;

		template<typename ... Cs>
		void Each(Cs*... cs)
		{
			
		}

		template<typename ... Cs>
		inline bool Init_CatchComponents(SArcheTypeManager* _ArcheManager)
		{
			// Cache available archetypes for quick search:
			m_Types = _ArcheManager->CompsGetArcheTypes<Cs...>();
			
			return (m_Types.size() != 0);
		}
		template<typename ... Ss>
		inline bool Init_RefSystems()
		{
			RefSystemCount = sizeof...(Ss);
			RefSystemHashes = TemplatePackUtils::GetHashes<Ss>();
			return (RefSystemCount != 0);
		}
	private:
		SArcheTypeList m_Types;
		
		size_t RefSystemCount = -1;
		size_t* RefSystemHashes = nullptr;
		// Get<SysClass>->SysClass.typeid.hash->iterate and get exact sys->static cast.
		SSystem* RefedSystems = nullptr;

		SArcheTypeManager* ArcheManager = nullptr;
	};
}