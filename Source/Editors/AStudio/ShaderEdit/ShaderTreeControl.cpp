#include "ShaderTreeControl.h"
#include "SHMainWindow.h"
#include <QtGui/QPainter>
#include <QtGui/QColorDialog>
#include <QtGui/QStylePainter>

#define NUM_POINTS 100
#define INVALID_QPOINT	QPoint( -65535, -65535)

namespace ST
{
	// 构造函数
	QExpressionInputOutput::QExpressionInputOutput( bool input, QWidget* parent)
		: QRadioButton( parent)
		, m_input( input)
	{

	}

	// 析构函数
	QExpressionInputOutput::~QExpressionInputOutput()
	{

	}

	// 获取图标中心点
	QPoint QExpressionInputOutput::IconCenter()
	{
		return layoutDirection()==Qt::LeftToRight ? QPoint( 0, 8) : QPoint( width(), 8);
	}

	// 构造函数
	ShaderTreeControl::ShaderTreeControl( QMdiArea* parent)
		: QGroupBox( parent)
		, m_state( OS_None)
		, m_coordinate( NULL)
	{
		setTitle( "");

		// 标题
		m_title = new QLabel( this);
		m_title->setFixedHeight( 18);
		m_title->setStyleSheet("color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(125, 125, 125, 255), stop:0.5 rgba(90, 90, 90, 255), stop:0.5 rgba(90, 90, 90, 255), stop:1 rgba(125, 125, 125, 255));");

		setStyleSheet( " QGroupBox { border: 2px solid rgb( 35, 35, 35); background-color: rgb(50,50,50); };background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(28, 28, 28, 255), stop:1 rgba(48, 48, 48, 255));");
		setFixedWidth( 128);
	}

	// 事件重载
	void ShaderTreeControl::mousePressEvent(QMouseEvent *event)
	{   
		m_mousePos  = event->globalPos();  
		m_windowPos = m_mdiSubWindow->pos();   
		m_dPos	    = m_mousePos - m_windowPos;

		if( m_title->rect().contains( event->pos()))
		{
			m_state	= OS_MoveWindow;
		}
		else
		{
			//m_state = OS_None;
		}
	}

	void ShaderTreeControl::mouseReleaseEvent(QMouseEvent *event)
	{
		if( m_state==OS_DragConnect)
		{
			//获取鼠标点击处的控件
			QWidget* widget = QApplication::widgetAt( QCursor::pos());
			ST::QExpressionInputOutput* radioButton = qobject_cast<ST::QExpressionInputOutput *>(widget);
			if( radioButton && radioButton->IsInput())
			{
				// 链接成功
				SHUI_ShaderTreeDisplay->GetConnectMgr().FinishConnect( radioButton);
			}
			else
			{
				// 链接失败
				SHUI_ShaderTreeDisplay->GetConnectMgr().DeleteEditConnect();
			}
		}

		m_state = OS_None;
	}

	// 事件重载
	void ShaderTreeControl::mouseMoveEvent(QMouseEvent *event)
	{   
		// 只有标题栏能移动窗口
		if( m_state==OS_MoveWindow)
		{
			QPoint pos = event->globalPos() - m_dPos;
			m_mdiSubWindow->move( pos);
			m_coordinate->Set( pos.x(), pos.y());

			// QExpressionInputOutput 移动
			foreach_( QExpressionInputOutput* inputoutput, m_ioputs)
				inputoutput->Signal_OnMoved( inputoutput);
		}
		else if( m_state==OS_DragConnect)
		{
			QPoint mousePos = this->mapTo( SHUI_ShaderTreeDisplay, event->pos());
			SHUI_ShaderTreeDisplay->GetConnectMgr().GetEditConnect()->BindEnd( NULL);
			SHUI_ShaderTreeDisplay->GetConnectMgr().GetEditConnect()->SetEndPos( mousePos);
		}
	}

	// 开始链接输出
	void ShaderTreeControl::StartConnect()
	{
		ST::QExpressionInputOutput* radioButton = qobject_cast<ST::QExpressionInputOutput *>(sender());
		if( radioButton)
		{
			SHUI_ShaderTreeDisplay->GetConnectMgr().CreateConnect();

			SHUI_ShaderTreeDisplay->GetConnectMgr().GetEditConnect()->BindStart( radioButton);
			SHUI_ShaderTreeDisplay->GetConnectMgr().GetEditConnect()->BindEnd( NULL);
			SHUI_ShaderTreeDisplay->GetConnectMgr().GetEditConnect()->SetEndPos( INVALID_QPOINT);

			m_state = OS_DragConnect;
		}
	}

