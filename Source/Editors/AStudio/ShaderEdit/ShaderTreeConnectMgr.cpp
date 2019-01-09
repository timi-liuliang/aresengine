#include "ShaderTreeConnectMgr.h"
#include "SHMainWindow.h"
#include "SHLogic.h"

namespace Ares
{
	// 新建链接
	void ShaderTreeConnectMgr::CreateConnect()
	{
		DeleteEditConnect();

		m_curveEdit = new_ ST::QBezierCurve();

		SHUI_ShaderTreeDisplay->viewport()->repaint();
	}

	// 完成链接
	void ShaderTreeConnectMgr::FinishConnect( QExpressionInputOutput* end)
	{
		if( m_curveEdit && end)
		{
			m_curveEdit->BindEnd( end);

			SHLogic_ShaderTreeEdit->AddConnection( m_curveEdit->m_start->GetID(), m_curveEdit->m_end->GetID());

			m_curveEdit = NULL;
		}

		SHUI_ShaderTreeDisplay->UpdateShaderTreeDisplay();
	}

	// 删除链接
	void ShaderTreeConnectMgr::DeleteEditConnect()
	{
		SAFE_DELETE( m_curveEdit);

		SHUI_ShaderTreeDisplay->viewport()->repaint();
	}

	// 获取链接
	QBezierCurve* ShaderTreeConnectMgr::EditConnect( ST::QExpressionInputOutput* end)
	{
		DeleteEditConnect();

		for( vector<ST::QBezierCurve*>::iterator it=m_curves.begin(); it!=m_curves.end(); )
		{
			if( (*it)->m_end==end)
			{
				SHLogic_ShaderTreeEdit->DelConnection( (*it)->m_end->GetID());

				swap( (*it), m_curveEdit);
				it = m_curves.erase( it);
			}
			else
				it++;
		}

		SHUI_ShaderTreeDisplay->UpdateShaderTreeDisplay();

		return m_curveEdit;
	}

	// 当输入输出移动
	void ShaderTreeConnectMgr::OnMove( QExpressionInputOutput* ioput)
	{
		for( size_t i=0; i<m_curves.size(); i++)
		{
			if( m_curves[i] && ( (m_curves[i]->m_start==ioput) || (m_curves[i]->m_end==ioput)))
				m_curves[i]->OnMove();
		}
	}

	// 重置
	void ShaderTreeConnectMgr::Reset()
	{
		foreach_( ST::QBezierCurve* curve, m_curves)
			SAFE_DELETE( curve);

		m_curves.clear();
	}

	// 添加链接
	void ShaderTreeConnectMgr::AddConnection( ST::QBezierCurve* curve) 
	{ 
		m_curves.push_back( curve); 
	}

	// 移除无用链接
	void ShaderTreeConnectMgr::RemoveUselessConnection( MaterialShaderTree* shaderTree)
	{
		for( vector<ST::QBezierCurve*>::iterator it=m_curves.begin(); it!=m_curves.end();)
		{
			if( !shaderTree->GetMaterialExpressionConnection( (*it)->m_start->GetID(), (*it)->m_end->GetID()))
			{
				SAFE_DELETE( *it);
				it = m_curves.erase( it);
			}
			else
			{
				it++;
			}
		}
	}

	// 获取链接
	ST::QBezierCurve* ShaderTreeConnectMgr::GetConnection( const string& expression, const string& input)
	{
		for( vector<ST::QBezierCurve*>::iterator it=m_curves.begin(); it!=m_curves.end(); it++)
		{
			if( (*it)->m_start->GetID() == expression && (*it)->m_end->GetID()==input)
				return *it;
		}

		return NULL;
	}
}