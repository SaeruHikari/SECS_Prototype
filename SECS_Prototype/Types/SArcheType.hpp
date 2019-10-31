#pragma once
#include "SEntity.hpp"
#include "SComponent.hpp"
#include "SChunk.hpp"
#include <cassert>
#include <iostream>

// ArcheType.
struct SArcheType
{
	int ComponentNum = 0;
	int ComponentTotalSize = 0;
	size_t* typeHashes;
	size_t* ComponentOffsets;
	size_t* SizeOfs;
	SChunk* chunks = nullptr;
	SChunk* freeChunk = nullptr;

public:
	template<typename __C>
	void __init__Internal()
	{
		typeHashes[ComponentNum] = typeid(__C).hash_code();
		SizeOfs[ComponentNum] = sizeof(__C);
		ComponentOffsets[ComponentNum] = ComponentTotalSize;
		ComponentNum += 1;
		ComponentTotalSize += sizeof(__C);
#if defined(DEBUG) || defined(_DEBUG)
		std::cout << "Current " << ComponentNum << " Objects in SArcheType ";
		std::cout << typeid(__C).name() << ": \nSize: " << SizeOfs[ComponentNum-1] << ", Offset: " << ComponentOffsets[ComponentNum-1];
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
	void Init()
	{
#if defined(DEBUG) || defined(_DEBUG)
		std::cout << typeid(*this).name() << std::endl;
#endif
		typeHashes = new size_t[sizeof...(Components)];
		ComponentOffsets = new size_t[sizeof...(Components)];
		SizeOfs = new size_t[sizeof...(Components)];
		assert(chunks == nullptr);
		chunks = new SChunk();
		freeChunk = chunks;
		__init__Internal<Components...>();
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