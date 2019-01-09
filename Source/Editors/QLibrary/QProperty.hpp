#pragma once

#include "QPropertyModel.h"
#include "QPropertyDelegate.h"

namespace QT_UI
{
	//----------------------------------
	// QProperty 2013-1-21 帝林
	// 记录Model,Delegate对
	//----------------------------------
	struct QProperty
	{
		QPropertyModel*		m_model;
		QPropertyDelegate*	m_delegate;

		// 构造函数
		QProperty( const char* config)
		{			// 设置
			m_model    = new_ QPropertyModel( config);
			m_delegate = new_ QPropertyDelegate( m_model);
		}

		// 析构函数
		~QProperty()
		{
			SAFE_DELETE( m_model);
			SAFE_DELETE( m_delegate);
		}
	};
}