/*****************************************************************************************
				             Copyrights   SaeruHikari
Description:					Chunk master class.
Details:		
*******************************************************************************************/
#pragma once
#include "SArcheType.hpp"

namespace SECS
{
	struct SArcheType;
	class SArcheTypeManager;
	class SChunkList;
	class SEntityManager;
	///<summary>
	/// Chunk is a block buffer that contains properties
	/// and components of archetype.
	///</summary>
	struct SChunk
	{
		friend class SArcheTypeManager;
		friend struct SArcheType;
		friend class SChunkList;
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
		void* buff;
		void* entityPtr = nullptr;
		void** comListPtrs = nullptr;
		void** wrkptrs = nullptr;
		ChunkProperties* properties;
	public:
		ChunkProperties* GetChunkProperty()
		{
			return properties;
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
		
		size_t GetProperUnitCount(size_t chunkSize, size_t arcSize)
		{
			return chunkSize / arcSize;
		}

		// Check if there is data on given index.
		bool __haveEntity(size_t n)
		{
			return properties->Flag[n];
		}

		// Returns work ptr for type T.
		template<typename T>
		void* __getPtr()
		{
			int index = properties->ArcheType->GetComponentIndex<T>();
			if (index >= 0)
			{
				return wrkptrs[index];
			}
			return nullptr;
		}

		// Unsafe get.
		// Not implemented now (? Not necessary)
		template<typename T>
		void* __unsafeGetPtr(size_t n)
		{
			if (n < properties->FreeUnits)
			{
				return nullptr;
			}
			else return nullptr;
		}

		// Returns false when the chunk is full.
		bool __offsetWrkptrs(size_t units)
		{
			properties->FreeUnits -= units;
			if (properties->FreeUnits <= 0) return false;
			for (size_t i = 0; i < properties->ArcheType->ComponentNum; i++)
			{
				// Do offset works.
				wrkptrs[i] = (unsigned char*)wrkptrs[i] + properties->ArcheType->ComponentOffsets[i] * units;
			}
		}

		// Init Chunk Layout with given Component templates.
		void InitChunkLayout(SArcheType* arcType)
		{
			properties->ArcheType = arcType;
			size_t size_E = arcType->EntitySize;
			size_t size_Comps = arcType->ComponentTotalSize;
			size_t size_Arc = size_E + size_Comps;
			// Calculate proper unit of the hole chunk
			properties->Count = GetProperUnitCount(properties->ChunkBuffSize, size_Arc);
			properties->FreeUnits = properties->Count;
			wrkptrs = new void* [arcType->ComponentNum];
			comListPtrs = new void* [arcType->ComponentNum];
			entityPtr = (unsigned char*)buff + sizeof(*properties);
			int test = sizeof(*properties);
			comListPtrs[0] = (unsigned char*)entityPtr + properties->Count * arcType->EntitySize;
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
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << std::endl;
#endif
			// Init finish.
		}

		template<typename T>
		T* ConstructOnChunk()
		{
			T* Tptr = (T*)__getPtr<T>();
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "Sakura Chunk: Construct Comp " << typeid(T).name() << " offset to buffer: " << (unsigned char*)Tptr - (unsigned char*)buff << std::endl;
#endif
			return new(Tptr) T();
		}

	};


}

