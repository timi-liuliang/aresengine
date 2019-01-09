#pragma once

#include "ui_WorldMap.h"
#include <QtGui/QtGui>
#include <QtGui/QDockWidget>
#include <QtGui/QSortFilterProxyModel>
#include "ui_SceneInspector.h"
#include "SceneInspectorModel.h"

namespace QT_UI
{
	//--------------------------------
	// 世界地图 2011-08-16  帝林
	//--------------------------------
	class SceneInspector : public QDockWidget, public Ui_SceneInspector
	{
		Q_OBJECT

	public:
		SceneInspector( QWidget* parent = 0);
		~SceneInspector();

		// 获取MapModel
		SceneInspectorModel* GetSceneInsepctorModel() { return m_model; }

	private slots:
		// 右键实体菜单
		void ShowMenu(const QPoint& point);

		// 执行过滤
		void ApplyFilter( QString str);

		// 选择某实体
		void OnSelectEntity( const char* entityName);

	private:
		SceneInspectorModel*	m_model;		// QModel
		QSortFilterProxyModel*	m_proxyModel;	// 排序代理模型
		QMenu*					m_menu;			// 右键菜单
	};
}