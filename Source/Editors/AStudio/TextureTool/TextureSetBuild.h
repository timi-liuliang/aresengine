#pragma once

#include <Engine/RenderSystem/AresTextureSet.h>

namespace Ares
{
	//-----------------------------------------------
	// TexturePackageBuild 2012-7-23 帝林
	//-----------------------------------------------
	class TextureSetBuild
	{
	public:
		// 新建纹理包
		void SetTextureSize( UINT width, UINT height, ElementFormat format);

		// 插入纹理
		void Insert( TexturePtr& subTex);

		// 插入纹理
		void Insert( const char* fileName);

		// 获取结果纹理
		TexturePtr GetResultTexture() { return m_result; }

	private:
		TexturePtr	   m_result;
		TextureSet*	   m_texSet;
	};
}