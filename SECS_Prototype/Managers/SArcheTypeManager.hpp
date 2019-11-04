/*****************************************************************************************
				             Copyrights   SaeruHikari
Description:	          Arche Type Manager, Create and manage Arche types.
Details:    A specific combine of components on a single entity is called the ArcheType.
*******************************************************************************************/
#pragma once
#include "../Types/SArcheType.hpp"
#include "../Types/SChunk.hpp"
#include "../Types/Containers/SArcheTypeList.hpp"
#include <unordered_map>
#include "../Types/Containers/SChunkList.hpp"

namespace SECS
{
	class SArcheTypeManager
	{
		friend class SEntityManager;
		friend class SWorld;
	private:
		static SArcheTypeManager* Get()
		{
			if (_this == nullptr)
			{
				_this = new SArcheTypeManager();
			}
			return _this;
		}
	protected:
		// members.
		// this
		static SArcheTypeManager* _this;
		// ArcheType Map: <compcount, ArchetypeList>
		std::unordered_map<size_t, SArcheTypeList> m_TypeLookup;

	protected:
		// No filter version.
		template<typename ... Cs>
		inline SArcheTypeList CompsGetArcheTypes()
		{
			SArcheTypeList resList;
			size_t* InHashes = TemplatePackUtils::GetHashes<Cs...>();
			size_t  InComps = sizeof...(Cs);
			// Iterate around map.
			for (auto _pair : m_TypeLookup)
			{
				if(_pair.first < InComps) continue;
				// Iterate
				size_t FocusHash = 0;
				for (int i = 0; i < _pair.second.size(); i++)
				{
					if (_pair.second[i]->typeHashes[0] > InHashes[0])
					{
						continue;
					}
					for (int j = 0; j < _pair.second[i]->ComponentNum; j++)
					{
						size_t test0 = _pair.second[i]->typeHashes[j];
						size_t test1 = InHashes[FocusHash];
						if (_pair.second[i]->typeHashes[j] == InHashes[FocusHash])
						{
							FocusHash += 1;
						}
						// All match, return this archetype.
						if (FocusHash == InComps)
						{
							resList.push_back(_pair.second[i]);
							break;
						}
						// ArcheType Hash already bigger than in hash tail, break.
						if(_pair.second[i]->typeHashes[j] > InHashes[InComps-1])
							break;
						// else do nothing and step in next loop.
					}
				}
			}
			return std::move(resList);
		}

		template<typename ... Cs>
		inline SArcheType* GetArcheType(SEntity* e)
		{
			// Finding...
			SArcheType* _archetype = GetExistingArcheType<Cs...>(e);
			if (_archetype != nullptr)
			{
#if defined(DEBUG) || defined(_DEBUG)
				std::cout << "Found existed archeType, reuse it." << std::endl;
#endif
				return _archetype;
			}

			// Not find
			_archetype = CreateArcheType<Cs...>(e);
			return _archetype;
		}

		template<typename ... Cs>
		inline SArcheType* GetExistingArcheType(SEntity* e)
		{
			SArcheType* _archetype = nullptr;
			size_t compCount = sizeof...(Cs);
			if (m_TypeLookup.find(compCount) != m_TypeLookup.end())
			{
				// Match
				for (auto* e : m_TypeLookup[compCount])
				{
					if (TemplatePackUtils::match<Cs...>(e->typeHashes, e->ComponentNum))
						_archetype = e;
					return _archetype;
				}
			}
			return nullptr;
		}

		template<typename ... Cs>
		inline SArcheType* CreateArcheType(SEntity* e)
		{
			SArcheType* Arche = new SArcheType();
			Arche->Init<Cs...>(e);	
			// found list
			if (m_TypeLookup.find(Arche->ComponentNum) != m_TypeLookup.end())
			{
				m_TypeLookup[Arche->ComponentNum].push_back(Arche);
			}
			// Not found, init new list and refers to the list.
			else
			{
				SArcheTypeList NewList;
				NewList.push_back(Arche);
				m_TypeLookup[Arche->ComponentNum] = NewList;
			}
			return Arche;
		}


		SChunk* GetFreeSChunk(SArcheType* arcType)
		{
			SChunk* _chunk = nullptr;
			if (arcType->chunks == nullptr)
			{
#if defined(DEBUG) || (_DEBUG)
				std::cout << "No free chunk, construct Chunk: ";
#endif
				_chunk = new SChunk();
				_chunk->InitChunkLayout(arcType);
				SChunkList* chunklist = new SChunkList();
				_chunk->properties->ListIndex = chunklist->size();
				chunklist->push_back(_chunk);
				arcType->chunks = chunklist;
				arcType->freeChunk = _chunk;
			}	
			else if (arcType->freeChunk == nullptr)
			{
				_chunk = new SChunk();
				_chunk->InitChunkLayout(arcType);
				_chunk->properties->ListIndex = arcType->chunks->size();
				arcType->chunks->push_back(_chunk);
				arcType->freeChunk = _chunk;
			}
			else
			{
#if defined(DEBUG) || (_DEBUG)
				std::cout << "Found free chunk, construct on it: " << std::endl;
#endif
				_chunk = arcType->freeChunk;
			}
			return _chunk;
		};


	protected:
		SArcheTypeManager()
		{
			assert(_this == nullptr);
		}

	};
	SArcheTypeManager* SArcheTypeManager::_this = nullptr;
}
