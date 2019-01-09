#include "UIEdit.h"
#include <boost/array.hpp>
#include <Engine/AresRoot.h>

namespace Ares
{
	// 构造函数
	UiEdit::UiEdit()
		: m_label( NULL)
	{
	}

	// 新建效果
	void UiEdit::New()
	{
		
	}

	// 打开
	void UiEdit::Open( const char* layoutPath)
	{
		using namespace MyGUI;

		const VectorWidgetPtr& layout = AUiLayoutMgr.loadLayout( layoutPath);
	}

	// 更新
	void UiEdit::FrameMove()
	{
	}

	// 执行
	void UiEdit::Exec()
	{
	}
}