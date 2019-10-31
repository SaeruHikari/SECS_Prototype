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
	uint32_t TemplatePack_Num(T* t, TArgs* ... args)
	{
		int num = 0;
		TemplatePack_Num_Internal(num, t, args...);
		return num;
	}

private:
	template <typename T>
	void TemplatePack_Num_Internal(int& num, T* t)
	{
		num += 1;
	}

	template <typename T, typename... TArgs>
	uint32_t TemplatePack_Num_Internal(int& num, T* t, TArgs* ... args)
	{
		num += 1;
		TemplatePack_Num_Internal(num, args...);
		return num;
	}
};