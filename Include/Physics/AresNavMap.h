#pragma once

#include <vector>
#include <Core/AresMath.h>

using namespace std;

namespace Ares
{
	//--------------------------------------
	// 导航图 2011-03-08		帝林
	//--------------------------------------
	class INavMap
	{
		friend class CAStar;
	protected:
		// 获取可通行周围接点
		virtual void GetPassAbleNeighbor( int* childID, int& numChild,int fatherID)=0;

		// 根据ID计算两多边形间h
		virtual float CalcH( const Vector3& vStart, const Vector3& vEnd, int idFrom, int idTo=-1)=0;

		// 根据ID计算两多边形间g
		virtual float CalcG( const Vector3& vStart, int idFrom, int idTo, int idFromFrom=-1)=0;
	};
}