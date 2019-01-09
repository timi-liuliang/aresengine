#pragma once

#include <Core/Graphics/AresRenderLayout.h>
#include "OGLRenderMethod.h"

namespace Ares
{
	//----------------------------------------
	// OGLESRenderLayout 2013-02-05 帝林
	//----------------------------------------
	class OGLRenderLayout : public RenderLayout
	{
	public:
		// 设置激活
		void Render( OGLRenderLayout* preLayout, OGLRenderMethod* method);

		// 激活
		void Active( const OGLRenderMethod& method) const;

		// 吊销
		void Deactive( const OGLRenderMethod& method) const;
	};
}