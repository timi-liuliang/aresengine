#pragma once

#include <string>
#include <QtGui/QDockWidget>
#include <QTimer>
#include "ui_MOAnimationEdit.h"

using namespace std;

namespace QT_UI
{
	//------------------------------------------
	// CAnimationEdit 动画控制 2011-07-12 帝林
	//------------------------------------------
	class IAresAnimation;
	class CAnimationEdit  : public QDockWidget, public Ui_AnimationEdit
	{
		Q_OBJECT

		typedef map<string, QIcon>  IconMap;
	public:
		// 构造函数
		CAnimationEdit( QWidget* parent = 0);

		// 析构函数
		~CAnimationEdit();

	public slots:
		// 重置(模型选择更改)
		void Reset();

	private slots:
		// 初始化动画列表
		void InitAniList();

		// 单击
		void OnClicked();

		// 更换武器
		void SwitchWeapon();

		// 显示自定义骨骼信息
		void ShowBoneCustomInfo();

	private slots:
		// 更新自定义骨骼信息
		void UpdateBoneCustomInfo();

	private:
		//// 显示骨骼的树型结构
		//void ShowBoneHierarchy( IAresAnimation* animation);

		//// 显示常规信息
		//void ShowBoneGeneralInfo( IAresAnimation* animation);

		//// 添加骨骼
		//void AddBone( IAresAnimation* animation, QTreeWidgetItem* parentItem, size_t index);

	private:
		//QTimer*             m_pTimer;		// 计时器
		IconMap				m_iconMap;		// 图标
	};
}