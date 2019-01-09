#pragma once

#include <QtGui/QDockWidget>
#include <QtGui/QtEvents>
#include "ui_MaterialExpression.h"

namespace Ares
{
	//---------------------------------
	// 材质表达式 2013-3-19 帝林
	//---------------------------------
	class UIMaterialExpression : public QDockWidget, public Ui_MaterialExpression
	{
		Q_OBJECT

	public:
		// 构造函数
		UIMaterialExpression( QWidget* parent= NULL);

	protected slots:
		// 开启拖拽事件
		void OnSelectExpression();
	};
}