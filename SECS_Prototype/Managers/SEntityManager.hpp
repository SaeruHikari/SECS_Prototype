#pragma once
#include "../Types/SEntity.hpp"
#include "../Types/SSystem.hpp"
#include "../Types/SComponent.hpp"
#include "SArcheTypeManager.hpp"

namespace SECS
{
	class SEntityManager
	{
	public:
		template<typename ... Components>
		static SEntity CreateEntity()
		{
			// Create New Entity.
			SEntity NewEntity;
			// Get archetype of this combination of components.
			auto* arMng = SArcheTypeManager::Get();
			auto* _archeType = arMng->GetArcheType<Components...>(&NewEntity);
#if defined(DEBUG) | defined(_DEBUG)
			std::cout << _archeType << std::endl;
#endif
			// Get chunk with the _archtype.
			// Always init one now   !!!!!
			auto _chunk = arMng->GetFreeSChunk(_archeType);
			// Fill in the chunk.

			return NewEntity;
		}

	};
}
