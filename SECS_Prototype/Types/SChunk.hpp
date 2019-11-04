/*****************************************************************************************
				             Copyrights   SaeruHikari
Description:					Chunk master class.
Details:		
*******************************************************************************************/
#pragma once
#include <stdlib.h>
#include "SArcheType.hpp"

namespace SECS
{
	struct SArcheType;
	class SArcheTypeManager;
	class SEntityManager;
	///<summary>
	/// Chunk is a block buffer that contains properties
	/// and components of archetype.
	///</summary>
	struct SChunk
	{
		friend class SArcheTypeManager;
		friend struct SArcheType;
		friend class SEntityManager;
	protected:
		// aligned structure of size 256
		struct ChunkProperties
		{
			alignas(8) SArcheType* ArcheType = nullptr;
			// Count of units in this chunk.
			alignas(8) int32_t Count = -1;
			// Index of this in the ChunkList.
			alignas(8) int32_t ListIndex = -1;

			alignas(8) size_t ChunkBuffSize = 16 * 1024 - 256;

			alignas(8) size_t FreeUnits = 0;
			// Pad
			alignas(1) char Flag[256 - 40] = {0};
		};
		// Size 16*1024
		void* buff;

		void* entityPtr = nullptr;
		void** comListPtrs = nullptr;

		void** wrkptrs = nullptr;
		void* entityWrkPtr = nullptr;
		ChunkProperties* properties;
	public:
		ChunkProperties* GetChunkProperty()
		{
			return properties;
		}
		~SChunk()
		{
			free(buff);
		}
	protected:
		SChunk()
		{
			buff = malloc(16 * 1024);
			unsigned char* ptr = (unsigned char*)buff;
			properties = new(ptr)ChunkProperties();
			// Rest: Chunk data that contains components and entities.
			// Cause we need to know exactly how the layout is
			// we need to have a template version call.
		}
		
		inline size_t GetProperUnitCount(size_t chunkSize, size_t arcSize)
		{
			return chunkSize / arcSize;
		}

		// Returns work ptr for type T.
		template<typename T>
		inline void* __getCompWrkPtr()
		{
			int index = properties->ArcheType->GetComponentIndex<T>();
			if (index >= 0)
			{
				return wrkptrs[index];
			}
			return nullptr;
		}

		// get entity ptr.
		template<typename T>
		inline T* __getEntityPtr(size_t n)
		{
			if (n > properties->Count - properties->FreeUnits)
			{
				return nullptr;
			}
			return (T*)entityPtr + n;
		}

		// get comp ptr.
		template<typename T>
		inline T* __getCompPtr(size_t n)
		{
			if (n > properties->Count - properties->FreeUnits)
			{
				return nullptr;
			}
			int index = properties->ArcheType->GetComponentIndex<T>();
			return ((T*)(comListPtrs[index])) + n;
		}

		// Returns false when the chunk is full.
		inline bool __offsetWrkptrs(size_t units)
		{
			properties->FreeUnits -= units;
			if (properties->FreeUnits <= 0) return false;
			for (size_t i = 0; i < properties->ArcheType->ComponentNum; i++)
			{
				// Do offset works.
				wrkptrs[i] = (unsigned char*)wrkptrs[i] + properties->ArcheType->SizeOfs[i] * units;
				entityWrkPtr = (unsigned char*)entityWrkPtr + properties->ArcheType->EntitySize * units;
			}
		}

		// Init Chunk Layout with given Component templates.
		inline void InitChunkLayout(SArcheType* arcType)
		{
			properties->ArcheType = arcType;
			// Calculate proper unit of the hole chunk
			properties->Count = GetProperUnitCount(properties->ChunkBuffSize, arcType->ComponentTotalSize + arcType->EntitySize);
			properties->FreeUnits = properties->Count;
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "SChunk: Generate " << properties->Count << " Units" << std::endl;
#endif
			wrkptrs = new void* [arcType->ComponentNum];
			comListPtrs = new void* [arcType->ComponentNum];
			entityPtr = (unsigned char*)buff + sizeof(*properties);
			entityWrkPtr = entityPtr;
			comListPtrs[0] = (unsigned char*)entityWrkPtr + properties->Count * arcType->EntitySize;
			unsigned char* startptr = (unsigned char*)comListPtrs[0];
			for (size_t i = 0; i < arcType->ComponentNum; i++)
			{
				// Cache pointer for every component.
				comListPtrs[i] = (unsigned char*)startptr + properties->Count * (arcType->ComponentOffsets[i]);
				wrkptrs[i] = comListPtrs[i];
#if defined(DEBUG) || defined(_DEBUG)
				std::cout << "SChunk: malloc," << properties->ArcheType->typeHashes[i] << " ptr offset: " << (unsigned char*)wrkptrs[i] - (unsigned char*)buff << std::endl;
#endif
			}
			// properties->ChunkBuffSize - arcType->ComponentTotalSize * properties->Count
			//memset(startptr + arcType->ComponentTotalSize * properties->Count, 0, properties->ChunkBuffSize - (arcType->ComponentTotalSize + arcType->EntitySize) * properties->Count);
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << std::endl;
#endif
			// Init finish.
		}

		template<typename T>
		inline T* __constructOnChunk_Internal()
		{
			T* Tptr = (T*)__getCompWrkPtr<T>();
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "Sakura Chunk: Construct Comp " << typeid(T).name() << " offset to buffer: " << (unsigned char*)Tptr - (unsigned char*)buff << std::endl;
#endif
			T* res = new(Tptr) T();
			return res;
		}

		template<typename T1, typename T2, typename... Ts>
		inline void __constructOnChunk_Internal()
		{
			__constructOnChunk_Internal<T1>();
			__constructOnChunk_Internal<T2, Ts...>();
		}

		// Returns the index in chunk.
		template<typename... Ts>
		inline int ConstructionOnChunk(SEntity&& entity)
		{
			*(SEntity*)entityWrkPtr = entity;
			__constructOnChunk_Internal<Ts...>();
			if (!__offsetWrkptrs(1)) // Full
			{
				properties->ArcheType->freeChunk = nullptr;
				
#if defined(DEBUG) || defined(_DEBUG)
				std::cout << "CHUNK FULL!" << std::endl;
#endif
			}
			return properties->Count - properties->FreeUnits - 1;
		}
	};


}

