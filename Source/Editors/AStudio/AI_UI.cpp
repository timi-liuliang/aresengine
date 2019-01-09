#include <QtGui/QtGui>
#include <QPropertyDelegate.h>
#include <QPropertyModel.h>
#include "AI_UI.h"
#include "SCLogic.h"

namespace QT_UI
{
	// 构造函数
	CAIUI::CAIUI( QWidget* parent/* = 0*/)
		: QMainWindow( parent)
	{
		setupUi( this);

		connect( m_pPushButtonBuild, SIGNAL(clicked()),		this, SLOT(BuildNavMesh()));
		connect( m_pSaveNavMesh,     SIGNAL(clicked()),		this, SLOT(SaveNavMesh()));
		connect( m_pathFind,		 SIGNAL(clicked()),		this, SLOT(PathFind()));
		connect( m_originMeshShow,	 SIGNAL(toggled(bool)), this, SLOT(Show()));
		connect( n_navMeshShow,		 SIGNAL(toggled(bool)), this, SLOT(Show()));
		connect( n_navMeshDetailShow,SIGNAL(toggled(bool)), this, SLOT(Show()));
		connect( m_pathLineShow,	 SIGNAL(toggled(bool)), this, SLOT(Show()));
		connect( m_regionBuild,		 SIGNAL(clicked()),		this, SLOT(BuildNavMeshByRegion()));

		// 设置
		m_pModel = new QPropertyModel("config\\NavBuild.property", this);
		QPropertyDelegate* pDelegate = new QPropertyDelegate( m_pModel);

		m_pTreeView->setModel( m_pModel);
		m_pTreeView->setItemDelegate( pDelegate);

		m_pTreeView->setAlternatingRowColors(true);
		m_pTreeView->expandAll();

		// 添加默认值
		m_pModel->setValue( "Cell Size",         m_buildParam.m_fCellSize);
		m_pModel->setValue( "Cell Height",       m_buildParam.m_fCellHeight);

		m_pModel->setValue( "Height",            m_buildParam.m_fAgentHeight);
		m_pModel->setValue( "Radius",            m_buildParam.m_fAgentRadius);
		m_pModel->setValue( "Max Climb",         m_buildParam.m_fAgentClimb);
		m_pModel->setValue( "Max Slope",         m_buildParam.m_fAgentMaxSlope);

		m_pModel->setValue( "Min Region Size",   m_buildParam.m_fRegionMinSize);
		m_pModel->setValue( "Merged Region Size",m_buildParam.m_fRegionMergeSize);

		m_pModel->setValue( "Max Edge Length",   m_buildParam.m_fEdgeMaxLen);
		m_pModel->setValue( "Max Edge Error",    m_buildParam.m_fEdgeMaxError);
		m_pModel->setValue( "Verts Per Poly",    m_buildParam.m_iVertesPerPoly);

		m_pModel->setValue( "Sample Distance",   m_buildParam.m_fDetailSampleDist);
		m_pModel->setValue( "Max Sample Error",  m_buildParam.m_fDetailSampleMaxError);
	}

	// 刷新BuildParam
	void CAIUI::RefreshBuildParam()
	{
		QVariant variant;

		if( m_pModel->findValue( variant, "Cell Size"))	    m_buildParam.m_fCellSize = variant.toFloat();
		if( m_pModel->findValue( variant, "Cell Height"))	m_buildParam.m_fCellHeight = variant.toFloat();

		if( m_pModel->findValue( variant, "Height"))	    m_buildParam.m_fAgentHeight = variant.toFloat();
		if( m_pModel->findValue( variant, "Radius"))	    m_buildParam.m_fAgentRadius = variant.toFloat();
		if( m_pModel->findValue( variant, "Max Climb"))	    m_buildParam.m_fAgentClimb = variant.toFloat();
		if( m_pModel->findValue( variant, "Max Slope"))	    m_buildParam.m_fAgentMaxSlope = variant.toFloat();

		if( m_pModel->findValue( variant, "Min Region Size"))	m_buildParam.m_fRegionMinSize = variant.toFloat();
		if( m_pModel->findValue( variant, "Merged Region Size"))m_buildParam.m_fRegionMergeSize = variant.toFloat();

		if( m_pModel->findValue( variant, "Max Edge Length"))	m_buildParam.m_fEdgeMaxLen = variant.toFloat();
		if( m_pModel->findValue( variant, "Max Edge Error"))	m_buildParam.m_fEdgeMaxError = variant.toFloat();
		if( m_pModel->findValue( variant, "Verts Per Poly"))	m_buildParam.m_iVertesPerPoly = variant.toInt();

		if( m_pModel->findValue( variant, "Sample Distance"))	m_buildParam.m_fDetailSampleDist = variant.toInt();
		if( m_pModel->findValue( variant, "Max Sample Error"))	m_buildParam.m_fDetailSampleMaxError = variant.toInt();
	}

	// Build NavMesh
	void CAIUI::BuildNavMesh()
	{
		RefreshBuildParam();

		Logic_AI->BuildNavMesh( m_buildParam);

		Show();
	}

	// 根据区域构建导航网格
	void CAIUI::BuildNavMeshByRegion()
	{
		RefreshBuildParam();

		Logic_AI->BuildNavMeshByRegion( m_buildParam);
	}

	// 保存导航数据
	void CAIUI::SaveNavMesh()
	{
		QString fileName = QFileDialog::getSaveFileName( this, tr("Save Nav Mesh"), "", tr("(*.nav)"));
		if( !fileName.isEmpty())
		{
			string tFileName = fileName.toStdString();

			Logic_AI->SaveNavMesh( tFileName.c_str());
		}
	}

	// 寻路
	void CAIUI::PathFind()
	{
		Vector3 startPos;
		Vector3 endPos;

		startPos[0] = (float)m_sp0->value();
		startPos[1] = (float)m_sp1->value();
		startPos[2] = (float)m_sp2->value();

		endPos[0]   = (float)m_ep0->value();
		endPos[1]   = (float)m_ep1->value();
		endPos[2]  = (float)m_ep2->value();

		Logic_AI->PathFind( startPos, endPos);
	}

	// 更新开始位置显示
	void CAIUI::SetStartPos( Vector3 startPos)
	{
		m_sp0->setValue( startPos[0]);
		m_sp1->setValue( startPos[1]);
		m_sp2->setValue( startPos[2]);
	}

	// 更新结束位置显示
	void CAIUI::SetEndPos( Vector3 endPos)
	{
		m_ep0->setValue( endPos[0]);
		m_ep1->setValue( endPos[1]);
		m_ep2->setValue( endPos[2]);
	}

	// Show
	void CAIUI::Show()
	{
		Logic_AI->NavDebugShow( m_originMeshShow->isChecked(), n_navMeshShow->isChecked(), n_navMeshDetailShow->isChecked(), m_pathLineShow->isChecked());
	}

	// 显示PolyRef
	void CAIUI::SetPolyRef( unsigned int startPoly, unsigned int endPoly)
	{
		if( startPoly)
		{
			char tText[1024];
			sprintf( tText, "StartPolyID: %d", startPoly);

			m_startPolyID->setText( tText);
		}

		if( endPoly)
		{
			char tText[1024];
			sprintf( tText, "EndPolyID:   %d", endPoly);

			m_endPolyID->setText( tText);
		}
	}
}