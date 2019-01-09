#include "M1Export.h"

namespace M1_IGame
{
	extern SExportConfig	  g_config;				// 导出配置

	// Save an M1 model
	bool SExportM1::Save()
	{
		// 保存m1
		char modelNameExt[1024];
		char materialNameExt[1024];
		char animationNameExt[1024];

		NameExt( modelNameExt,    g_config.m_modLocation.c_str());
		NameExt( materialNameExt, g_config.m_matLocation.c_str());
		NameExt( animationNameExt,g_config.m_aniLocation.c_str());

		if( g_config.m_exportM1)
			m_m1.Save( g_config.m_m1Location.c_str());

		// 保存模型
		if( g_config.m_exportMod)
		{
			FILE* fileModel = fopen( g_config.m_modLocation.c_str(), "wb");
			if( fileModel)
			{
				m_model.Save( fileModel);

				fflush( fileModel);
				fclose( fileModel);
			}
		}

		// 保存材质
		if( g_config.m_exportMat)
		{
			bool bAnimation = m_animation.m_header.m_iNumBones ? true : false;

			m_material.Save( g_config.m_matLocation.c_str(), bAnimation);
		}
		

		// 保存动画数据
		if( g_config.m_exportAni)
		{
			if( m_animation.m_header.m_iNumBones)
			{
				m_animation.Save( g_config.m_aniLocation.c_str());
			}
		}

		// 保存碰撞数据
		if( g_config.m_exportTri)
		{
			m_collidedTri.Save( g_config.m_triLocation.c_str());
		}

		return false;
	}
}