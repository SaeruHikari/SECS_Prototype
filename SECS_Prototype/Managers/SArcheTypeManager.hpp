/*****************************************************************************************
				             Copyrights   SaeruHikari
Description:	          Arche Type Manager, Create and manage Arche types.
Details:    A specific combine of components on a single entity is called the ArcheType.
*******************************************************************************************/
#pragma once
#include "..\Types\SArcheType.hpp"
#include "..\Types\SChunk.hpp"

namespace SECS
{
	class SArcheTypeManager
	{
		friend class SEntityManager;
	protected:
		// members.
		static SArcheTypeManager* _this;
		

	protected:
		template<typename ... Cs>
		SArcheType* GetArcheType(SEntity* e)
		{
			SArcheType* _archetype = nullptr;
			// Finding...(unimplemented)

			// Not find(Always new):
			_archetype = CreateArcheType<Cs...>(e);
			return _archetype;
		}

		template<typename ... Cs>
		SArcheType* CreateArcheType(SEntity* e)
		{
			SArcheType* Arche = new SArcheType();
			Arche->Init<Cs...>(e);	
			return Arche;
		}

		// Not implemented.
		SChunk* GetFreeSChunk(SArcheType* _arc)
		{
			return CreateSChunk(_arc);
		}


		SChunk* CreateSChunk(SArcheType* arcType)
		{
			SChunk* _chunk = new SChunk();
			_chunk->InitChunkLayout(arcType);
			return _chunk;
		};


	protected:
		SArcheTypeManager()
		{
			assert(_this == nullptr);
		}
		static SArcheTypeManager* Get()
		{
			if (_this == nullptr)
			{
				_this = new SArcheTypeManager();
			}
			return _this;
		}
	};
	SArcheTypeManager* SArcheTypeManager::_this = nullptr;
}
