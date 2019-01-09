#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;
namespace Qt_Ares
{
	//-------------------------------
	// shader高度编辑 2011-04-06
	//-------------------------------
	class QLuaSyntaxHighLighter : public QSyntaxHighlighter
	{
		Q_OBJECT

	public:
		// 构造函数
		QLuaSyntaxHighLighter( QTextDocument* parent = 0);

	protected:
		// 高度block
		void  highlightBlock( const QString& text);

	private:
		// 高度规则
		struct HighlightingRule
		{
			QRegExp			pattern;
			QTextCharFormat format;
		};

		QRegExp commentStartExpression;
		QRegExp commentEndExpression;
		QTextCharFormat classFormat;
		QTextCharFormat singleLineCommentFormat;
		QTextCharFormat multiLineCommentFormat;
		QTextCharFormat quotationFormat;
		QTextCharFormat functionFormat;
		QTextCharFormat globalFormat;						// 全局变量

		QVector<HighlightingRule>	m_highLightRules;
		QTextCharFormat				m_keyWordFormat;		// 关键字格式
	};
}