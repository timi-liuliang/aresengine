#pragma once

#include <map>
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <Core/AresCore.h>
#include <boost/signals2/signal.hpp>
#include <boost/any.hpp>
#include <Core/3rdParty/TinyXml/tinyxml.h>

using namespace std;

namespace QT_UI
{
	//------------------------------------------
	// 属性专用(用途有限) 2010-03-15  帝林
	//------------------------------------------
	class QPropertyModel : public QStandardItemModel
	{	
		Q_OBJECT

		typedef map<QString, QVariant> AttribList;
	public:
		// 构造函数
		QPropertyModel( const char* fileName, QObject* parent= 0);
		~QPropertyModel();

		// 设置数据
		void  setupModelData( TiXmlDocument* cfg);

		// 根据属性查找数据
		bool findValue( QVariant& value,const QString& propertyName);

		// 设置数据
		void setValue( const QString& propertyName, QVariant value);

		// 根据text查找item
		QStandardItem* findItemByText( const char* text);

		// 添加子结点
		QStandardItem* addChildItem( QStandardItem* parent, const char* text, bool bold, int row, int col, const char* widget, const char* property);

		// 除除子结点
		void removeChildItem( QStandardItem* parent);

	public:
		// values changed signals
		boost::signals2::signal<void()> Signal_ValueChanged;

	private:
		// 解析配置文件
		void  ParseCfg();

		// 递归添加
		void  addChildItem( TiXmlElement* pFirstChildElement, QStandardItem* pParentItem);

		// 添加wigetMap
		void addWidgetCfg(QStandardItem* item, const char* type);

	private:
		TiXmlDocument*				m_cfg;
		AttribList					m_valueList;
		vector<QStandardItem*>		m_standItems;		// 表项
	};
}