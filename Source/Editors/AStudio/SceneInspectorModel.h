
#pragma once

#include <map>
#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <QtGui/QTreeView>
#include <QtGui/QStandardItemModel>
#include <QtGui/QSortFilterProxyModel>
#include <Engine/SceneSystem/AresScene.h>

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace Ares;

namespace QT_UI
{
	//---------------------------------------------
	// DirectoryModel for Qt  2012-6-3 帝林
	//---------------------------------------------
	class SceneInspectorModel : public QStandardItemModel
	{
		Q_OBJECT

		typedef map<string, QIcon>			IconMap;
		typedef map<int,	QIcon>			EnityIcon;
		typedef map<string, QStandardItem*> EntityItemMap;

	public:
		// 构造函数
		SceneInspectorModel();

		// 设置主目录, 文件类型过滤
		void SetTreeViewAndProxy( QTreeView* treeView, QSortFilterProxyModel* proxy);

		// 设置地图
		void SetScene( Scene* scene);

		// 是否支持文件类型
		bool IsSupportExt( const string& ext);

		// 刷新显示
		void Refresh();

		// 设置图标
		void SetIcon( const char* name, const QIcon& icon){ m_iconMaps[name] = icon; }

	private:
		// 迭代目录
		void RecursiveDir( const path& dir, QStandardItem* parentItem);

		// 更新区域数量显示
		void RefreshEntityListDisplay( const Scene& scene);

		// 清空
		void Reset();

	public: signals:
		// 编辑材质
		void FileSelected( const char* path);

	private slots:
		// 展开某Fiter
		void OnExpandedFilter(const QModelIndex& pIndex);

		// 选择某文件
		void OnSelectedFile( const QModelIndex& pIndex);

	private:
		QTreeView*				m_treeView;		// treeView
		QSortFilterProxyModel*	m_proxy;		// 排序代理模型
		string					m_rootPath;		// 主目录	
		vector<string>			m_exts;			// 文件类型
		IconMap					m_iconMaps;		// 图标
		EnityIcon				m_entityIcons;	// 实体图标
		map<int, QStandardItem*>m_entityTypes;	// 类型分组

		QStandardItem*			m_activeItem;	// active item
		EntityItemMap			m_entityItems;	// 实体列表
	};
}