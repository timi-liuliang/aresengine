#pragma once

#include "AresLight.h"

namespace Ares
{
	//---------------------------------------------------
	// 点光源  ILightPoint 2010-08-25  帝林
	//---------------------------------------------------
	class PointLight : public Light
	{
	public:
		// 构造函数
		PointLight();

		// 获取衰减公式( a, b*d, c*d*d)系数a, b ,c，其中 d为光源位置到顶点的距离
		const Vector3& GetAttenuate() { return m_attenuate; }

		// 设置衰减度
		void SetAttenuate( float a, float b, float c);

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr) {}

	private:
		Vector3		m_attenuate;
	};
}