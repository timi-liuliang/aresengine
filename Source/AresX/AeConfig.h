#pragma once

#include <string>
#include "max.h"

using namespace std;

namespace M1_IGame
{
	//----------------------------------------------
	// 导出配置 2010-08-31   帝林
	//----------------------------------------------
	struct SExportConfig
	{
		string  m_m1Location;	// 存储位置
		string	m_modLocation;	// 模型存储位置
		string  m_matLocation;	// 材质位置
		string  m_aniLocation;	// 动画位置
		string	m_triLocation;	// 三角形位置

		bool    binary;			// 是否二进制导出
		Point3  m_scale;
		bool    m_bDDS;			// 更换纹理到dds格式
		bool    m_bCopyTexture;	// 拷贝纹理到目的路径
		BOOL	m_keyFrame;		// 是否动画优先关键帧导出
		int		m_sampledensity;// 动画采样密度

		BOOL	m_exportM1;		// 导出m1文件
		BOOL	m_exportMod;	// 导出模型
		BOOL    m_exportMat;	// 导出材质
		BOOL	m_exportAni;	// 导出动画
		BOOL	m_exportTri;	// 碰撞三角形 

		string	m_colTriFilter;	// 碰撞mesh过滤

		// 构造函数
		SExportConfig()
		{
			binary = true;
			m_scale.x =  0.01f;
			m_scale.y =  0.01f;
			m_scale.z =  0.01f;
			m_bDDS    =  false;
			m_bCopyTexture = true;
			m_keyFrame	= true;
			m_exportTri = true;

			m_sampledensity = 3;
			m_colTriFilter  = "collision";
		}
	};
}