#pragma once

#include "MyGUI_IVertexBuffer.h"

using namespace Ares;

namespace MyGUI
{
	//-------------------------------------
	// MyGUIBuffer 2012-10-16 帝林
	//-------------------------------------
	class MyGUIBuffer : public IVertexBuffer
	{
	public:
		MyGUIBuffer();
		~MyGUIBuffer();

		// 设置顶点数量
		virtual void setVertexCount(size_t _value);

		// 获取顶点数量
		virtual size_t getVertexCount();

		// 获取Buffer
		virtual GraphicBufferPtr GetBuffer() { return m_buffer; }

		// 获取RenderLayout
		virtual RenderLayoutPtr& GetLayout() { return m_layout; }

	private:
		int					m_vertexNum;		// 顶点数
		RenderLayoutPtr		m_layout;			// 顶点数
		GraphicBufferPtr	m_buffer;			// 数据流
	};
}