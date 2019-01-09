#pragma once

#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/filesystem/path.hpp>
#include <QtGui/QTreeView>
#include <QtGui/QStandardItemModel>
#include <QtGui/QSortFilterProxyModel>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

namespace Ares
{
	//---------------------------------------------
	// DirectoryModel for Qt  2012-6-3 帝林
	//---------------------------------------------
	class QDirectoryModel : public QStandardItemModel
	{
		Q_OBJECT

		typedef map<string, QIcon>  IconMap;

	public:
		// 构造函数
		QDirectoryModel();

		// 设置主目录, 文件类型过滤
		void SetRootPath( const char* rootPath, const char* extFilter, QTreeView* treeView, QSortFilterProxyModel* proxy);

		// 是否支持文件类型
		bool IsSupportExt( const string& ext);

		// 刷新显示
		void Refresh();

		// 设置图标
		void SetIcon( const char* name, const QIcon& icon){ m_iconMaps[name] = icon; }

	private:
		// 迭代目录
		void RecursiveDir( const path& dir, QStandardItem* parentItem);

	public: signals:
		// 编辑材质
		void FileSelected( const char* path);

		// 编辑文件
		void FileEdit( const char* path);

	private slots:
		// 展开某Fiter
		void OnExpandedFilter(const QModelIndex& pIndex);

		// 选择某文件
		void OnSelectedFile( const QModelIndex& pIndex);

		// 双击某文件
		void OnEditFile( const QModelIndex& pIndex);

	private:
		QTreeView*				m_treeView;		// treeView
		QSortFilterProxyModel*	m_proxy;	// 排序代理模型
		string					m_rootPath;		// 主目录	
		vector<string>			m_exts;			// 文件类型
		IconMap					m_iconMaps;		// 图标
		QStandardItem*			m_activeItem;		// active item
	};
}