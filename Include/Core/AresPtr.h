#pragma once

#include "AresPlatform.h"

namespace Ares
{
	// ÷∏ƒ‹÷∏’Î
	template<typename T> 
	INLINE boost::shared_ptr<T> MakeSharePtr( T* p)
	{
		return boost::shared_ptr<T>( p);
	}

	// COM∂‘œÛ∂®÷∆…æ≥˝∆˜
	template<typename T>
	INLINE void ReleaseCOMPtr( T* p)
	{
		SAFE_RELEASE( p);
	}

	// ªÒ»°COM∂‘œÛµƒ÷«ƒ‹÷∏’Î
	template<typename T>
	INLINE boost::shared_ptr<T> MakeCOMPtr(T* p)
	{
		return boost::shared_ptr<T>( p, ReleaseCOMPtr<T>);
	}
}	