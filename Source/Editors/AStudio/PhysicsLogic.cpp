#include "SCLogic.h"
#include <Physics/Intersect.h>
#include <Physics/IntrLine3Plane3.h>

namespace Ares
{
	// 构造函数
	PhysicsLogic::PhysicsLogic()
	{
		m_segment = NULL;
	}

	// 析构函数
	PhysicsLogic::~PhysicsLogic()
	{

	}

	// 获取屏幕捡选射线
	void PhysicsLogic::GetPickRay( Vector3& rayOrig, Vector3& rayDir)
	{
		HWND hwnd = SCLogic_Main->GetHwnd();

		POINT ptPoint;
		::GetCursorPos( &ptPoint);
		::ScreenToClient( hwnd, &ptPoint);

		RECT clientRect;
		::GetClientRect( hwnd, &clientRect);

		long clientWidth  = clientRect.right  - clientRect.left; 
		long clientHeight = clientRect.bottom - clientRect.top;

		WindowPointToRay( rayOrig, rayDir, ptPoint.x, ptPoint.y, clientWidth, clientHeight, SCLogic_Camera->GetView(), SCLogic_Camera->GetProj());

#if FALSE
		if( !m_segment)
			m_segment = AViewShapeMgr.CreateSegment();

		m_segment->Set( Segment3( Vector3( rayOrig), Vector3( rayOrig+rayDir*1000.f)));
#endif
	}

	// 鼠标检选
	bool PhysicsLogic::Pick( Vector3* intersectPos, float* pDist)
	{
		//if ( m_collMap)
		{
			Vector3 rayOrig;
			Vector3 rayDir;			
			GetPickRay( rayOrig, rayDir);
				
			Line3 tline( rayOrig, rayDir);
			//if( m_collMap->Pick(  tLine, tDist, intersePos))
			// 与场景有碰撞
			if( FALSE)
			{
				Vector3 intersePos;
				if( pDist)
					*pDist = 1000.f;

				if( intersectPos)
					*intersectPos = intersePos;

				return true;
			}
			// 默认相交点
			else
			{
				Plane3	tplane( Vector3::Zero, Vector3::ZAxis);
				IntrLine3Plane3 inLP( tline, tplane);
				if( inLP.Test())
				{
					if( pDist)
						*pDist = inLP.m_distance;
					if( intersectPos)
						*intersectPos = inLP.m_intrPoint;

					return true;
				}
			}

			return false;
		}
	}
}