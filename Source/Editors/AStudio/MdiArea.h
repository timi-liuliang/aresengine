#pragma once

#include <QtGui/QMdiArea>
#include "RenderWidget.h"

namespace QT_UI
{
	//-----------------------------------
	// Dx9MdiArea dx显示窗口
	//-----------------------------------
	class MOMdiArea : public QMdiArea
	{
		Q_OBJECT

	public:
		MOMdiArea( QWidget* parent = NULL);
		~MOMdiArea();

		// 开始渲染
		void BeginRender();

		// 设置渲染
		void SetRenderenable( bool scRender, bool moRender);

		// 窗口大小改变
		void ResizeWindow();

	private slots:
		// 渲染
		void  Render();

		// 重置设备
		void  ResetDevice();

	private:
		QSize				m_dx9Size;				// 调整后大小

		MORenderWidget*		m_renderWidgetMO;		// 模型渲染窗口
		MORenderWidget*		m_renderWidgetSC;		// 模型渲染窗口

		QMdiSubWindow*		m_subWindowMO;			// 子窗口
		QMdiSubWindow*		m_subWindowSC;			// 子窗口

		bool				m_scRender;				// 是否渲染场景
		bool				m_moRender;				// 是否渲染模型
	};
}