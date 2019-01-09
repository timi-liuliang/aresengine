#pragma once

#include <Engine/UISystem/MyGUI.h>

namespace Ares
{
	//--------------------------------------
	// 效果编辑逻辑 2012-8-20 帝林
	//--------------------------------------
	class UiEdit
	{
	public:
		UiEdit();

		// 新建效果
		void New();

		// 打开
		void Open( const char* layoutPath);

		// 更新
		void FrameMove();

		// 执行
		void Exec();

	private:
		MyGUI::TextBox*		m_label;		// 测试用<王鹏你妹>
	};
}