	// 重链接终点
	void ShaderTreeControl::ReConnectEnd()
	{
		ST::QExpressionInputOutput* radioButton = qobject_cast<ST::QExpressionInputOutput *>(sender());
		if( radioButton)
		{
			if( SHUI_ShaderTreeDisplay->GetConnectMgr().EditConnect( radioButton))
				m_state = OS_DragConnect;
		}
	}

	// 构造函数
	MaterialSlot::MaterialSlot( QMdiArea* parent)
		: ShaderTreeControl( parent)
	{
		m_id = "slot";
		m_verticalLayout = new QVBoxLayout( this);
		m_diffuse	 = new QExpressionInputOutput( true, this);
		m_diffuse->setText( "Diffuse");
		m_diffuse->setLayoutDirection(Qt::RightToLeft);
		m_diffuse->setStyleSheet( "color: rgb(200, 200, 200); background-color: rgba(255, 255, 255, 0);");
		m_diffuse->Signal_OnMoved.connect( boost::bind<void>(&ShaderTreeConnectMgr::OnMove, &SHUI_ShaderTreeDisplay->GetConnectMgr(), _1));
		
		m_normal		 = new QExpressionInputOutput( true, this);
		m_normal->setText( "Normal");
		m_normal->setLayoutDirection(Qt::RightToLeft);
		m_normal->setStyleSheet( "color: rgb(200, 200, 200); background-color: rgba(255, 255, 255, 0);");
		m_normal->Signal_OnMoved.connect( boost::bind<void>(&ShaderTreeConnectMgr::OnMove, &SHUI_ShaderTreeDisplay->GetConnectMgr(), _1));

		m_title->setText( " Material Slot");
		m_verticalLayout->addWidget( m_title);
		m_verticalLayout->addWidget( m_diffuse);
		m_verticalLayout->addWidget( m_normal);

		m_verticalLayout->setContentsMargins(0, 0, 0, -1);

		setLayoutDirection(Qt::RightToLeft);

		m_ioputs.push_back( m_normal);
		m_ioputs.push_back( m_diffuse);

		connect( m_diffuse, SIGNAL( released()), this, SLOT( ReConnectEnd()));
		connect( m_normal,  SIGNAL( released()), this, SLOT( ReConnectEnd()));

		m_mdiSubWindow = parent->addSubWindow( this, Qt::FramelessWindowHint);
		m_mdiSubWindow->setVisible( true);
	}

	// 析构函数
	MaterialSlot::~MaterialSlot()
	{
	}

	// 设置ID
	void MaterialSlot::SetID( const string& diffuseID, const string& normalID)
	{
		m_diffuse->SetID( diffuseID);
		m_normal->SetID( normalID);
	}

	// 构造函数
	ConstantColor::ConstantColor( QMdiArea* parent)
		: ShaderTreeControl( parent)
		, m_expression( NULL)
	{
		m_layout = new QVBoxLayout( this);
		m_layoutInternal = new QHBoxLayout( this);
		m_output = new QExpressionInputOutput( false, this);
		m_output->setText( "Color");
		m_output->setStyleSheet( "color: rgb(200, 200, 200); background-color: rgba(255, 255, 255, 0);");
		m_output->Signal_OnMoved.connect( boost::bind<void>( &ShaderTreeConnectMgr::OnMove, &SHUI_ShaderTreeDisplay->GetConnectMgr(), _1));

		m_title->setText( " Color");
		m_layout->addWidget( m_title);

		// 颜色按钮
		m_colorButton = new_ QPushButton( this);
		m_colorButton->setStyleSheet( "border: 2px solid rgb( 35, 35, 35); border-radius : 5px; background-color: rgb(255,50,50);");
		m_colorButton->setFixedHeight( 22);
		m_layoutInternal->addSpacing( 5);
		m_layoutInternal->addWidget( m_colorButton);
		m_layoutInternal->addSpacing( 5);
		m_layoutInternal->setContentsMargins( -1, -1, -1, -1);
		m_layout->addLayout( m_layoutInternal);

		m_layout->addWidget( m_output);
		m_layout->setContentsMargins(0, 0, 0, -1);

		// 消息
		connect( m_colorButton, SIGNAL(clicked()),  this, SLOT(OnSelectColor()));
		connect( m_output,		SIGNAL( pressed()), this, SLOT( StartConnect()));

		m_ioputs.push_back( m_output);

		m_mdiSubWindow = parent->addSubWindow( this, Qt::FramelessWindowHint);
		m_mdiSubWindow->setVisible( true);
	}

