#include <QtGui/QtGui>
#include "MOVideoSetting.h"
#include "MOLogic.h"

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	MOVideoSetting::MOVideoSetting( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		connect( m_fillSolid,     SIGNAL( toggled(bool)), this, SLOT( UpdateFillMode()));
		connect( m_fillWireFrame, SIGNAL( toggled(bool)), this, SLOT( UpdateFillMode()));
		connect( m_fillPoint,     SIGNAL( toggled(bool)), this, SLOT( UpdateFillMode()));
	}

	// 更新VideoFillMode
	void MOVideoSetting::UpdateFillMode()
	{
		// 设置填充模式
		if( m_fillSolid->isChecked())
			MOLogic_Main->SetFillMode( 2);
		else if( m_fillWireFrame->isChecked())
			MOLogic_Main->SetFillMode( 1);
		else 
			MOLogic_Main->SetFillMode( 0);
	}
}