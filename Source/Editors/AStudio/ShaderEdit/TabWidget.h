#ifndef SHADER_TAB_WIDGET_H
#define SHADER_TAB_WIDGET_H

#include <QtGui/QTextEdit>
#include <QShaderSyntaxHighLighter.h>
#include <QLuaSyntaxHighLighter.h>
#include <QtGui/QTabWidget>
#include <map>

using namespace std;
using namespace Qt_Ares;

namespace Ares
{
	//-----------------------------------
	// Dx9MdiArea dx显示窗口
	//-----------------------------------
	class CTabWidget : public QTabWidget
	{
		Q_OBJECT

	public:
		struct SEditGroup
		{
			QTextEdit*					m_pText;
			QSyntaxHighlighter*			m_highLighter;// 高亮
		};

		typedef map< string, SEditGroup>  EditList;

	public:
		// 构造函数
		CTabWidget( QWidget* parent = NULL);

		// 析构函数
		~CTabWidget();

		// 打开文件
		void Open( const char* fileName);

		// 保存
		void Save();

	public: signals:
		// Error
		void Error( const char* err);

	private slots:
		// 编译
		bool Compile();

	private:
		EditList     m_editList;		// 编辑列表
	};
}

#endif