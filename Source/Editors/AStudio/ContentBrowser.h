#ifndef MODEL_LIST_H
#define MODEL_LIST_H

#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMainWindow>
#include <QtGui/QSortFilterProxyModel>
#include <map>
#include <string>
#include "ui_ContentBrowser.h"
#include "QDirectoryModel.h"

using namespace std;
using namespace Ares;

class QIcon;
class QTreeWidgetItem;
namespace QT_UI
{
	//----------------------------------------
	// 模型编辑器 主窗口 2010-12-30
	//----------------------------------------
	class ContentBrowser : public QMainWindow, public Ui_ContentBrowser
	{
		Q_OBJECT

		typedef map<string, QIcon>  IconMap;
	public:
		// 构造函数
		ContentBrowser( QWidget* parent = 0);

		// 析构函数
		~ContentBrowser();

	private slots:
		// 选择某实体
		void OnSelectEntity( const char* filePath);

		// 编辑某实体
		void EditSelect( const char* filePath);

		// 右键实体菜单
		void ShowMenu(const QPoint& point);

		// 移除 or 删除文件
		void ReMoveFile();

		// 添加资源
		void AddRes();

		// 添加Filter
		void AddFilter();

		// 重命名
		void ReName();

		// 名字发生改变
		void NameChanged( const QModelIndex& index);

		// 初始化模型列表
		void InitModelList();

	private slots:
		// 新建点光源
		void NewDynamicPointLight();

		// 新建方向光
		void NewDynamicDirectLight();

		// 新建环境光
		void NewDynamicAmbientLight();

		// 新建地形
		void NewTerrain();

		// 新建海洋
		void NewOcean();

		// 执行过滤
		void ApplyFilter( QString str);

	private:
		// 保存
		void  Save();

	private:
		IconMap					m_iconMap;
		QDirectoryModel*		m_model;
		QSortFilterProxyModel*	m_proxyModel;	// 排序代理模型

		QMenu*					m_entityMenu;
		QMenu*					m_filterMenu;

		string					m_rootPath;			// 主目录
		string					m_extFilter;		// 后缀名过滤
	};
}

#endif