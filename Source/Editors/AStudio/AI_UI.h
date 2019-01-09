#pragma once

#include <QtGui/QMainWindow>
#include <Physics/AresStaticMeshTiledBuilder.h>
#include "QPropertyModel.h"
#include "ui_AI.h"

using namespace Ares;

namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class CAIUI : public QMainWindow, public Ui_AI
	{
		Q_OBJECT

	public:
		// 构造函数
		CAIUI( QWidget* parent = 0);

	public slots:
		// 显示PolyRef
		void SetPolyRef( unsigned int startPoly, unsigned int endPoly);

		// 更新开始位置显示
		void SetStartPos( Vector3 startPos);

		// 更新结束位置显示
		void SetEndPos( Vector3 endPos);

		// 保存导航数据
		void SaveNavMesh();

	private slots:
		// Build NavMesh
		void BuildNavMesh();

		// 根据区域构建导航网格
		void BuildNavMeshByRegion();

		// 寻路
		void PathFind();

		// Show
		void Show();

		// 刷新BuildParam
		void RefreshBuildParam();

	private:
		CStaticMeshTiledBuilder::SBuildParam m_buildParam;
		QPropertyModel*						 m_pModel;			// 使用的Model
	};
}