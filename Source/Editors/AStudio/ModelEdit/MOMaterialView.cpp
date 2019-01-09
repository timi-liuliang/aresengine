#include "MOLogic.h"
#include "MOMainWindow.h"

namespace QT_UI
{
	// 构造函数
	MateriaViewer::MateriaViewer( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		QObject::connect( m_materialList, SIGNAL(currentIndexChanged(int)), this, SLOT( OnSelectedMaterialChanged( int)));
		QObject::connect( m_editMaterial, SIGNAL(clicked()), this, SLOT( OnEditMaterial()));
	}

	// 打开材质文件
	void MateriaViewer::RefreshDisplay()
	{
		// 清空
		m_materialList->clear();
		m_materialPath->clear();

		ModelPtr& m1Entity = MOLogic_ModelEdit->GetCurrEntity();
		if( m1Entity)
		{
			size_t renderableNum = m1Entity->GetMeshRenderableNum();
			for ( size_t i=0; i<renderableNum; i++)
			{
				const MeshRenderable& m1Renderable = m1Entity->GetMeshRenderable(i);

				char desc[1024];
				sprintf_s( desc, "MeshRenderable %d - %d Primitives", i, m1Renderable.GetPrimitiveCount());

				m_materialList->addItem( QIcon(":/icon/Icon/model.png"), desc);

				QString filePath = m1Renderable.m_materialPath.c_str();
				//MOUI_FileSystemWatcher->addPath( filePath);
			}
		}

		m_materialList->setCurrentIndex( m_materialList->count()-1);
	}

	// 材质选中改变
	void MateriaViewer::OnSelectedMaterialChanged( int idx)
	{
		ModelPtr& m1Entity = MOLogic_ModelEdit->GetCurrEntity();
		if( m1Entity)
		{
			if( idx>=0 && idx<(int)m1Entity->GetMeshRenderableNum())
			{
				const MeshRenderable& m1Renderable = m1Entity->GetMeshRenderable( idx);

				// 材质纹理
				m_materialImage->setIcon( QIcon(":/icon/Icon/material.jpg"));

				// 材质路径
				m_materialPath->setText( m1Renderable.m_materialPath.c_str());
			}
		}
	}

	// 编辑材质
	void MateriaViewer::OnEditMaterial()
	{
		ModelPtr& m1Entity = MOLogic_ModelEdit->GetCurrEntity();
		if( m1Entity)
		{
			int idx = m_materialList->currentIndex();
			if( idx>=0 && idx<(int)m1Entity->GetMeshRenderableNum())
			{
				const MeshRenderable& m1Renderable = m1Entity->GetMeshRenderable( idx);

				char cmd[1024];
#if ARES_DEBUG
				sprintf_s( cmd, "MaterialEditor_d.exe om %s %d", m1Renderable.m_materialPath.c_str(), m1Renderable.m_materialIdx);
#else
				sprintf_s( cmd, "MaterialEditor.exe om %s %d", m1Renderable.m_materialPath.c_str(), m1Renderable.m_materialIdx);
#endif						
				WinExec( cmd, SW_NORMAL);
			}
		}
	}
}