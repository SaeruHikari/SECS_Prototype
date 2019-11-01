#pragma once
#include "SEntity.hpp"
#include "SComponent.hpp"
#include <cassert>
#include <iostream>

namespace SECS
{
	struct SChunk;
	class SChunkList;
	// ArcheType.
	struct SArcheType
	{
		friend class SArcheTypeManager;
		friend struct SChunk;
	protected:
		int ComponentNum = 0;
		size_t EntitySize = 0;
		size_t ComponentTotalSize = 0;
		// Insert sorted at init time.
		size_t* typeHashes;
		// Offsets in the chunk.
		size_t* ComponentOffsets;
		size_t* SizeOfs;
		SChunkList* chunks = nullptr;
		SChunk* freeChunk = nullptr;
	public:
		// Returns -1 when not such component found.
		template<typename T>
		int GetComponentIndex()
		{
			size_t _hash = typeid(T).hash_code();
			size_t _hashB = typeid(ComponentB).hash_code();
			for (size_t i = 0; i < ComponentNum; i++)
			{
#if defined(DEBUG_ARCHETYPE)
				std::cout << std::endl << "ArcheType: Get Comp Index Output component hash " << typeHashes[i] << std::endl;
#endif
				if (_hash == typeHashes[i])
					return i;
			}
			return -1;
		}

		// hash code version of GetComponentIndex<T>()
		int GetComponentIndex(size_t _hash)
		{
			for (size_t i = 0; i < ComponentNum; i++)
			{
				if (_hash == typeHashes[i]) return i;
			}
			return -1;
		}
	protected:
		template<typename __C>
		void __init__Internal()
		{
			size_t _hash = typeid(__C).hash_code();
			size_t sizeOf = sizeof(__C);
			size_t componentOffset = ComponentTotalSize;
			int i = 0;
			if (ComponentNum == 0)
			{
				typeHashes[0] = _hash;
				SizeOfs[0] = sizeOf;
				ComponentOffsets[0] = componentOffset;
			}
			else i = ComponentNum - 1;
			for (i; i >= 0; i--)
			{
				if (_hash < typeHashes[i])
				{
					typeHashes[i + 1] = typeHashes[i];
					SizeOfs[i+1] = SizeOfs[i];
					ComponentOffsets[i+1] = ComponentOffsets[i];
					if (i == 0)
					{
						typeHashes[i] = _hash;
						SizeOfs[i] = sizeOf;
						ComponentOffsets[i] = componentOffset;
						i = 0;
						break;
					}
				}
				else
				{
					typeHashes[i + 1] = _hash;
					SizeOfs[i + 1] = sizeOf;
					ComponentOffsets[i + 1] = componentOffset;
					i = i + 1;
					break;
				}
			}
			ComponentNum += 1;
			ComponentTotalSize += sizeof(__C);
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "Current " << ComponentNum << " Objects in SArcheType " << "   " << typeid(__C).hash_code();
			std::cout << typeid(__C).name() << ": \nSize: " << SizeOfs[i] << ", Offset: " << ComponentOffsets[i];
			std::cout << ", Total Size: " << ComponentTotalSize << std::endl;
#endif
		}
		template<typename __C1, typename __C2, typename ... __Cs>
		void __init__Internal()
		{
			__init__Internal<__C1>();
			__init__Internal<__C2, __Cs...>();
		}

		template<typename ... Components>
		void Init(SEntity* e)
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << std::endl << "Start construct: " << typeid(*this).name() << std::endl;
#endif
			typeHashes = new size_t[sizeof...(Components)]();
			ComponentOffsets = new size_t[sizeof...(Components)]();
			SizeOfs = new size_t[sizeof...(Components)]();
			EntitySize = sizeof(e);
			__init__Internal<Components...>();
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "End construct! " << typeid(*this).name() << std::endl << std::endl;
#endif
		}

		SArcheType()
		{

		}

		template<typename __C>
		__C* __findInChunk(SChunk* __chunk)
		{
			return nullptr;
		}
		template<typename __C, typename ... Params>
		void __createComponentAtWithParams(Params&& ... __params)
		{
			__C* __ptr = __findInChunk<__C>();
			new(__ptr) __C(__params...);
		}
	public:
		friend std::ostream& operator <<(std::ostream& out, const SArcheType& ar)
		{
			out << "ArcheType Component Num: " << ar.ComponentNum << std::endl;
			out << "ArcheType Component Total Size: " << ar.ComponentTotalSize << std::endl;
			for (int i = 0; i < ar.ComponentNum; i++)
				out << "ArcheType Component typeHash: " << *(ar.typeHashes + i) << "  " << std::endl;
			for (int i = 0; i < ar.ComponentNum; i++)
				out << "ArcheType Component ComponentOffsets: " << *(ar.ComponentOffsets + i) << std::endl;
			return out;
		}
	};
}
	