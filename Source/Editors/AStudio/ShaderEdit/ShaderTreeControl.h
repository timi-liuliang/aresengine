#pragma once

#include <vector>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QMouseEvent>
#include <QtGui/QMdiArea>
#include <QtGui/QLabel>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QPushButton>
#include <Physics/Bezier3.h>
#include <boost/signals2/signal.hpp>
#include <Engine/RenderSystem/SuperMaterialExpression.h>

using namespace std;
using namespace Ares;
using namespace boost::signals2;

namespace Ares
{
	class UIShaderTreeDisplay;
}

namespace ST
{
	enum OperationState
	{
		OS_MoveWindow,		// 移动窗口
		OS_DragConnect,		// 链接表达式
		OS_None,
	};

	//------------------------------
	// 表达式输入输出 2013-3-18 帝林
	//------------------------------
	class QBezierCurve;
	class QExpressionInputOutput : public QRadioButton
	{
		Q_OBJECT

	public:
		QExpressionInputOutput( bool input, QWidget* parent=NULL);
		~QExpressionInputOutput();

		// 获取图标中心点
		QPoint IconCenter();

		// 是否为表达式输入项
		bool IsInput() { return m_input; }

		// 获取ID
		const string& GetID() { return m_id; }

		// 设置ID
		void SetID( const string& id) { m_id = id; }

		// 位置移动
		signal<void(QExpressionInputOutput*)> Signal_OnMoved;

	private:
		string				m_id;
		bool				m_input;
	};

	//------------------------------
	// STControl 2013-3-17 帝林
	//------------------------------
	class ShaderTreeControl : public QGroupBox
	{
		Q_OBJECT

	public:
		ShaderTreeControl( QMdiArea* parent=NULL);
		~ShaderTreeControl() {}

		// 获取MdiSubWindow
		QMdiSubWindow* GetMdiSubWindow() { return m_mdiSubWindow; }

		// 绑定表达式
		void BindUICoordinate( UICoordinate* coordinate) { m_coordinate = coordinate; }

		// 获取输入输出
		vector<QExpressionInputOutput*>& GetInputOutputs() { return m_ioputs; }

		// 获取ID
		const string& GetID() { return m_id; }

	protected:
		// 鼠标事件
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	private slots:
		// 开始链接输出
		void StartConnect();

		// 重链接终点
		void ReConnectEnd();

	protected:
		string				m_id;				// 标识
		OperationState		m_state;			// 状态
		QMdiSubWindow*		m_mdiSubWindow;
		QLabel*				m_title;			// 标题
		QPoint				m_windowPos;
		QPoint				m_mousePos;
		QPoint				m_dPos;

		UICoordinate*		m_coordinate;
		vector<QExpressionInputOutput*> m_ioputs;
	};

	//------------------------------
	// MaterialSlot 2013-3-12 帝林
	//------------------------------
	class MaterialSlot : public ShaderTreeControl
	{
	public:
		MaterialSlot( QMdiArea* parent=NULL);
		~MaterialSlot();

		// 设置ID
		void SetID( const string& diffuseID, const string& normalID);

	protected:
		QVBoxLayout*				m_verticalLayout;
		QExpressionInputOutput*		m_diffuse;
		QExpressionInputOutput*		m_normal;
	};

	//--------------------------------
	// ConstantColor  2013-3-12 帝林
	//--------------------------------
	class ConstantColor : public ShaderTreeControl
	{
		Q_OBJECT

	public:
		ConstantColor( QMdiArea* parent=NULL);
		~ConstantColor();

		// 设置ID
		void SetID( const string& oID);

		// 绑定表达式
		void BindExpression( MaterialExpressionConstantColor* expression);

	private slots:
		// 选择路径
		void OnSelectColor();

	protected:
		QVBoxLayout*				m_layout;
		QHBoxLayout*				m_layoutInternal;
		QExpressionInputOutput*		m_output;
		QPushButton*				m_colorButton;

		QColor						m_color;
		
		MaterialExpressionConstantColor* m_expression;
	};

	//--------------------------------
	// QBezierCurve 2013-3-17 帝林
	//--------------------------------
	class QBezierCurve
	{
	public:
		QBezierCurve();
		~QBezierCurve();
		
		// 设置开始点
		void SetStartPos( const QPoint& start);

		// 设置结束点
		void SetEndPos( const QPoint& end);

		// 绑定
		void BindStart( QExpressionInputOutput* start);

		// 绑定
		void BindEnd( QExpressionInputOutput* end);

		// 渲染
		void paint( QWidget* parent);

		// 当输入输出移动
		void OnMove();

	private:
		// 更新数据
		void UpdateDisplay();

	public:
		QPoint					m_startPos;
		QPoint					m_endPos;
		QExpressionInputOutput*	m_start;
		QExpressionInputOutput*	m_end;
		vector<QPointF>			m_path;			// 路径
	};
}