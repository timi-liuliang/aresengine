#include <QtGui/QtGui>
#include "ImageProcess.h"
#include "SCLogic.h"

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	CImageProcess::CImageProcess( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		// 设置默认编辑格式
		setupUi( this);

		//g_pLogic->m_pImageProcessMgr->AddImageProgress( IImageProcessManager::EM_NIGHT_VIGION);

		connect( m_pFilter, SIGNAL( lostFocus()), this, SLOT( FilterLostFocus()));

		// 添加可处理的图象处理方式
		m_pImageProcessList->setEditTriggers( QAbstractItemView::SelectedClicked);
		QListWidgetItem* pItem = NULL;

		// 夜视
		pItem = new QListWidgetItem( "Night Vision", m_pImageProcessList);
		pItem->setCheckState( Qt::Unchecked);

		// 夜视
		pItem = new QListWidgetItem( "Radial Blur", m_pImageProcessList);
		pItem->setCheckState( Qt::Unchecked);

		// Bloom
		pItem = new QListWidgetItem( "Bloom", m_pImageProcessList);
		pItem->setCheckState( Qt::Unchecked);

		// Under Water
		pItem = new QListWidgetItem( "Under Water", m_pImageProcessList);
		pItem->setCheckState( Qt::Unchecked);

		// 热成像
		//pItem = new QListWidgetItem( "Heat Vision", m_pImageProcessList);
		//pItem->setCheckState( Qt::Unchecked);

		// Bloom
		pItem = new QListWidgetItem( "Old TV", m_pImageProcessList);
		pItem->setCheckState( Qt::Unchecked);

		// 灰度
		pItem = new QListWidgetItem( "Gray Scale", m_pImageProcessList);
		pItem->setCheckState( Qt::Unchecked);

		// 体积雾
		pItem = new QListWidgetItem( "Volume Fog", m_pImageProcessList);
		pItem->setCheckState( Qt::Unchecked);

		connect( m_pImageProcessList, SIGNAL(itemClicked( QListWidgetItem*)), this, SLOT( OnClicked( QListWidgetItem*)));
	}

	// 设置Filter提醒 
	void CImageProcess::FilterLostFocus()
	{
		m_pFilter->setText( "Filter");
	}

	//// 转换
	//static Ares::IImageProcessManager::ImageProcessType Transform( QString&  text)
	//{
	//	if( text == "Night Vision")
	//		return IImageProcessManager::EM_NIGHT_VIGION;
	//	else if( text == "Radial Blur")
	//		return IImageProcessManager::EM_RADIAL_BLUR;
	//	else if( text == "Volume Fog")
	//		return IImageProcessManager::EM_FOG;
	//	else if( text == "Bloom")
	//		return IImageProcessManager::EM_BLOOM;
	//	else if ( text == "Under Water")
	//		return IImageProcessManager::EM_UNDER_WATER;
	//	else if( text == "Old TV")
	//		return IImageProcessManager::EM_OLD_TV;

	//	return IImageProcessManager::EM_GRAY_SCALE;
	//}

	// 双击
	void CImageProcess::OnClicked( QListWidgetItem* pItem)
	{
		if( pItem->checkState() == Qt::Checked)
		{
			pItem->setCheckState( Qt::Unchecked);

			//Logic_ImageProcess->DisableImageProcess( Transform( pItem->text()));
		}
		else
		{
			pItem->setCheckState( Qt::Checked);
			//Logic_ImageProcess->EnableImageProcess( Transform( pItem->text()));
		}
	}
}