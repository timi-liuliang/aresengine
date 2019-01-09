#include "Physics/WaterZoneMgr.h"

#define  COMP FLT_MAX / 2.0f;

namespace Ares
{
	// 构造函数
	WaterZoneMgr::WaterZoneMgr()
	 : Shape( ST_UnKnown) 
	, m_nXGrids(0)
	 , m_nZGrids(0)
	,	m_nGridSize(1)
	,	m_pWaterInfoFlag(NULL)
	{
	}

	WaterZoneMgr::~WaterZoneMgr(void)
	{
		SAFE_DELETE_ARRAY( m_pWaterInfoFlag);
	}

	bool WaterZoneMgr::PtInWater(int nX, int nZ, float& fDeep) const
	{
		if (!m_pWaterInfoFlag)
			return false;
		int X = nX/m_nGridSize;
		int Z = nZ/m_nGridSize;
		if (X < 0 || X > m_nXGrids)
		{
			return false;
		}
		if (Z < 0 || Z > m_nZGrids)
		{
			return false;
		}
		int nIndex = m_nXGrids * Z + X;
		if (m_pWaterInfoFlag[nIndex].m_fFlag < FLT_MAX)
		{
			fDeep = m_pWaterInfoFlag[nIndex].m_fFlag;
			return true;
		}
		else
		{
			return false;
		}
	}

	// 判断是否在水中
	bool WaterZoneMgr::IsPointIn( const Vector3& point) const
	{
		float depth = -FLT_BIG;
		if( PtInWater( (int)point.x, (int)point.y, depth))
		{
			if( point.z < depth-1.f)
				return true;
		}

		return false;
	}

	//------------------------------------------------------------------------------
	// 加载水标识
	//------------------------------------------------------------------------------
	bool WaterZoneMgr::LoadWaterFlags(const char* pszFileName)
	{
		SAFE_DELETE_ARRAY(m_pWaterInfoFlag);
		m_nXGrids = 0;
		m_nZGrids = 0;

		if (!pszFileName)
			return false;

		FILE* fileHandle = fopen( pszFileName, "rb");
		if ( !fileHandle)
			return false;

		int nFLen;
		fseek(fileHandle, 0L, SEEK_END);
		nFLen = ftell(fileHandle);
		fseek(fileHandle, 0L, SEEK_SET);
		--nFLen;
		if (nFLen < sizeof(int) * 2)
		{
			fclose(fileHandle);
			return false;
		}


		//
		fread(&m_nGridSize, 1, sizeof(int), fileHandle);
		if (m_nGridSize>8)
		{
			return false;
		}
		char* pBuffer = new char[nFLen];
		fread(pBuffer, 1, nFLen, fileHandle);
		int nX = *(int*)pBuffer;
		int nZ = *(int*)(pBuffer + sizeof(int));

		if (nX <= 0 || nZ <= 0)
		{
			fclose(fileHandle);
			SAFE_DELETE_ARRAY(pBuffer);
			return false;
		}
		m_nXGrids = nX;
		m_nZGrids = nZ;

		m_pWaterInfoFlag = new SWaterInfoFlag[(m_nXGrids + 1) * (m_nZGrids + 1)];
		memcpy(m_pWaterInfoFlag, pBuffer + sizeof(int) * 2, sizeof(SWaterInfoFlag) * (m_nXGrids + 1) * (m_nZGrids + 1));
		//CopyArray(m_pWaterInfoFlag,pBuffer + sizeof(int) * 2, sizeof(SWaterInfoFlag) * (m_nXGrids + 1) * (m_nZGrids + 1));
		SAFE_DELETE_ARRAY(pBuffer);
		fclose(fileHandle);
		return true;
	}

	// 构建AABB包围盒
	void WaterZoneMgr::BuildAABB( Rect3& box, const Transform& trans) const
	{
		box.m_min = Vector3( 0.f, 0.f, -2000.f);
		box.m_max = Vector3( (float)(m_nXGrids*m_nGridSize), (float)(m_nZGrids*m_nGridSize), 2000.f);
	}
}