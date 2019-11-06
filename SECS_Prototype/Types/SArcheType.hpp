#pragma once
#include "SEntity.hpp"
#include "SComponent.hpp"
#include <cassert>
#include <iostream>
#include "Containers\SChunkList.hpp"


namespace SECS
{
	struct SChunk;
	// ArcheType.
	struct SArcheType
	{
		friend class SEntityManager;
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
		template<typename... Ts>
		inline SArcheType Expand()
		{
			size_t* _hashes = TemplatePackUtils::GetHashes<Ts...>();
			size_t* _sizes = TemplatePackUtils::TemplatePack_TSizes<Ts...>();
			return std::move(Expand(_hashes, sizeof...(Ts), _sizes));
		}
		inline SArcheType Expand(size_t* _InHash, int _HashLength, size_t* _EntitySizes)
		{
			assert(_HashLength > 0);
			SArcheType _newArche = *this;
			_newArche.chunks = nullptr;

			_newArche.ComponentTotalSize += EntitySize;
			_newArche.freeChunk = nullptr;
			// insert 
			_HashLength--;
			int i;
			for (_HashLength; _HashLength >= 0; _HashLength--)
			{
				for (i = ComponentNum - 1; i >= 0 && _newArche.typeHashes[i] > _InHash[_HashLength]; i--)
				{
					_newArche.typeHashes[i + 1] = _newArche.typeHashes[i];
					_newArche.SizeOfs[i + 1] = _newArche.SizeOfs[i];
				}
				_newArche.typeHashes[i + 1] = _InHash[_HashLength];
				_newArche.SizeOfs[i + 1] = _EntitySizes[_HashLength];
				
				// Insert finish, comp num++
				_newArche.ComponentNum += 1;
			}
			// Recompute offsets
			_newArche.ComponentOffsets[0] = 0;
			for (i = 1; i < ComponentNum; i++)
			{
				_newArche.ComponentOffsets[i] = _newArche.ComponentOffsets[i - 1] + _newArche.SizeOfs[i - 1];
			}
			// return moved
			return std::move(_newArche);
		}
	public:
		inline bool Is(size_t num, size_t* _typeHashes)
		{
			if (num != this->ComponentNum) return false;
			while (num > 0)
			{
				num--;
				if (typeHashes[num] != _typeHashes[num]) return false;	
			}
			return true;
		}
		inline bool Is(SArcheType* _arc)
		{
			return Is(_arc->ComponentNum, _arc->typeHashes);
		}
	public:
		// Returns -1 when not such component found.
		template<typename T>
		inline int GetComponentIndex()
		{
			size_t _hash = typeid(T).hash_code();
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
		inline int GetComponentIndex(size_t _hash)
		{
			for (size_t i = 0; i < ComponentNum; i++)
			{
				if (_hash == typeHashes[i]) return i;
			}
			return -1;
		}
	protected:
		template<typename __C>
		inline void __init__Internal()
		{
			size_t _hash = typeid(__C).hash_code();
			int i = ComponentNum - 1;
			for (i; i >= 0 & _hash < typeHashes[i]; i--)
			{
				typeHashes[i + 1] = typeHashes[i];
				SizeOfs[i+1] = SizeOfs[i];
				ComponentOffsets[i+1] = ComponentOffsets[i];
			}
			typeHashes[i + 1] = _hash;
			SizeOfs[i+1] = sizeof(__C);
			ComponentOffsets[i+1] = ComponentTotalSize;

			ComponentNum += 1;
			ComponentTotalSize += sizeof(__C);
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "Current " << ComponentNum << " Objects in SArcheType " << "   " << typeid(__C).hash_code();
			std::cout << typeid(__C).name() << ": \nSize: " << SizeOfs[i + 1] << ", Offset: " << ComponentOffsets[i + 1];
			std::cout << ", Total Size: " << ComponentTotalSize << std::endl;
#endif
		}
		template<typename __C1, typename __C2, typename ... __Cs>
		inline void __init__Internal()
		{
			__init__Internal<__C1>();
			__init__Internal<__C2, __Cs...>();
		}

		template<typename ... Components>
		inline void Init(SEntity* e)
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << std::endl << "Start construct: " << typeid(*this).name() << std::endl;
#endif
			typeHashes = new size_t[sizeof...(Components)]();
			ComponentOffsets = new size_t[sizeof...(Components)]();
			SizeOfs = new size_t[sizeof...(Components)]();
			EntitySize = sizeof(*e);
			__init__Internal<Components...>();
#if defined(DEBUG) || defined(_DEBUG) 
			std::cout << "End construct! " << typeid(*this).name() << std::endl << std::endl;
#endif
		}
		SArcheType()
		{

		}
		// Not implemented.
		template<typename __C>
		inline __C* __findInChunk(SChunk* __chunk)
		{
			return nullptr;
		}
		// Not implemented.
		template<typename __C, typename ... Params>
		inline void __createComponentAtWithParams(Params&& ... __params)
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
	