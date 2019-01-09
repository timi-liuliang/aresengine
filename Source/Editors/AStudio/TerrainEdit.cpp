#include <QtGui/QtGui>
#include "TerrainEdit.h"
#include "SCLogic.h"
#include <Engine/SceneSystem/AresScene.h>

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	TerrainEdit::TerrainEdit( QWidget* parent/* = 0*/)
		: QWidget( parent)
	{
		setupUi( this);

		m_imageRadios[0] = Texture0; m_imageRadios[1] = Texture1; m_imageRadios[2] = Texture2; 
		m_imageRadios[3] = Texture3; m_imageRadios[4] = Texture4; m_imageRadios[5] = Texture5;
		m_imageRadios[6] = Texture6; m_imageRadios[7] = Texture7; m_imageRadios[8] = TextureBlend1;
		m_imageRadios[9] = TextureBlend2;

		// 更新地形纹理显示
		UpdateTerrainTexture();

		connect( m_pTerrainEditSize, SIGNAL(valueChanged(int)), this, SLOT( UpdateTerrainEditSize(int)));

		connect( RadioEMT1, SIGNAL(clicked()), this, SLOT(UpdateTerrainHandleType()));
		connect( RadioEMT2, SIGNAL(clicked()), this, SLOT(UpdateTerrainHandleType()));
		connect( RadioEMT3, SIGNAL(clicked()), this, SLOT(UpdateTerrainHandleType()));
		connect( RadioEMT4, SIGNAL(clicked()), this, SLOT(UpdateTerrainHandleType()));
		connect( RadioEMT5, SIGNAL(clicked()), this, SLOT(UpdateTerrainHandleType()));

		connect( Texture0, SIGNAL(clicked()), this, SLOT(UpdateTexture()));
		connect( Texture1, SIGNAL(clicked()), this, SLOT(UpdateTexture()));
		connect( Texture2, SIGNAL(clicked()), this, SLOT(UpdateTexture()));
		connect( Texture3, SIGNAL(clicked()), this, SLOT(UpdateTexture()));
		connect( Texture4, SIGNAL(clicked()), this, SLOT(UpdateTexture()));
		connect( Texture5, SIGNAL(clicked()), this, SLOT(UpdateTexture()));
		connect( Texture6, SIGNAL(clicked()), this, SLOT(UpdateTexture()));
		connect( Texture7, SIGNAL(clicked()), this, SLOT(UpdateTexture()));

		connect( Texture0, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( Texture1, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( Texture2, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( Texture3, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( Texture4, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( Texture5, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( Texture6, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( Texture7, SIGNAL(imageChanged(QString)), this, SLOT(UpdateTexture(QString)));
		connect( TextureBlend1, SIGNAL(imageChanged(QString)), this, SLOT(ImportTexBlend(QString)));
		connect( TextureBlend2, SIGNAL(imageChanged(QString)), this, SLOT(ImportTexBlend(QString)));

		// 高度图导入导出
		connect( m_importTool, SIGNAL(clicked()), this, SLOT( SelectImportHeightMap()));
		connect( m_import,     SIGNAL(clicked()), this, SLOT( ImportTerrainHeight()));

		connect( m_pUVScale, SIGNAL(valueChanged(double)), this, SLOT(UpdateUVScale(double)));

		// 初始化显示
		SetCurrTerrain( NULL);
	}

	// 更新地形纹理显示
	void TerrainEdit::UpdateTerrainTexture()
	{
		for ( int i=0; i<MAX_TEXTURE; i++)
		{
			if( SCLogic_Main && SCLogic_Map->GetTerrainTexture( i))
				m_imageRadios[i]->SetImage( SCLogic_Map->GetTerrainTexture( i));
			else
				m_imageRadios[i]->SetImage(":/icon/Icon/null.png");
		}
	}

	// 编辑地形尺寸更改
	void TerrainEdit::UpdateTerrainEditSize( int size)
	{
		SCLogic_Map->GetTerrainMeshEditorData().size = size;
	}

	// 编辑地形类型
	void TerrainEdit::UpdateTerrainHandleType()
	{
		// 更改编辑类型
		if ( RadioEMT1->isChecked())
		{
			SCLogic_Map->GetTerrainMeshEditorData().type = STerrainMeshEditorData::EM_UP_HEAVAL;
		}
		else if ( RadioEMT2->isChecked())
		{
			SCLogic_Map->GetTerrainMeshEditorData().type = STerrainMeshEditorData::EM_DEBASE;
		}
		else if ( RadioEMT3->isChecked())
		{
			SCLogic_Map->GetTerrainMeshEditorData().type = STerrainMeshEditorData::EM_CALM1;
		}
		else if ( RadioEMT4->isChecked())
		{
			SCLogic_Map->GetTerrainMeshEditorData().type = STerrainMeshEditorData::EM_CALM2;
		}
		else
		{
			SCLogic_Map->GetTerrainMeshEditorData().type = STerrainMeshEditorData::EM_CRAGGEDNESS;
		}

		SCLogic_Main->SetHandleType( EM_HT_EDITORTERRAIN);
	}

	// 编辑地形纹理
	void TerrainEdit::UpdateTexture()
	{
		SCLogic_Main->SetHandleType( EM_HT_EDITTERRAINTEXTURE);

		for ( int i=0; i<MAX_TEXTURE; i++)
		{
			if( m_imageRadios[i]->isChecked())
			{
				SCLogic_Map->GetTerrainMeshEditorData().activeTexture = (STerrainMeshEditorData::TextureNum)i;

				// 同步uv缩放值
				m_pUVScale->setValue( SCLogic_Map->GetUVScale(i));
			}
		}
	}

	// 导入地形纹理混合度
	void TerrainEdit::ImportTexBlend( QString str)
	{
		if( TextureBlend1->isChecked())
		{
			SCLogic_Map->ImportTerrainTexBlend( 0, str.toStdString().c_str());
		}
		else
		{
			SCLogic_Map->ImportTerrainTexBlend( 1, str.toStdString().c_str());
		}
	}

	// 更改地形纹理 
	void TerrainEdit::UpdateTexture( QString str)
	{
		for ( int i=0; i<MAX_TEXTURE; i++)
		{
			if( m_imageRadios[i]->isChecked())
				SCLogic_Map->SetTerrainTexture( i, str.toStdString().c_str());
		}
	}

	// 更改uvScale
	void TerrainEdit::UpdateUVScale(double uvScale)
	{
		SCLogic_Map->SetUVScale( SCLogic_Map->GetTerrainMeshEditorData().activeTexture, (float)uvScale);
	}

	// 选择导入高度图
	void TerrainEdit::SelectImportHeightMap()
	{
		QString qFileName = QFileDialog::getOpenFileName( this, tr("import height map"), "", tr("*.hei"));

		QDir dir( QDir::currentPath());
		qFileName = dir.relativeFilePath( qFileName);

		m_importHeiText->setText( qFileName);
	}

	// 导入高度图
	void TerrainEdit::ImportTerrainHeight()
	{
		string heiPath = m_importHeiText->text().toStdString();
		if( !heiPath.empty())
		{
			SCLogic_Map->ImportTerrainHeight( heiPath.c_str());
		}
	}

	// 设置当前编辑地形
	void TerrainEdit::SetCurrTerrain( Terrain* pTerrain)
	{
		if( !pTerrain)
		{
			m_heightFieldGB->setVisible( false);
			m_textureGP->setVisible( false);
			m_uvGP->setVisible(false);
		}
		else
		{
			m_heightFieldGB->setVisible( true);
			m_textureGP->setVisible( true);
			m_uvGP->setVisible(true);

			UpdateTerrainTexture();
		}

		adjustSize();
	}
}