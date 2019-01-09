#pragma once

#include "../../Foundation/AresComponent.h"

namespace Ares
{
	//------------------------------------
	// 渲染组件 2012-3-30 帝林
	//------------------------------------
	class ARES_EXPORT ComponentRender : public Component
	{
	public:
		// 构造函数
		ComponentRender();

		// 析构函数
		~ComponentRender();

	protected:
		RenderLayout	m_layout;
		RenderMaterial	m_material;
	};
}