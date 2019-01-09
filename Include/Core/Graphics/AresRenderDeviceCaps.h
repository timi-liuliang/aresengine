#pragma once

#include "Core/AresPlatform.h"

namespace Ares
{
	//-----------------------------------
	// TessellationMethod 2012-3-20 帝林
	//-----------------------------------
	enum TessellationMethod
	{
		TM_Hardware,
		TM_Instanced,
		TM_No
	};

	//-----------------------------------
	// 设备能力 2011-12-28 帝林
	//				摘自 KlayGe 龚敏敏
	//-----------------------------------
	struct RenderDeviceCaps
	{
		UINT8		m_maxShaderModel;

		UINT		m_maxTextureWidth;				// 纹理最大宽
		UINT		m_maxTextureHeight;				// 纹理最大高
		UINT		m_maxTextureDepth;
		UINT		m_maxTextureCubeSize;
		UINT		m_maxTextureArrayLength;
		UINT8		m_maxVertexTextureUnits;
		UINT8		m_maxPixelTextureUnits;
		UINT8		m_maxGeometryTextureUnits;

		UINT8		m_maxSimulataneousRts;			// 最大同时渲染目标数量

		UINT		m_maxVertices;					// 最大顶点数
		UINT		m_maxIndices;					// 最大索引数
		UINT		m_maxVertexStreams;				// 最大顶点流

		UINT8		m_maxTextureAnisotropy;	

		bool		m_hwInstancingSupport;			// 是否支持硬件实例化
		bool		m_instanceIdSupport;			
		bool		m_streamOutputSupport;
		bool		m_alphaToCoverageSupport;
		bool		m_primitiveRestartSupport;
		bool		m_multithreadRenderingSupport;
		bool		m_multithreadResCreatingSupport;
		bool		m_mrtIndependentBitDepthsSupport;

		bool		m_gsSupport;
		bool		m_csSupport;
		bool		m_hsSupport;
		bool		m_dsSupport;

		TessellationMethod	m_tessllationMethod;

		boost::function<bool(ElementFormat)> IsVertexFormatSupport;
		boost::function<bool(ElementFormat)> IsTextureFormatSupport;
		boost::function<bool(ElementFormat, UINT32, UINT32)> IsRenderTargetFormatSupport;
	};
}