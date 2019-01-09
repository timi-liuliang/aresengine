#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H

#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include "ui_PropertyEditor.h"
#include <Engine/RenderSystem/AresTerrain.h>
#include <QProperty.hpp>
#include <map>

using namespace std;
using namespace Ares;

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class CPropertyEditor : public QDockWidget, public Ui_PropertyEditor
	{
		Q_OBJECT

		typedef map<string, QWidget*> FrameList;
	public:
		// 构造函数
		CPropertyEditor( QWidget* parent = 0);

		// 显示当前选择地形属性
		void ShowProperty( Entity* entity);

	public:
		// 根据名字获取Frame
		QWidget* GetFrame( const char* frame);

		// 显示某Frame
		void ShowFrame( const char* frame);

		// 隐藏所有Frame
		void HideAllFrame();

	private:
		string				m_currName;
		QWidget*			m_currFrame;		// 当前
		FrameList			m_frameList;		// 所有属性控制面板
	};
}

#endif