#include "SceneInspector.h"
#include <QtGui/QFileDialog>
#include "SCLogic.h"
#include <boost/filesystem.hpp>

namespace QT_UI
{
	// 构造函数
	SceneInspector::SceneInspector( QWidget* parent)
		: QDockWidget( parent),
		  m_menu( NULL)
	{
		setupUi( this);

		// tree model
		m_model		 = new_ SceneInspectorModel;
		m_proxyModel = new_ QSortFilterProxyModel( this);
		m_proxyModel->setSourceModel( m_model);
		m_proxyModel->setFilterKeyColumn( 0);
		m_treeView->setModel( m_proxyModel);
		m_treeView->expandAll();
		m_model->SetTreeViewAndProxy( m_treeView, m_proxyModel);

		connect( m_treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT( ShowMenu(const QPoint&)));
		connect( m_filter,	 SIGNAL(textChanged(QString)), this, SLOT(ApplyFilter(QString)));
		connect( m_model,	 SIGNAL( FileSelected( const char*)), this, SLOT( OnSelectEntity( const char*)));
	}

	// 析构函数
	SceneInspector::~SceneInspector()
	{
	}

	// 右键实体菜单
	void SceneInspector::ShowMenu(const QPoint& point)
	{	
		if( !m_menu)
		{
			m_menu = new QMenu();

			QMenu* addMenu = new QMenu("添加");
			//addMenu->addAction( m_pActionAdd);
			//addMenu->addAction( m_pActionAddFilter);

			m_menu->addMenu( addMenu);
			m_menu->addSeparator();
			m_menu->addAction( m_actionReMove);
			m_menu->addAction( m_actionReName);
		}

		m_menu->exec( QCursor::pos());
	}

	// 执行过滤
	void SceneInspector::ApplyFilter( QString str)
	{
		QRegExp regExp( str, Qt::CaseInsensitive, QRegExp::Wildcard);
		m_proxyModel->setFilterRegExp( regExp);
	}

	// 选择某实体
	void SceneInspector::OnSelectEntity( const char* entityName)
	{
		if( entityName)
		{
			SCLogic_Main->SetHandleType( EM_HT_EDITOENTITY);
			SCLogic_EntityEdit->SetSelEntity( entityName);
		}
	}
}