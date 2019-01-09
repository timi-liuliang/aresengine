#ifndef ARES_FXAA_H
#define ARES_FXAA_H

#include "AresFrameFilter.h"

namespace Ares
{
	//------------------------------------
	// FXAA 2013-7-13 帝林
	//------------------------------------
	class FXAA : public FrameFilter
	{
	public:
		// 构造函数
		FXAA( Scene& scene);

		// 设置参数
		void Set( FXAALevel level);

		// 设置源纹理
		bool SetSrcTexture( TexturePtr& src);

	private:
		ShaderParamPtr			m_srcTexture;
	};
	typedef s_ptr<FXAA> FXAAPtr;
}

#endif