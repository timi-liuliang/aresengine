#pragma once

#include <Engine/RenderSystem/RenderAPI/AresRenderEffect.h>
#include "D3D11Typedefs.h"
#include <Cg/cgD3D11.h>

namespace Ares
{
	//--------------------------------------
	// D3D11RenderEffect 2012-4-6 帝林
	// CGeffect Encapsulation
	//--------------------------------------
	class D3D11CgEffect : public RenderEffect
	{
	public:
		// 初始化
		static bool InitCg( ID3D11DevicePtr& d3d11Device);

		// 获取CGcontext
		static CGcontext GetContext() { return m_cgContext; } 

	public:
		// 构造函数
		D3D11CgEffect();

		// 析构函数
		~D3D11CgEffect();

		// 加载函数 *.cgfx
		virtual bool Load( const char* fileName);

		// 从内存加载
		virtual bool LoadFromMemory( void* code, size_t length);

	private:
		// 剖析Effect
		void ParseCgEffect();

		// 剖析参数
		void ParseCgEffectParameters();

		// 剖析Techniques
		void ParseCgEffectTechniques();

		// 剖析CgTechnique
		void ParseCgTechnique( CGtechnique cgTechnique, RenderTechniquePtr& renderTechnique);

		// 剖析CgPass
		void ParseCgPass( CGpass cgPass, RenderPassPtr& renderPass);

	private:
		static CGcontext	m_cgContext;			// 全局唯一

		CGeffect			m_cgEffect;				// CgEffect
	};
}