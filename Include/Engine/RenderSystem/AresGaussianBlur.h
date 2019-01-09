#pragma once

#include <vector>
#include "AresFrameFilter.h"

using namespace std;

namespace Ares
{
	//--------------------------------------
	// BlurTexOffseter 2013-2-26 帝林
	//--------------------------------------
	struct BlurTexOffseter
	{
		UINT				m_width;		// 光照图宽
		UINT				m_height;		// 光照图高
		int					m_kernelSize;
		int					m_kernelRadius;
		ShaderParamPtr		m_texOffset;

		static const float  WeightTable1x1[1];	// 1x1权重表
		static const float	WeightTable3x3[9];	// 3x3权重表
		static const float	WeightTable7x7[49];	// 7x7权重表

		// 构造函数
		BlurTexOffseter();

		// 设置参数
		void SetInput( UINT width, UINT height, const float* weightTable, int tableMembers);

		// 设置参数
		void SetInput( TexturePtr texture, const float* weightTable, int tableMembers);
	};

	//--------------------------------------
	// 模糊化 2013-2-25 帝林
	//--------------------------------------
	class GaussianBlur : public FrameFilter
	{
	public:
		static const float WeightTable1x49[49]; // 模糊权重表

	public:
		GaussianBlur( Scene& sceneSystem, bool hdrTexture=false);
		~GaussianBlur();

		// 执行模糊
		virtual void Execute();

		// 设置输入
		void SetInput( TexturePtr& oTexture, const TexturePtr& iTexture, const float* weightTable, int tableMembers);

	private:
		vector<float4>		m_texOffsetX;
		vector<float4>		m_texOffsetY;
		ShaderParamPtr		m_texOffset;
		ShaderParamPtr		m_blurTexture;			// 被模糊纹理

		TexturePtr			m_inputTexture;			// 输入纹理
		TexturePtr			m_middleTexture;		// 中间阶段纹理
		FrameBufferPtr		m_middleFrameBuffer;	
		FrameBufferPtr		m_finalFrameBuffer;
	};
	typedef s_ptr<GaussianBlur> GaussianBlurPtr;
}