#pragma once

#include <Engine/FxSystem/AresFxObject.h>
#include <Engine/FxSystem/AresFx3DPatch.h>
#include <QtGui/QDockWidget>
#include "ui_FxPropertyEdit.h"
#include "QProperty.hpp"

using namespace Ares;

namespace QT_UI
{
	//----------------------------------------
	// 对象查看器 主窗口 2012-8-28
	//----------------------------------------
	class FxPropertyEdit : public QDockWidget, public Ui_FxPropertyEdit
	{
		Q_OBJECT

	public:
		FxPropertyEdit( QWidget* parent = 0);

		// 选中FxObject
		void OnSelectFxObject( FxObject* fxObject);

	public slots:
		// 更新3D面片属性到界面
		void RefreshProperty3DPatchToUI();

		// 更新界面参数到3D面片
		void RefreshPropertyUITo3DPatch();

		// 更新粒子系统属性到界面
		void RefreshPropertyParticleSystemToUI();

		// 更新属性界面到粒子系统
		void RefreshPropertyUIToParticleSystem();

	private:
		FxObject*		m_fxObject;					// 效果对象
		QProperty*		m_fx3dpatchProperty;		// 3D面片属性
		QProperty*		m_particleSystemProperty;	// 粒子系统属性
	};
}
