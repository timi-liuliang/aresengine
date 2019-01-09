#include <QtGui/QtGui>
#include "SHMainWindow.h"
#include "TabWidget.h"
#include "SHLogic.h"
#include <Engine/RenderSystem/SuperMaterialShaderTree.h>
#include "UIShaderTreeDisplay.h"

namespace Ares
{
	// 构造函数
	UIShaderTreeDisplay::UIShaderTreeDisplay( QWidget* parent/* = NULL*/)
		: QMdiArea( parent)
		//, m_pTimer( NULL)
		, m_materialSlot( NULL)
	{
		m_pDx9Widget = new QWidget( this);
		//addSubWindow( m_pDx9Widget);

		// 显示区域
		m_pTabWidget = new CTabWidget( this);

		//m_pTabWidget->showMaximized();

		this->setBackground( QBrush( QColor( 50, 50, 50)));

		viewport()->setAcceptDrops( true);
		viewport()->setMouseTracking( true);

		// 默认材质槽
		m_materialSlot = new_ ST::MaterialSlot( this);
		m_materialSlot->GetMdiSubWindow()->move( QPoint( 100, 250));
	}

	// 析构函数
	UIShaderTreeDisplay::~UIShaderTreeDisplay()
	{
		//SAFE_DELETE( m_pTimer);		
	}

	//// 开始渲染
	//void UIShaderTreeDisplay::BeginRender()
	//{
	//	m_dx9Size = this->size();
	//	//m_pTabWidget->show();
	//	m_pDx9Widget->setWindowTitle( tr("Dx9 Show"));

	//	// 扩展策略
	//	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	//	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	//	// 时间事件
	//	m_pTimer = new QTimer( this);
	//	connect( m_pTimer, SIGNAL( timeout()), this, SLOT( Render()));
	//	m_pTimer->start( 15);
	//}

	// 渲染更新
	void  UIShaderTreeDisplay::Render()
	{
	}

	// 更新shadertree显示
	void UIShaderTreeDisplay::UpdateShaderTreeDisplay()
	{
		MaterialShaderTree* shaderTree = SHLogic_ShaderTreeEdit->GetShaderTree();
		if( shaderTree)
		{
			// 1.Material slot
			MaterialExpressionSlot& materialSlot   = shaderTree->GetExpressionSlot();
			Vector2 slotPos = materialSlot.GetUICoordinate().m_pos;
			m_materialSlot->GetMdiSubWindow()->move( slotPos.x, slotPos.y);
			m_materialSlot->SetID( materialSlot.m_diffuseColor.m_id, materialSlot.m_normal.m_id);
			m_materialSlot->BindUICoordinate( &materialSlot.GetUICoordinate());
	
			// 2.Expression
			RemoveUselessQExpression( shaderTree);

			const vector<MaterialExpression*>& expressions = shaderTree->GetMaterialExpression();
			for( size_t i=0; i<expressions.size(); i++)
			{
				A_ASSERT( expressions[i]);
			
				if( !GetExpressionControl( expressions[i]->GetID()))
				{
					switch( expressions[i]->GetType())
					{
					case ET_ConstantColor:
						{
							ST::ConstantColor* constantColor = new_ ST::ConstantColor( this);
							Vector2 slotPos = expressions[i]->GetUICoordinate().m_pos;
							constantColor->GetMdiSubWindow()->move( slotPos.x, slotPos.y);
							constantColor->BindUICoordinate( &expressions[i]->GetUICoordinate());
							constantColor->SetID( expressions[i]->GetID());
							constantColor->BindExpression( dynamic_cast<MaterialExpressionConstantColor*>(expressions[i]));
							m_shaderTreeControls.push_back( constantColor);
						}
						break;
					}
				}
			}

			// 3.Connection
			m_shaderTreeConnectMgr.RemoveUselessConnection( shaderTree);
			const vector<MaterialExpressionConnection*>& connections = shaderTree->GetMaterialExpressionConnection();
			for( size_t i=0; i<connections.size(); i++)
			{
				A_ASSERT( connections[i]);

				if( !m_shaderTreeConnectMgr.GetConnection( connections[i]->m_expression, connections[i]->m_input))
				{
					ST::QBezierCurve* curve = new_ ST::QBezierCurve();

					QExpressionInputOutput* start = GetInputOutput( connections[i]->m_expression);
					QExpressionInputOutput* end	  = GetInputOutput( connections[i]->m_input);

					A_ASSERT( start);
					A_ASSERT( end);

					curve->BindStart( start);
					curve->BindEnd( end);

					m_shaderTreeConnectMgr.AddConnection( curve);
				}
			}
		}

		viewport()->repaint();
	}

