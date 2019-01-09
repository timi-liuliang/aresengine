#include <QtGui/QtGui>
#include "VideoSetting.h"
#include "PropertyEditor.h"
#include "TerrainEdit.h"
#include "LightProperty.h"
#include "SCLogic.h"
#include <QPropertyDelegate.h>
#include <QPropertyModel.h>

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	CPropertyEditor::CPropertyEditor( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
		, m_currFrame( NULL)
	{
		setupUi( this);

		m_frameList["TerrainEdit"]   = new_ TerrainEdit( dockWidgetContents);
		m_frameList["PropertyLight"] = new_ MQLightProperty( dockWidgetContents);

		// 添加widget
		for( FrameList::iterator it=m_frameList.begin(); it!=m_frameList.end(); it++)
		{
			verticalLayout_3->insertWidget( 0, it->second);
		}

		HideAllFrame();
	}

	// 设置为不可见
	void CPropertyEditor::HideAllFrame()
	{
		for ( FrameList::iterator it=m_frameList.begin(); it != m_frameList.end(); it++)
		{
			it->second->setVisible( false);
		}
	}

	// 根据名字获取Frame
	QWidget* CPropertyEditor::GetFrame( const char* frame)
	{
		FrameList::iterator it=m_frameList.find( frame);
		if( it != m_frameList.end())
			return it->second;

		return NULL;
	}

	// 显示某Frame
	void CPropertyEditor::ShowFrame( const char* frame)
	{
		if( m_currFrame && m_currName == frame)
		{
			m_currFrame->setVisible( true);
		}
		else
		{
			for ( FrameList::iterator it=m_frameList.begin(); it != m_frameList.end(); it++)
			{
				if( it->first == frame)
				{
					m_currName	= it->first;
					m_currFrame = it->second;
					it->second->setVisible( true);
				}
				else
				{
					it->second->setVisible( false);
				}
			}
		}
	}

	// 显示当前选择地形属性
	void CPropertyEditor::ShowProperty( Entity* entity)
	{
		// 根据选择的实体类型, 显示属性控制
		if( entity)
		{
			switch(entity->GetTypeDetail())
			{
			case ED_Terrain:
				{
					Terrain* terrain = dynamic_cast<Terrain*>( entity);
					if( terrain)
					{
						TerrainEdit* terrainEdit = dynamic_cast<TerrainEdit*>( m_frameList["TerrainEdit"]);
						if( terrainEdit)
						{
							terrainEdit->SetCurrTerrain( terrain);

							ShowFrame( "TerrainEdit");
						}
					}
				}
				break;
			case ED_LightAmbient:
			case ED_LightPoint:
			case ED_LightDirection:
				{
					Light* light = dynamic_cast<Light*>( entity);
					if( light)
					{
						MQLightProperty* lightProperty = dynamic_cast<MQLightProperty*>( m_frameList["PropertyLight"]);
						if( lightProperty)
						{
							lightProperty->SetCurrLight( light);

							ShowFrame( "PropertyLight");
						}
					}
				}
				break;
			default:
				{
					ShowFrame( "");
				}
				break;
			}
		}
		else
		{
			ShowFrame( "");
		}
	}
}