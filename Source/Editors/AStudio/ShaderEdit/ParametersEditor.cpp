#include <QtGui/QtGui>
#include "ParametersEditor.h"
#include "SHLogic.h"
#include <Core/3rdParty/RapidXml/rapidxml.hpp>
#include <Core/3rdParty/RapidXml/rapidxml_utils.hpp>
#include <boost/lexical_cast.hpp>
#include <Engine/AresRoot.h>

using namespace std;
using namespace boost;
using namespace rapidxml;
using namespace Ares;

namespace Ares
{
	// 构造函数
	UIParametersEditor::UIParametersEditor( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		// 设置
		m_pTreeView->setAlternatingRowColors(true);
		m_pTreeView->expandAll();

		m_shaderWidget->setVisible( false);
	}

	// 材质属性改变
	void UIParametersEditor::MaterialPropertyChanged()
	{
	}

	// 打开材质
	void UIParametersEditor::OpenMaterial( const char* materialPath, size_t index)
	{
		file<>         fdoc( materialPath);
		xml_document<> doc;
		doc.parse<0>( fdoc.data());

		// 主结点, 材质结点
		xml_node<>* rootNode = doc.first_node();
		size_t		tMatNum = lexical_cast<size_t>( rootNode->first_attribute("size")->value());
		if( index<tMatNum)
		{
			xml_node<>* matNode  = rootNode->first_node("material");
			while ( matNode)
			{
				if( (index--)==0)
				{
					string methodPath = matNode->first_attribute("method")->value();

					m_materialPath->setText( methodPath.c_str());

					break;
				}

				matNode = matNode->next_sibling("material");
			}
		}

		ALogSystem.Warning( "open material failed!");
	}
}