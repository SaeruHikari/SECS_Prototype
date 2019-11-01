/*****************************************************************************************
				             Copyrights   SaeruHikari
Description:				Hika util of template packs.
Details:		
*******************************************************************************************/
#pragma once
#include <stdint.h>

class TemplatePackUtils
{
public:
	template <typename T, typename... TArgs>
	static uint32_t TemplatePack_Num(T* t, TArgs* ... args)
	{
		int num = 0;
		TemplatePack_Num_Internal(num, t, args...);
		return num;
	}

	template <typename ... Ts>
	static uint32_t TemplatePack_SizeOf()
	{
		uint32_t num = 0;
		TemplatePack_SizeOf_Internal<Ts...>(num);
		return num;
	}
private:
	template <typename T>
	static void TemplatePack_SizeOf_Internal(uint32_t& num)
	{
		num += sizeof(T);
	}

	template <typename T, typename T2, typename ... Ts>
	static void TemplatePack_SizeOf_Internal(uint32_t& num)
	{
		TemplatePack_SizeOf_Internal<T>(num);
		TemplatePack_SizeOf_Internal<T2, Ts...>(num);
	}

	template <typename T>
	static void TemplatePack_Num_Internal(int& num, T* t)
	{
		num += 1;
	}

	template <typename T, typename... TArgs>
	static uint32_t TemplatePack_Num_Internal(int& num, T* t, TArgs* ... args)
	{
		num += 1;
		TemplatePack_Num_Internal(num, args...);
		return num;
	}
};