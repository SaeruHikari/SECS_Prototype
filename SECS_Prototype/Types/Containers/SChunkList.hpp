/*****************************************************************************************
							 Copyrights   SaeruHikari
Description:				 SChunkList master class.
Details:		Chunk list is a container of chunks of the same architecture.
*******************************************************************************************/
#pragma once
#include "../SChunk.hpp"

namespace SECS
{
	class SChunkList
	{
	public:
		SChunk** chunks = nullptr;
		SChunk** freeChunk = nullptr;

		void chunk_push(SChunk* _chunk)
		{

		}
	};
}
