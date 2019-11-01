// SEntity is the Entity part of ECS. 
// Contains to size_t to describe index and version info.
#pragma once

namespace SECS
{
	struct SEntity
	{
		// Index of SEntity
	    size_t Index = -1;
		size_t Version = -1;
		virtual ~SEntity() {};
	};
}

