#include <algorithm>
#include "QDirectoryModel.h"
#include <boost/filesystem.hpp>

namespace Ares
{
	// 构造函数
	QDirectoryModel::QDirectoryModel()
		: QStandardItemModel(),
		  m_treeView( NULL),
		  m_activeItem( NULL)
		, m_proxy( NULL)
	{
		// 初始化图标
		m_iconMaps["filter"]       = QIcon(":/icon/Icon/folder_close.png");
		m_iconMaps["filterexpend"] = QIcon(":/icon/Icon/folder_open.png");
		m_iconMaps[".m1"]          = QIcon(":/icon/Icon/model.png");
	}

	// 设置主目录, 文件类型过滤
	void QDirectoryModel::SetRootPath( const char* rootPath, const char* extFilter, QTreeView* treeView, QSortFilterProxyModel* proxy)
	{
		m_rootPath = rootPath;

		// 解析显示的文件类型
		typedef tokenizer<char_separator<char>> tokenizer;

		string filters( extFilter);
		char_separator<char> sep(" "); 
		tokenizer tokExt( filters, sep);
		for ( tokenizer::iterator it=tokExt.begin(); it!=tokExt.end(); it++)
		{
			m_exts.push_back( *it);
		}

		m_treeView = treeView;
		m_proxy    = proxy;
		connect( m_treeView, SIGNAL(expanded(const QModelIndex&)),  this, SLOT( OnExpandedFilter(const QModelIndex&)));
		connect( m_treeView, SIGNAL(collapsed(const QModelIndex&)), this, SLOT( OnExpandedFilter(const QModelIndex&)));
		connect( m_treeView, SIGNAL(clicked(const QModelIndex&)),   this, SLOT( OnSelectedFile(const QModelIndex&)));
		connect( m_treeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT( OnEditFile(const QModelIndex&)));
	}

	// 是否支持文件类型
	bool QDirectoryModel::IsSupportExt( const string& ext)
	{
		if( !m_exts.empty())
		{
			if( std::find( m_exts.begin(), m_exts.end(), ext) == m_exts.end())
				return false;
		}

		return true;
	}

	// 刷新显示
	void QDirectoryModel::Refresh()
	{
		if( filesystem::exists( m_rootPath))
			RecursiveDir( m_rootPath, invisibleRootItem());
	}

	// 迭代目录
	void QDirectoryModel::RecursiveDir( const path& dir, QStandardItem* parentItem)
	{
		vector<QStandardItem*> dirItems;
		vector<QStandardItem*> fileItems;
		for ( directory_iterator it(dir); it!=directory_iterator(); it++)
		{		
			path   childPath = *it;
			string tDirName  = childPath.stem().string();
			string tExt      = childPath.extension().string();

			// 设置item显示数据
			if( is_directory( *it))
			{
				QStandardItem* childItem = new QStandardItem;
				childItem->setText( tDirName.c_str());
				childItem->setIcon( m_iconMaps["filter"]);
				childItem->setData( childPath.string().c_str(), Qt::UserRole);

				dirItems.push_back( childItem);

				RecursiveDir( *it, childItem);
			}
			else if( IsSupportExt( tExt))
			{
				QStandardItem* childItem = new QStandardItem;
				childItem->setText( childPath.filename().string().c_str());
				childItem->setIcon( m_iconMaps[tExt]);
				childItem->setData( childPath.string().c_str(), Qt::UserRole);

				fileItems.push_back( childItem);
			}
		}

		int tRow = 0;

		// 先插入目录
		for ( size_t i=0; i<dirItems.size(); i++)
		{
			parentItem->setChild( tRow, 0, dirItems[i]);
			tRow++;
		}

		// 插入文件
		for ( size_t i=0; i<fileItems.size(); i++)
		{
			parentItem->setChild( tRow, 0, fileItems[i]);
			tRow++;
		}
	}

	// 展开某Fiter
	void QDirectoryModel::OnExpandedFilter(const QModelIndex& pIndex)
	{
		if( m_treeView->isExpanded( pIndex))
			itemFromIndex( pIndex)->setIcon( m_iconMaps["filterexpend"]);
		else
			itemFromIndex( pIndex)->setIcon( m_iconMaps["filter"]);
	}

	// 选择某文件
	void QDirectoryModel::OnSelectedFile( const QModelIndex& pIndex)
	{
		QString filePath = m_proxy ? m_proxy->data( pIndex, Qt::UserRole).toString() : this->data( pIndex, Qt::UserRole).toString();

		// 取消激活 // 粗体显示
		if( m_activeItem)
			m_activeItem->setFont( m_treeView->font());

		// 设置激活
		m_activeItem = itemFromIndex( pIndex);
		if( m_activeItem)
		{
			// 界面响应
			QFont font = m_treeView->font();
			font.setBold( true);

			m_activeItem->setFont( font);
		}

		emit FileSelected( filePath.toStdString().c_str());
	}

	// 双击某文件
	void QDirectoryModel::OnEditFile( const QModelIndex& pIndex)
	{
		QString filePath = m_proxy ? m_proxy->data( pIndex, Qt::UserRole).toString() : this->data( pIndex, Qt::UserRole).toString();

		// 取消激活 // 粗体显示
		if( m_activeItem)
			m_activeItem->setFont( m_treeView->font());

		// 设置激活
		m_activeItem = itemFromIndex( pIndex);
		if( m_activeItem)
		{
			// 界面响应
			QFont font = m_treeView->font();
			font.setBold( true);

			m_activeItem->setFont( font);
		}

		emit FileEdit( filePath.toStdString().c_str());
	}
}