#include <QtGui/QtGui>
#include "TabWidget.h"
#include "SHLogic.h"
#include <Engine/AresRoot.h>
#include "SHMainWindow.h"

namespace Ares
{
	// 构造函数
	CTabWidget::CTabWidget( QWidget* parent/* = NULL*/)
		: QTabWidget( parent)
	{
		//open("Data\\shader\\bloom.fx");
	}

	// 析构函数
	CTabWidget::~CTabWidget()
	{
		Save();
	}

	// 打开文件
	void CTabWidget::Open( const char* fileName)
	{
		if( fileName)
		{
			// 判断文件类型
			char ext[256];
			_splitpath( fileName, NULL, NULL, NULL, ext);

			EditList::iterator it = m_editList.find( fileName);

			if( it == m_editList.end())
			{
				QFile file( fileName);
				if( file.open( QFile::ReadOnly | QFile::Text))
				{
					SEditGroup  editGroup;

					editGroup.m_pText = new QTextEdit(this);
					editGroup.m_pText->setTabStopWidth( 24);
					editGroup.m_pText->setPlainText( file.readAll());

					if( strcmp( ext, ".fx") == 0)
						editGroup.m_highLighter = new QShaderSyntaxHighLighter(editGroup.m_pText->document());
					else
						editGroup.m_highLighter = new QLuaSyntaxHighLighter(editGroup.m_pText->document());

					m_editList[fileName] = editGroup;

					string nameAndExt = FileUtils::GetNameExt( fileName);

					addTab( editGroup.m_pText, nameAndExt.c_str());
				}
			}
			else
			{
				// 显示最前
				setCurrentIndex( indexOf( it->second.m_pText));
			}
		}
	}

	// 保存
	void CTabWidget::Save()
	{
		for ( EditList::iterator it = m_editList.begin(); it != m_editList.end(); it++)
		{
			FILE* fileHandle = fopen( it->first.c_str(), "wt");
			if( fileHandle)
			{
				string plainText = it->second.m_pText->toPlainText().toStdString();
				fwrite( plainText.c_str(), plainText.size(), 1, fileHandle);

				fflush(fileHandle);
				fclose(fileHandle);
			}
		}
	}

	// 编译
	bool CTabWidget::Compile()
	{
		// 1.先保存
		Save();

		// 2.清空输出框显示
		//UI_ErrorOutput->Clear();

		// 3.查出当前页shader
		for ( EditList::iterator it = m_editList.begin(); it != m_editList.end(); it++)
		{
			if( it->second.m_pText == currentWidget())
			{
				RenderMethodPtr method = ARenderDevice->CreateRenderMethod();
				method->Load( it->first.c_str(), ShaderMacros());

				//method->SaveByteCode( "D:\\test.vs", ST_VertexShader);
				//method->SaveByteCode( "D:\\test.ps", ST_PixelShader);
			}
		}



		//QWidget* pWidget = currentWidget();
		//if( pWidget)
		//{
		//	QTextEdit* pTextEdit = dynamic_cast<QTextEdit*>(pWidget);
		//	string plainText = pTextEdit->toPlainText().toStdString();

		//	
		//}

		return false;
	}
}