	// 移除表达式
	void UIShaderTreeDisplay::RemoveUselessQExpression( MaterialShaderTree* shaderTree)
	{
		for( vector<ST::ShaderTreeControl*>::iterator it=m_shaderTreeControls.begin(); it!=m_shaderTreeControls.end(); )
		{
			if( !shaderTree->GetMaterialExpression( (*it)->GetID()))
			{
				this->removeSubWindow( (*it)->GetMdiSubWindow());
				SAFE_DELETE( (*it));
				it = m_shaderTreeControls.erase( it);
			}
			else
			{
				it++;
			}
		}
	}

	// 获取InputOutput
	QExpressionInputOutput* UIShaderTreeDisplay::GetInputOutput( const string& id)
	{
		// 材质槽
		foreach_( QExpressionInputOutput* inputoutput, m_materialSlot->GetInputOutputs())
		{
			if( inputoutput->GetID() == id)
				return inputoutput;
		}

		// 表达式
		foreach_( ST::ShaderTreeControl* control, m_shaderTreeControls)
		{
			foreach_( QExpressionInputOutput* inputoutput, control->GetInputOutputs())
			{
				if( inputoutput->GetID() == id)
					return inputoutput;
			}
		}

		return NULL;
	}

	// 获取表达式控件
	ShaderTreeControl* UIShaderTreeDisplay::GetExpressionControl( const string& id)
	{
		foreach_( ST::ShaderTreeControl* control, m_shaderTreeControls)
		{
			if( control->GetID() == id)
				return control;
		}

		return NULL;
	}

	// 重置设备
	void  UIShaderTreeDisplay::ResetDevice()
	{

	}

	// 渲染重载
	void UIShaderTreeDisplay::paintEvent(QPaintEvent* event)
	{
		QMdiArea::paintEvent( event);

		// 绘制曲线
		foreach_( ST::QBezierCurve* curve, m_shaderTreeConnectMgr.m_curves)
			curve->paint( viewport());

		if( m_shaderTreeConnectMgr.m_curveEdit)
			m_shaderTreeConnectMgr.m_curveEdit->paint( viewport());
	}

	// 拖拽进入事件
	void UIShaderTreeDisplay::dragEnterEvent( QDragEnterEvent* event)
	{
		if( event->mimeData()->hasFormat( "text/plain"))
			event->acceptProposedAction();
	}

	// 拖拽释放事件
	void UIShaderTreeDisplay::dropEvent( QDropEvent * event)
	{
		if( event->mimeData()->hasFormat("text/plain"))
		{
			Vector2 pos = Vector2( static_cast<float>( event->pos().x()), static_cast<float>( event->pos().y()));

			const QMimeData* mime = event->mimeData();
			QString text = mime->text();
			if( text == "ConstantColor")
			{
				MaterialShaderTree* shaderTree = SHLogic_ShaderTreeEdit->GetShaderTree();
				if( shaderTree)
				{
					pos = pos - Vector2( 64, 8);
					SHLogic_ShaderTreeEdit->AddExpression( ET_ConstantColor, pos);
				}
			}

			if( event->source()==this)
			{
				event->setDropAction( Qt::MoveAction);
				event->accept();
			}
			else
			{
				event->acceptProposedAction();
			}
		}
		else
		{
			event->ignore();
		}

		UpdateShaderTreeDisplay();
	}

	// 重置,清空所有界面
	void UIShaderTreeDisplay::Reset()
	{
		while( !m_shaderTreeControls.empty())
		{
			ST::ShaderTreeControl* control = m_shaderTreeControls.back();
			removeSubWindow( control->GetMdiSubWindow());
			SAFE_DELETE( control);

			m_shaderTreeControls.pop_back();
		}

		m_shaderTreeConnectMgr.Reset();
	}
}