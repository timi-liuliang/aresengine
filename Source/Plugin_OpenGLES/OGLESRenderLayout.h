#pragma once

#include <Core/Graphics/AresRenderLayout.h>
#include "OGLESRenderMethod.h"

namespace Ares
{
	//----------------------------------------
	// OGLESRenderLayout 2013-02-05 帝林
	//----------------------------------------
	class OGLESRenderLayout : public RenderLayout
	{
	public:
		// 设置激活
		void Render( OGLESRenderLayout* preLayout, OGLESRenderMethod* method);

		// 激活
		void Active( const OGLESRenderMethod& method) const;

		// 吊销
		void Deactive( const OGLESRenderMethod& method) const;
	};
}