#include <Physics/OverlappingPairCache.h>

namespace Ares
{
	// ÊÇ·ñÐèÒªÅö×²
	bool OverlappingPairCache::IsNeedBroadphaseCollision( BroadphaseProxy* proxy0, BroadphaseProxy* proxy1)
	{
		bool collides0 = proxy0->m_collisionFilterMask & proxy1->m_collisionFilterGroup;
		bool collides1 = proxy1->m_collisionFilterMask & proxy0->m_collisionFilterGroup;

		return collides0 && collides1;
	}
}