	// 析构孙
	ConstantColor::~ConstantColor()
	{

	}

	// 设置ID
	void ConstantColor::SetID( const string& oID) 
	{ 
		m_id = oID;
		m_output->SetID( oID);
	}

	// 绑定表达式
	void ConstantColor::BindExpression( MaterialExpressionConstantColor* expression) 
	{ 
		m_expression = expression; 
		
		A_ASSERT( m_expression);

		Vector4 color = m_expression->m_output.m_value;

		m_color = QColor( color.r * 255.f, color.g * 255.f, color.b*255.f, color.a*255.f);
		char result[512];
		sprintf_s( result, 512, "border: 2px solid rgb( 35, 35, 35); border-radius : 5px; background-color: rgb(%d,%d,%d);", m_color.red(), m_color.green(), m_color.blue());

		m_colorButton->setStyleSheet( result);
	}

	// 选择路径
	void ConstantColor::OnSelectColor()
	{
		char   result[512];
		m_color = QColorDialog::getColor( m_color, NULL);	
		sprintf_s( result, 512, "border: 2px solid rgb( 35, 35, 35); border-radius : 5px; background-color: rgb(%d,%d,%d);", m_color.red(), m_color.green(), m_color.blue());

		m_colorButton->setStyleSheet( result);

		if( m_expression)
			m_expression->SetValue( ColorRGB( m_color.red()/255.f, m_color.green()/255.f, m_color.blue()/255.f, m_color.alpha()/255.f));
	}

	// 构造函数
	QBezierCurve::QBezierCurve()
		: m_start( NULL)
		, m_end( NULL)
		, m_startPos( INVALID_QPOINT)
		, m_endPos( INVALID_QPOINT)
	{
		m_path.resize( NUM_POINTS);
	}

	// 析构函数
	QBezierCurve::~QBezierCurve()
	{

	}

	// 设置开始点
	void QBezierCurve::SetStartPos( const QPoint& start)
	{
		if( m_startPos!=start)
		{
			m_startPos = start;
			UpdateDisplay();
		}
	}

	// 设置结束点
	void QBezierCurve::SetEndPos( const QPoint& end)
	{
		if( m_endPos!=end)
		{
			m_endPos = end;
			UpdateDisplay();
		}
	}

	// 绑定
	void QBezierCurve::BindStart( QExpressionInputOutput* start) 
	{ 
		m_start = start;
		if( m_start)
		{
			OnMove();
		}
	}

	// 绑定
	void QBezierCurve::BindEnd( QExpressionInputOutput* end) 
	{ 
		m_end = end;
		if( m_end)
		{
			OnMove();
		}
	}

	// 当输入输出移动
	void QBezierCurve::OnMove()
	{
		if( m_start)
		{
			QPoint pos = m_start->mapTo( SHUI_ShaderTreeDisplay, m_start->IconCenter());
			SetStartPos( pos);
		}

		if( m_end)
		{
			QPoint pos = m_end->mapTo( SHUI_ShaderTreeDisplay, m_end->IconCenter());
			SetEndPos( pos);
		}
	}

	// 更新数据
	void QBezierCurve::UpdateDisplay()
	{
		float timeStep = 1.f / NUM_POINTS;
		Vector3 startPos( m_startPos.x(), m_startPos.y(), 0.f);
		Vector3 endPos( m_endPos.x(), m_endPos.y(), 0.f);
		Vector3 centerPos0 = startPos + ( endPos - startPos) * 0.4f; centerPos0.y = startPos.y; 
		Vector3 centerPos1 = startPos + ( endPos - startPos) * 0.6f; centerPos1.y = endPos.y;
		Vector3 result;
		for( int i=0; i<NUM_POINTS; i++)
		{
			Curves::Bezier3( result, startPos, centerPos0, centerPos1, endPos, (float)i*timeStep);

			m_path[i] = QPointF( result.x, result.y);
		}

		SHUI_ShaderTreeDisplay->viewport()->repaint();
	}

	// 渲染
	void QBezierCurve::paint( QWidget* parent)
	{
		if( !m_start && m_startPos==INVALID_QPOINT) return;
		if( !m_end   && m_endPos  ==INVALID_QPOINT) return;
		
		QPainter painter( parent);
		painter.setRenderHint( QPainter::Antialiasing);
		painter.setPen( QPen( QColor( 200, 200, 140, 255), 2));
		for( size_t i=0; i<m_path.size()-1; i++)
			painter.drawLine( m_path[i], m_path[i+1]);
	}
}
