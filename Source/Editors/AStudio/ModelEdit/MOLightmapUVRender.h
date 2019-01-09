#pragma once

#include <Engine/RenderSystem/AresRenderable.h>

namespace Ares
{
	//------------------------------------
	// RenderLightmapUV 2012-7-10 帝林
	//------------------------------------
	class LightmapUVRender : public Renderable
	{
	public:
		// 构造函数
		LightmapUVRender();

		// 析构函数
		~LightmapUVRender();

		// 设置显示数据
		void Set( RenderLayout::TopologyType topologyType, const void* positions, int vertexSizeInByte, const void* indices, int indicesSizeInByte, ElementFormat indiceFormat=EF_R16UI);
	};
}