#pragma once

// 注：此头文件可不对外发布

#include "AresRecast.h"
#include "AresStaticNavMesh.h"

namespace Ares
{
	//------------------------------------------------------
	// 操作管理器(可处理的数据大小有限32*32) 2011-02-22 帝林
	//	   生成器模式
	//------------------------------------------------------
	class AI_API CStaticMeshBuilder
	{
	public:
		// 构造函数
		CStaticMeshBuilder();

		// 析构函数

		// 构建
		bool HandleBuild( const SBuildParam& params);

	private:
		// 重置
		void  CleanUp();

	public:
		bool                    m_bKeepInterResults;	// 是否保存内部数据
		SRcConfig				m_rcConfig;				// 配置
		unsigned char*			m_pTriFlags;			// 标记
		SRcHeightField*			m_pSolid;				//
		SRcCompactHeightField*	m_pCHF;					//
		SRcContourSet*          m_pCSet;
		RcPolyMesh*				m_pMesh;				// 多边形mesh
		SRcPolyMeshDetail*		m_pDMesh;				// 详细多边形mesh
	};
}