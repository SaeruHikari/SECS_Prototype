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
	inline static uint32_t TemplatePack_Num(T* t, TArgs* ... args)
	{
		int num = 0;
		TemplatePack_Num_Internal(num, t, args...);
		return num;
	}

	template <typename ... Ts>
	inline static uint32_t TemplatePack_SizeOf()
	{
		uint32_t num = 0;
		TemplatePack_SizeOf_Internal<Ts...>(num);
		return num;
	}

	template <typename ... Ts>
	inline static size_t* GetHashes()
	{
		size_t* res = new size_t[sizeof...(Ts)]();
		getHash_Internal<Ts...>(sizeof...(Ts), res);
		return res;
	}

	template<typename ... __Args>
	inline static bool match(size_t* _inhashes, size_t size)
	{
		if (size != sizeof...(__Args)) 
			return false;
		else
		{
			size_t* __selfHash = GetHashes<__Args...>();
			for (size_t i = 0; i < sizeof...(__Args); i++)
			{
				if (_inhashes[i] != __selfHash[i])
				{
					return false;
				}
			}
		}
		return true;
	}
public:
	template<typename ... __Args>
	struct ArcheTemplate
	{
		template<typename ... __InArgs>
		bool match()
		{
			if (sizeof...(__Args) != sizeof...(__InArgs)) return false;
			else
			{
				size_t* __inHash = GetHashes<__InArgs...>();
				size_t* __selfHash = GetHashes<__Args...>();
				for (size_t i = 0; i < sizeof...(__InArgs); i++)
				{
					if (__inHash[i] != __selfHash[i])
					{
						return false;
					}
				}
			}
			return true;
		}
	};

private:
	template<typename T>
	inline static void getHash_Internal(size_t _pos, size_t* _targ)
	{
		size_t _hash = typeid(T).hash_code();
		if (_pos == 0)
		{
			_targ[_pos] = _hash;
			return;
		}
		// do insert, hash small ~ big
		for (int i = _pos - 1; i >= 0; i--)
		{
			if (_hash < _targ[i])
			{
				_targ[i + 1] = _targ[i];
				if (i == 0)
					_targ[i] = _hash;
			}
			else
			{
				_targ[i + 1] = _hash;
				break;
			}
		}
	}
	template<typename T, typename T2, typename ... __Ts>
	inline static void getHash_Internal(size_t _len, size_t* _targ)
	{
		getHash_Internal<T>((size_t)(_len - sizeof...(__Ts)) - 2, _targ);
		getHash_Internal<T2, __Ts...>((sizeof...(__Ts) == 0) ? _len - 1 : _len, _targ);
	}

	template <typename T>
	inline static void TemplatePack_SizeOf_Internal(uint32_t& num)
	{
		num += sizeof(T);
	}

	template <typename T, typename T2, typename ... Ts>
	inline static void TemplatePack_SizeOf_Internal(uint32_t& num)
	{
		TemplatePack_SizeOf_Internal<T>(num);
		TemplatePack_SizeOf_Internal<T2, Ts...>(num);
	}

	template <typename T>
	inline static void TemplatePack_Num_Internal(int& num, T* t)
	{
		num += 1;
	}

	template <typename T, typename... TArgs>
	inline static uint32_t TemplatePack_Num_Internal(int& num, T* t, TArgs* ... args)
	{
		num += 1;
		TemplatePack_Num_Internal(num, args...);
		return num;
	}
};