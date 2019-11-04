#pragma once
#include "../Types/SEntity.hpp"
#include "../Types/SSystem.hpp"
#include "../Types/SComponent.hpp"
#include <vector>
#include "SArcheTypeManager.hpp"
#include <functional>

namespace SECS
{
	class SEntityManager
	{
		friend class SWorld;
	private:
		SEntityManager()
		{

		}
		// Refers
		struct SEntityData
		{
			SChunk* Chunk;
			int IndexInChunk = -1;
			// Generation, this generation will NEVER update unless the exact data in chunk updates.  
			// Any generation else where different from this mains that entity not alive.
			int generation = -1;
		};

		static SEntityManager* _this;
		std::vector<SEntityData> m_EntityData;
		std::vector<SEntity> m_Entities;

	private:
		inline static SEntityManager* Get()
		{
			if (_this == nullptr)
			{
				_this = new SEntityManager();
				return _this;
			}
			else return _this;
		}

		template<typename ... Components>
		inline SEntity CreateEntity(SArcheTypeManager* arMng)
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << std::endl << "Start Create new entity: " << std::endl;
#endif
			// Create New Entity.
			SEntity NewEntity;
			NewEntity.Index = m_Entities.size();
			NewEntity.generation += 1;

			// Get archetype of this combination of components.
			auto* _archeType = arMng->GetArcheType<Components...>(&NewEntity);
			// Get chunk with the _archtype.
			auto* _chunk = arMng->GetFreeSChunk(_archeType);
#if defined(DEBUG) || defined(_DEBUG)
			assert(_chunk != nullptr);
#endif
			// Fill in the chunk.
			// Entity data:
			SEntityData NewEntityData;
			NewEntityData.Chunk = _chunk;
			NewEntityData.IndexInChunk = _chunk->ConstructionOnChunk<Components...>(std::move(NewEntity));
			NewEntityData.generation = NewEntity.generation;
			// Ensure pushing together
			m_EntityData.push_back(NewEntityData);
			m_Entities.push_back(NewEntity);

#if defined(DEBUG) || defined(_DEBUG)
			std::cout << std::endl << "End Create new entity: Index in chunk: " << NewEntityData.IndexInChunk << std::endl << std::endl;
#endif

			return NewEntity;
		}

		// Returns false when the entity is not found.
		inline bool DestoryEntity(SEntity entity)
		{
			if (m_EntityData.size() <= entity.Index) return false;
			if (!IsEntityAlive(entity)) return false;
			m_EntityData[entity.Index].generation += 1;
		
			return true;
		}

		// Check if an entity is alive.
		inline bool IsEntityAlive(SEntity entity)
		{
			if (m_EntityData.size() <= entity.Index) return false;
			return m_EntityData[entity.Index].generation == entity.generation;
		}

		template<typename ... Cs>
		inline void Each(std::function<void(SEntity, Cs *...)> func, SArcheTypeList arclist)
		{
			for (auto _type : arclist)
			{
				for (auto _chunk : *_type->chunks)
				{
					// Batch func call for better performance.
					// -----------------------------

					// !
					for (int i = 0; i < _chunk->properties->Count - _chunk->properties->FreeUnits; i++)
					{
						SEntity* entityPtr = _chunk->__getEntityPtr<SEntity>(i);
						if (entityPtr->generation == m_EntityData[entityPtr->Index].generation)
						{
							func(*entityPtr, _chunk->__getCompPtr<Cs>(i)...);
						}
					}
				}
			}
			// Only iterate alive entities.
		}

		template<typename ... Cs>
		inline void Each(std::function<void(SEntity, Cs*...)> func, SArcheTypeManager* arMng)
		{
			size_t countC = sizeof...(Cs);
			// Iterate ArcheTypes:
			SArcheTypeList arclist = arMng->CompsGetArcheTypes<Cs...>();
			// Deal with arclist.
			Each<Cs...>(func, arclist);
		}
	};
	SEntityManager* SEntityManager::_this = nullptr;
}
