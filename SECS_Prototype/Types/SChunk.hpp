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
	///<summary>
	/// Chunk is a block buffer that contains properties
	/// and components of archetype.
	///</summary>
	struct SChunk
	{
		friend class SArcheTypeManager;
		friend struct SArcheType;
		friend class SChunkList;
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
			alignas(1) char Pad[256 - 40];
		};
		void* buff;
		unsigned char* wrkptr;
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
			wrkptr = (unsigned char*)buff;
			properties = new(wrkptr)ChunkProperties();
			wrkptr += sizeof(ChunkProperties);
			// Rest: Chunk data that contains components and entities.
			// Cause we need to know exactly how the layout is
			// we need to have a template version call.
		}
		
		size_t GetProperUnitCount(size_t chunkSize, size_t arcSize)
		{
			return chunkSize / arcSize;
		}

		// Init Chunk Layout with given Component templates.
		void InitChunkLayout(SArcheType* arcType)
		{
			size_t size_E = arcType->ComponentTotalSize;
			size_t size_Comps = arcType->ComponentTotalSize;
			size_t size_Arc = size_E + size_Comps;
			// Calculate proper unit of the hole chunk
			properties->Count = GetProperUnitCount(properties->ChunkBuffSize, size_Arc);
			properties->FreeUnits = properties->Count;
			// Cache pointer pos for after usage.

			// Init finish.
		}

	};


}

