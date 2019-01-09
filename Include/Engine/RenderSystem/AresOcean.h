#pragma once

#include "Engine/RenderSystem/AresRenderEntity.h"

namespace Ares
{
	//--------------------------------------
	// Infinity Big Ocean 2013-8-21 帝林
	//--------------------------------------
	class Ocean : public RenderEntity
	{
	public:
		Ocean();
		~Ocean();

		// 设置屏幕空间格式大小
		void SetScreenSpaceGridSize( int gridNum);

		// 更新
		virtual void FrameMove( float fTime);

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	public:
		// 添加到场景消息
		virtual void OnAddToScene( Scene* scene);

	private:
		int						m_scrGridNum;			// 分隔数量
		ShaderParamPtr			m_lodFactor;			// Lod factor
		ShaderParamPtr			m_oceanParams0;			// WindDirection WindSpeed, waveSpeed, wavesAmount
		ShaderParamPtr			m_oceanParams1;			// WavesSize WindDirection.x WinDirection.y WaterHeight
		ShaderParamPtr			m_waterShallowColor;	// 水颜色
		ShaderParamPtr			m_waterDeepColor;		// 深水区颜色
		ShaderParamPtr			m_fracStrength;			// 水边缘,折射泡沫控制因子 
		ShaderParamPtr			m_texWaveBump;			// 波浪贴图
		ShaderParamPtr			m_texFoam;				// 泡沫贴图
		ShaderParamPtr			m_texReflect;			// 反射贴图
		Renderable				m_renderable;			// 渲染组件
	};
	typedef s_ptr<Ocean> OceanPtr;
}