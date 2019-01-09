#pragma once

#include <vector>
#include "CollideAlgorithm.h"
#include "Physics/Shape.h"

namespace Ares
{
	//------------------------------------
	// Åö×²¼ì²â 2012-11-19 µÛÁÖ
	//------------------------------------
	class Collide
	{
	public:
		Collide();
		~Collide();

		// ÉèÖÃÅö×²Ëã·¨
		static bool SetCollideAlgorithm( ShapeType type0, ShapeType type1, CollideAlgorithm* algorithm);

		// Ïà½»²âÊÔ
		static bool IntersectionTest( const Shape* shape1, const Shape* shape2);

		// Ïà½»²âÊÔ
		static bool IntersectionTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2);

		// ½øĞĞÅö×²¼ì²â
		static bool ContactTest( const Shape* shape1, const Quat& q1, const Vector3& t1, const Shape* shape2, const Quat& q2, const Vector3& t2, CollideContactList& oContactList, bool clear=true);

	private:
		static CollideAlgorithm*	m_dispatch[ST_Total][ST_Total];			// Åö×²¼ì²âº¯Êı·ÖÅÉ
	};
}