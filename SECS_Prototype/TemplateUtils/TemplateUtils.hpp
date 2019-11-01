/*****************************************************************************************
				             Copyrights   SaeruHikari
Description:					 Template Utils.
Details:		
*******************************************************************************************/
#pragma once
#include <typeindex>

namespace TemplateUtils
{
	template<typename T>
	void getHash(size_t _pos, size_t* _targ)
	{
		size_t _hash = typeid(T).hash_code();
		// do insert, hash small ~ big
		for (int i = _pos - 1; i > 0; i--)
		{
			if (_hash < _targ[i])
			{
				_targ[i + 1] = _targ[i];
			}
			else 
			{ 
				_targ[i + 1] = _hash;
				break;
			}
		}
	}
	template<typename T, typename T2, typename ... __Ts>
	void getHash(size_t _len, size_t* _targ)
	{
		getHash<T>((size_t)(_len - (1 + sizeof...(__Ts))), _targ);
		getHash<T2, __Ts...>(_len, _targ);
	}

	template<typename ... __Args>
	struct ArcheTemplate
	{
		template<typename ... __InArgs>
		bool match()
		{
			if (sizeof...(__Args) != sizeof...(__InArgs)) return false;
			else
			{
				size_t __selfHash[sizeof...(__Args) + 1];
				size_t __inHash[sizeof...(__InArgs) + 1];
				getHash<__InArgs...>(sizeof...(__InArgs), __inHash);
				getHash<__Args...>(sizeof...(__InArgs), __selfHash);
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
};