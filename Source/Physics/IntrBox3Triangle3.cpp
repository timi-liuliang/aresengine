#include <Physics/IntrUtility3.h>
#include <Physics/IntrBox3Triangle3.h>

namespace Ares
{
	// construction 
	IntrBox3Triangle3::IntrBox3Triangle3( const Box3& box, const Triangle3& triangle)
		: m_box( box),
		  m_triangle( triangle),
		  m_quantity( 0)
	{
	}

	// static test-intersection query
	bool IntrBox3Triangle3::Test()
	{
		float   fMin0, fMax0, fMin1, fMax1;
		Vector3 kD, akE[3];

		// Test direction of triangle normal
		kD    = m_triangle.GetNormal();
		fMin0 = Vector3Dot( kD, m_triangle.m_v[0]);
		fMax0 = fMin0;
		IntrAxis::GetProjection( kD, m_box, fMin1, fMax1);
		if( fMax1 < fMin0 || fMax0 < fMin1)
		{
			return false;
		}

		// Test direction of box faces
		for ( int i=0; i<3; i++)
		{
			kD = m_box.m_axis[i];
			IntrAxis::GetProjection( kD, m_triangle, fMin0, fMax0);
			float fDdC = Vector3Dot( kD, m_box.m_center);
			fMin1 = fDdC - m_box.m_extent[i];
			fMax1 = fDdC + m_box.m_extent[i];
			if( fMax1 < fMin0 || fMax0 < fMin1)
			{
				return false;
			}
		}

		// Test direction of triangle-box edge cross products
		akE[2] = akE[1] - akE[0];
		for ( int i0=0; i0<3; i0++)
		{
			for ( int i1=0; i1<3; i1++)
			{
				kD = Vector3Cross( akE[i0], m_box.m_axis[i1]);
				IntrAxis::GetProjection( kD, m_triangle, fMin0, fMax0);
				IntrAxis::GetProjection( kD, m_box, fMin1, fMax1);
				if( fMax1 < fMin0 || fMax0 < fMin1)
				{
					return false;
				}
			}
		}

		return true;
	}

	// static test-intersection query
	bool IntrBox3Triangle3::Find()
	{
		// start with the triangle and clip it against each face of the box
		m_quantity = 3;

		for ( int i=0; i<3; i++)
		{
			m_akPoint[i] = m_triangle.m_v[i];
		}

		for ( int iDir=-1; iDir<=1; iDir+=2)
		{
			for( int iSide=0; iSide<3; iSide++)
			{
				Vector3 kInnerNormal = ((float)iDir) * m_box.m_axis[iSide];
				float fConstant = Vector3Dot( kInnerNormal, m_box.m_center) - m_box.m_extent[iSide];

				ClipConvexPolygonAgainstPlane( kInnerNormal, fConstant, m_quantity, m_akPoint);
			}
		}

		return m_quantity > 0 ? true : false;
	}
}