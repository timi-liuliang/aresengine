#include "MOLogic.h"
#include "MOCollisionBuild.h"
#include <Engine/RenderSystem/AresVisualShape.h>
#include <QtGui/QFileDialog>

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	CollisionBuild::CollisionBuild( QWidget* parent)
		: QDockWidget( parent)
		, m_mouseMenu( NULL)
		, m_locking( true)
	{
		setupUi( this);

		connect( m_shapeList, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectedCollisionShape()));

		connect( m_capsuleRadius, SIGNAL(valueChanged(double)), this, SLOT( UpdateUICapsuleParameterToShape()));
		connect( m_capsuleHeight, SIGNAL(valueChanged(double)), this, SLOT( UpdateUICapsuleParameterToShape()));
	}

	// 显示右键菜单
	void CollisionBuild::contextMenuEvent( QContextMenuEvent* event)
	{
		if( !m_mouseMenu)
		{
			m_mouseMenu = new_ QMenu;

			// 添加菜单
			QMenu* addMenu = new_ QMenu(tr("Add"));
			addMenu->addAction( m_actionCapsule3);
			m_mouseMenu->addMenu( addMenu);

			m_mouseMenu->addAction( m_actionDeleteCapsule);

			QMenu* fillMode = new_ QMenu("FillMode");
			fillMode->addAction( m_actionSolid);
			fillMode->addAction( m_actionWireFrame);
			m_mouseMenu->addMenu( fillMode);

			// 消息链接
			connect( m_actionCapsule3, SIGNAL(triggered()), this, SLOT(AddCapsule()));
			connect( m_actionDeleteCapsule, SIGNAL(triggered()),  this, SLOT( DeleteSelectedCapsule()));
			connect( m_actionSolid,     SIGNAL(triggered()), this, SLOT(SwitchToNormalFillMode()));
			connect( m_actionWireFrame, SIGNAL(triggered()), this, SLOT(SwitchToWireFrameFillMode()));
		}

		m_mouseMenu->exec( event->globalPos());
	}

	// add collision shape capsule
	void CollisionBuild::AddCapsule()
	{
		MOLogic_CollisionBuild->AddShape( VST_Capsule);

		// 更新形状列表显示
		UpdateTableDisplay();
	}

	// 删除当前选择
	void CollisionBuild::DeleteSelectedCapsule()
	{
		MOLogic_CollisionBuild->DeleteSelected();

		UpdateTableDisplay();
	}

	// 更新列表显示
	void CollisionBuild::UpdateTableDisplay()
	{
		m_shapeList->clear();
		const vector<VisualShape*>& shapes = MOLogic_CollisionBuild->GetShapes();
		for( size_t i=0; i<shapes.size(); i++)
		{
			QListWidgetItem* item = new_ QListWidgetItem("capsule");

			m_shapeList->addItem( item);
		}
	}

	// 选择collision shape
	void CollisionBuild::OnSelectedCollisionShape()
	{
		// 1.设置当前选择索引
		MOLogic_CollisionBuild->SetSelected( static_cast<size_t>(m_shapeList->currentRow()));

		// 2.更新界面显示
		UpdateShapeParameterToUI();
	}

	// 更新当前选择形状到界面
	void CollisionBuild::UpdateShapeParameterToUI()
	{
		// 锁定
		m_locking = true;

		// 2.隐藏所有编辑界面
		m_capsuleEditPanel->setVisible( false);

		VisualShape* vc = MOLogic_CollisionBuild->GetSelected();
		if( vc)
		{
			switch( vc->GetType())
			{
			case VST_Capsule:
				{
					VisualCapsule* capsule = dynamic_cast<VisualCapsule*>(vc);
					if( capsule)
					{
						m_capsuleRadius->setValue( capsule->GetRadius());
						m_capsuleHeight->setValue( capsule->GetHeight());
					}

					m_capsuleEditPanel->setVisible( true);
				}
				break;
			}
		}

		// 解锁
		m_locking = false;
	}

	// 更新界面数据到形状
	void CollisionBuild::UpdateUICapsuleParameterToShape()
	{
		VisualShape* vs = MOLogic_CollisionBuild->GetSelected();
		if( !m_locking && vs && vs->GetType()==VST_Capsule)
		{
			float radius = static_cast<float>(m_capsuleRadius->value());
			float height = static_cast<float>(m_capsuleHeight->value());

			VisualCapsule* vc = dynamic_cast<VisualCapsule*>(vs);
			if( vc)
				vc->SetShape( height, radius);
		}
	}

	// 导出碰撞模型
	void CollisionBuild::ExportCollisionShapes()
	{
		QString fileName = QFileDialog::getSaveFileName( this, tr("Export CollisionShapes"), "", tr("(*.cs)"));
		if( !fileName.isEmpty())
		{
			string tFileName = fileName.toStdString();

			ExportCollisionShapes( tFileName);
		}
	}

	// 导出CollisionShapes
	void CollisionBuild::ExportCollisionShapes( const string& path)
	{
		if( MOLogic_ModelEdit->GetModelType()==MT_YND)
			MOLogic_CollisionBuild->YNDSavePhysicsShapes( path.c_str());
		else
			MOLogic_CollisionBuild->SavePhysicsShapes( path.c_str());
	}

	// 导入碰撞模型
	void CollisionBuild::ImportCollisionShapes()
	{
		QString fileName = QFileDialog::getOpenFileName( this, tr("Import CollisionShapes"), "", tr("(*.cs)"));
		if( !fileName.isEmpty())
		{
			string tFileName = fileName.toStdString();

			ImportCollisionShapes( tFileName);
		}
	}

	// 导入CollisionShapes
	void CollisionBuild::ImportCollisionShapes( const string& path)
	{
		if( MOLogic_ModelEdit->GetModelType()==MT_YND)
			MOLogic_CollisionBuild->YNDLoadPhysicsShapes( path.c_str());
		else
			MOLogic_CollisionBuild->LoadPhysicsShapes( path.c_str());

		UpdateTableDisplay();
	}

	// 网络填充模式
	void CollisionBuild::SwitchToWireFrameFillMode()
	{
		MOLogic_CollisionBuild->SwitchToWireFrameFillMode();
	}

	// 正常填充模式
	void CollisionBuild::SwitchToNormalFillMode()
	{
		MOLogic_CollisionBuild->SwitchToNormalFillMode();
	}
}