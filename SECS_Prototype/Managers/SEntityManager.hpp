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

			// Get chunk with the _archtype.
			// Always init one now   !!!!!
			auto* _chunk = arMng->GetFreeSChunk(_archeType);
			// Fill in the chunk.
			__constructOnChunk_Internal<Components...>(_chunk);

			return NewEntity;
		}
	private:
		template<typename T>
		static void __constructOnChunk_Internal(SChunk* _chunk)
		{
			_chunk->ConstructOnChunk<T>();
		}
		template<typename T1, typename T2, typename... Ts>
		static void __constructOnChunk_Internal(SChunk* _chunk)
		{
			__constructOnChunk_Internal<T1>(_chunk);
			__constructOnChunk_Internal<T2, Ts...>(_chunk);
		}
	};
}
