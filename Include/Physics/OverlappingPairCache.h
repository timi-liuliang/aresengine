#pragma once

#include "BroadphaseProxy.h"

namespace Ares
{
	//------------------------------------------------------------
	// OverlappingPairCache provides an interface for overlapping
	// pair management( add, remove, storage), use by the broadpha-
	// seInterface broadphases. 
	//------------------------------------------------------------
	class OverlappingPairCache
	{
	public:
		// ÊÇ·ñÐèÒªÅö×²
		bool IsNeedBroadphaseCollision( BroadphaseProxy* proxy0, BroadphaseProxy* proxy1);
	};
}