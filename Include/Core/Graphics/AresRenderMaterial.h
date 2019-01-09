#pragma once

#include "AresRenderMethod.h"

using namespace std;

namespace Ares
{
	//--------------------------------------
	// RenderMaterial  2012-4-11 帝林
	//     每一个渲染实例,都有一个唯一的材质,
	// 在较少内存浪费的前替下。方便shader
	// 参数的管理
	//--------------------------------------
	class CORE_EXPORT RenderMaterial
	{
		typedef vector<ShaderParamPtr> ParamPtrs;
		typedef vector<string>		   ParamAB;

	public:
		// 设置效果文件
		void SetMethod( RenderMethodPtr& method);

		// 是否为同源材质(have same shader)
		bool IsSameOrigin( const RenderMaterial& material) { return false; }

		// 获取参数数量
		size_t  GetParamNum() const { return m_paramPtrs.size(); }

		// 获取Effect
		RenderMethodPtr& GetMethod() { return m_method; }

		// 获取渲染队列
		BITFIELD GetRenderQueue() const { return m_renderQueue; }

	public:
		// 依据索引获取参数
		const ShaderParamPtr& GetShaderParam( size_t i) const;

		// 依据参数名获取参数
		const ShaderParamPtr& GetShaderParam( const string& paramName) const;

		// 依据索引设置参数
		void SetParamRef( size_t i, const ShaderParamPtr& param);

		// 依据参数名设置参数
		void SetParamRef( const string& paramName, const ShaderParamPtr& param);

		// 设置参数
		void SetActive();

	public:
		// 依据索引获取参数
		const string& GetParamAutoBind( size_t i) const;

		// 依据参数名获取参数
		const string& GetParamAutoBind( const string& paramName) const;

		// 依据索引设置参数
		void SetParamAutoBind( size_t i, const string& spabDesc);

		// 依据参数名设置参数
		void SetParamAutoBind( const string& paramName, const string& spabDesc);

		// 设置光栅化状态
		void SetRasterizerState( RasterizerStatePtr& state) { m_rasterState=state; }

		// 设置深度模板状态
		void SetDepthStencilState( DepthStencilStatePtr& state) { m_depthStencilState=state; }

		// 设置混合状态
		void SetBlendState( BlendStatePtr& state, const ColorRGB& factor, UINT mask);

		// 设置状态
		void SetState();

	public:
		// 设置渲染队列
		void SetRenderQueue( BITFIELD renderQueue) { m_renderQueue = renderQueue; }

	protected:
		RenderMaterial();
		~RenderMaterial();

	protected:
		BITFIELD					m_renderQueue;				// RenderQueue

		RenderMethodPtr				m_method;					// Render Method
		ParamPtrs					m_paramPtrs;				// shader param value
		ParamAB						m_spabDescs;				// shader param auto bind

		RasterizerStatePtr			m_rasterState;				// 光栅化状态
		DepthStencilStatePtr		m_depthStencilState;		// 深度模板状态
		WORD						m_frontStencilRef;			//
		BlendStatePtr				m_blendState;				// Alpha混合状态
		ColorRGB					m_blendFactor;				// 混合参数
		UINT						m_sampleMask;				// 混合参数
	};
}