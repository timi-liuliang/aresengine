#ifndef DX9_MDI_AREA_H
#define DX9_MDI_AREA_H

#include "ShaderTreeControl.h"
#include "ShaderTreeConnectMgr.h"
#include <Engine/RenderSystem/SuperMaterialShaderTree.h>
#include <QtGui/QMdiArea>
#include "ui_ShaderTreeDisplay.h"

class Application;
namespace Ares
{
	//-----------------------------------
	// Dx9MdiArea dx显示窗口
	//-----------------------------------
	class CTabWidget;
	class UIShaderTreeDisplay : public QMdiArea, public Ui_ShaderTreeDisplay
	{
		Q_OBJECT

	public:
		UIShaderTreeDisplay( QWidget* parent = NULL);
		~UIShaderTreeDisplay();

		//// 开始渲染
		//void BeginRender();

		// 更新shadertree显示
		void UpdateShaderTreeDisplay();

		// 获取InputOutput
		QExpressionInputOutput* GetInputOutput( const string& id);

		// 获取表达式控件
		ShaderTreeControl* GetExpressionControl( const string& id);

		// 获取链接管理器
		ShaderTreeConnectMgr& GetConnectMgr() { return m_shaderTreeConnectMgr; }

		// 重置,清空所有界面
		void Reset();

	private:
		// 移除表达式
		void RemoveUselessQExpression( MaterialShaderTree* shaderTree);

	protected:
		// 绘制事件
		void paintEvent(QPaintEvent *);

		// 拖拽进入事件
		void dragEnterEvent( QDragEnterEvent* event);
		
		// 拖拽释放事件
		void dropEvent( QDropEvent * event);

	private slots:

		// 渲染
		void  Render();

		// 重置设备
		void  ResetDevice();

	private:
		QSize					m_dx9Size;			// 调整后大小
		//QTimer*					m_pTimer;			// 计时器

	public:
		QWidget*				m_pDx9Widget;		// dx9显示
		CTabWidget*				m_pTabWidget;		// shader显示

		// 1.Material slot
		ST::MaterialSlot*					m_materialSlot;
		vector<ST::ShaderTreeControl*>		m_shaderTreeControls;		// 表达式界面
		ShaderTreeConnectMgr				m_shaderTreeConnectMgr;		// 链接管理器
	};
}

#endif