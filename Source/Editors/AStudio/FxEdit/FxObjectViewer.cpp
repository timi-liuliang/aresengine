#include <QtGui/QtGui>
#include <Core/AresPlatform.h>
#include "FxObjectViewer.h"
#include "FxMainWindow.h"
#include "FxLogic.h"

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	FxObjectViewer::FxObjectViewer( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
		, m_mouseMenu( NULL)
	{
		setupUi( this);

		connect( m_listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectedFxObject()));

		m_icons.resize( FOT_Max);
		//m_icons[FOT_2DPatch] = ;
		m_icons[FOT_3DPatch]		= QIcon( ":/new/prefix1/Icon/3dPatch.png");
		m_icons[FOT_ParticleSystem] = QIcon( ":/new/prefix1/Icon/particlesystem.png");
	}

	// 显示右键菜单
	void FxObjectViewer::contextMenuEvent( QContextMenuEvent* event)
	{
		if( !m_mouseMenu)
		{
			m_mouseMenu = new_ QMenu( this);

			// 添加菜单
			QMenu* addMenu = new_ QMenu(tr("Add Object"));
			addMenu->addAction( m_object3DPatch);
			addMenu->addAction( m_objectParticleSystem);
			m_mouseMenu->addMenu( addMenu);
			m_mouseMenu->addAction( m_actionDelete);

			// 消息链接
			connect( m_object3DPatch,		SIGNAL(triggered()), this, SLOT( Add3dPatch()));
			connect( m_objectParticleSystem,SIGNAL(triggered()), this, SLOT( AddPartcileSystem()));
			connect( m_actionDelete,		SIGNAL(triggered()), this, SLOT( DeleteSelectedObject()));
		}

		m_mouseMenu->exec( event->globalPos());
	}

	// 添加3D面片
	void FxObjectViewer::Add3dPatch()
	{
		FxLogic_FxEdit->AddObject( FOT_3DPatch);

		// 界面
		UpdateObjectListDisplay();
	}

	// 添加粒子系统
	void FxObjectViewer::AddPartcileSystem()
	{
		FxLogic_FxEdit->AddObject( FOT_ParticleSystem);

		// 界面
		UpdateObjectListDisplay();
	}

	// 选中特效对象
	void FxObjectViewer::OnSelectedFxObject()
	{
		// 1.设置当前选择索引
		int index = static_cast<int>(m_listWidget->currentRow());
		FxEntity* fxEntity = FxLogic_FxEdit->GetFxEntity();
		if( fxEntity)
		{
			FxObject* fxObject = fxEntity->GetObjectByIndex( index);

			FxUI_PropertyEdit->OnSelectFxObject( fxObject);

			FxLogic_FxEdit->SetSelected( index);
		}
	}

	// 删除选中Object
	void FxObjectViewer::DeleteSelectedObject()
	{
		int index = static_cast<int>(m_listWidget->currentRow());
		FxLogic_FxEdit->DeleteSelected( index);

		UpdateObjectListDisplay();
	}

	// 组织效果对象显示名称
	QString FxObjectViewer::GetFxObjectDisplayName( FxObject* object)
	{
		A_ASSERT( object);

		QString result = object->GetName();
		switch( object->GetType())
		{
		case FOT_3DPatch:
			result += QString::fromLocal8Bit("(3DPatch)");
			break;
		case FOT_ParticleSystem:
			result += QString::fromLocal8Bit("(ParticleSystem)");
			break;
		}

		return result;
	}

	// 更新列表显示
	void FxObjectViewer::UpdateObjectListDisplay()
	{
		m_listWidget->clear();

		FxEntity* fxEntity = FxLogic_FxEdit->GetFxEntity();
		if( fxEntity)
		{
			for( int i=0; i<fxEntity->GetNumObjects(); i++)
			{
				FxObject* fxObject = fxEntity->GetObjectByIndex( i);
				if( fxObject)
				{
					QListWidgetItem* item = new_ QListWidgetItem( GetFxObjectDisplayName( fxObject));
					item->setIcon( m_icons[fxObject->GetType()]);
					item->setCheckState( Qt::Checked);

					m_listWidget->addItem( item);
				}
			}
		}
	}
}