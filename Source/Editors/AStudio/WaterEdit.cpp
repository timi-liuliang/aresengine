#include <QtGui/QtGui>
#include "WaterEdit.h"
#include "SCLogic.h"

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	CWaterEdit::CWaterEdit( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		connect( m_putWater, SIGNAL(toggled(bool)), this, SLOT( PutWater(bool)));
	}

	// 放置水
	void CWaterEdit::PutWater(bool checked)
	{
		// 进入设置水状态
		if( checked)
		{
			SCLogic_Main->SetHandleType( EM_HT_PUT_WATER);
		}
	}
}