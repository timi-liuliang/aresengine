#include "QPropertyModel.h"

namespace QT_UI
{
	// 构造函数
	QPropertyModel::QPropertyModel( const char* fileName, QObject* parent/*= 0*/)
		: QStandardItemModel( parent)
	{
		// 创建Item
		m_cfg = new TiXmlDocument(fileName);
		if( m_cfg->LoadFile())
			ParseCfg();
	} 

	QPropertyModel::~QPropertyModel()
	{
		SAFE_DELETE( m_cfg);
		//SAFE_DELETE_VECTOR( m_standItems);

	}

	// 设置数据
	void  QPropertyModel::setupModelData( TiXmlDocument* cfg)
	{
		if( !cfg)
			return;

		m_cfg = cfg;

		ParseCfg();
	}

	// 解析配置文件
	void  QPropertyModel::ParseCfg()
	{
		clear();

		// headerLabel
		TiXmlElement* root = m_cfg->FirstChildElement("cfg");
		TiXmlElement* headers = root->FirstChildElement("headers");
		if( headers)
		{
			QStringList headerLables;
			TiXmlElement* header = headers->FirstChildElement("header");
			while( header)
			{
				headerLables << header->Attribute( "text");

				header = header->NextSiblingElement("header");
			}

			setHorizontalHeaderLabels( headerLables);
		}

		// 表项
		addChildItem( root->FirstChildElement("item"), this->invisibleRootItem());
	}

	// 递归添加
	void  QPropertyModel::addChildItem( TiXmlElement* pFirstChildElement, QStandardItem* pParentItem)
	{
		while( pFirstChildElement)
		{
			QString text = QString::fromUtf8( pFirstChildElement->Attribute("text"));

			int  bold = false;
			pFirstChildElement->Attribute( "bold", &bold);

			int row=0, col=0;
			pFirstChildElement->Attribute( "row", &row);
			pFirstChildElement->Attribute( "col", &col);

			QStandardItem* item = addChildItem( pParentItem, text.toStdString().c_str(), bold, row, col, pFirstChildElement->Attribute("widget"), pFirstChildElement->Attribute("property"));

			addChildItem( pFirstChildElement->FirstChildElement("item"), item);

			pFirstChildElement = pFirstChildElement->NextSiblingElement("item");
		}
	}

	// 添加子结点
	QStandardItem* QPropertyModel::addChildItem( QStandardItem* parent, const char* text, bool bold, int row, int col, const char* widget, const char* property)
	{
		QStandardItem* item = new QStandardItem( text);
		QFont font = item->font(); font.setBold( bold);
		item->setFont( font);

		item->setData( widget, Qt::UserRole);
		item->setData( property, Qt::DisplayPropertyRole);

		parent->setChild( row, col, item);

		m_standItems.push_back( item);

		return item;
	}

	// 除除子结点
	void QPropertyModel::removeChildItem( QStandardItem* parent)
	{
		for( vector<QStandardItem*>::iterator it=m_standItems.begin(); it!=m_standItems.end();)
		{
			if( (*it)->parent()==parent)
				it = m_standItems.erase( it);
			else
				it++;
		}

		for( int i=0; i<parent->columnCount(); i++)
			parent->removeColumn( i);
	}

	// 根据text查找item
	QStandardItem* QPropertyModel::findItemByText( const char* text)
	{
		foreach_( QStandardItem* item, m_standItems)
		{
			QString xx = item->text();
			if( item->text()==text)
				return item;
		}

		return NULL;
	}

	// 根据属性查找数据
	bool QPropertyModel::findValue( QVariant& value,const QString& propertyName)
	{
		AttribList::iterator it = m_valueList.find( propertyName);
		if( it != m_valueList.end())
		{
			value = it->second;

			return true;
		}

		return false;
	}

	// 设置数据
	void QPropertyModel::setValue( const QString& propertyName, QVariant value)
	{
		// 正则表达式验证
		//if( value.mmdfkdf)
		//	return;

		if( m_valueList[propertyName] != value)
		{
			m_valueList[propertyName] = value;

			Signal_ValueChanged();
		}
	}
}