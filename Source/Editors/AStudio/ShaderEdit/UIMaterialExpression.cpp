#include "UIMaterialExpression.h"

namespace Ares
{
	// 构造函数
	UIMaterialExpression::UIMaterialExpression( QWidget* parent)
		: QDockWidget( parent)
	{
		setupUi( this);

		QTreeWidgetItem* constantItem = new QTreeWidgetItem(); constantItem->setText( 0, "Constant"); 
		QTreeWidgetItem* arithmeticFun= new QTreeWidgetItem(); arithmeticFun->setText( 0, "Arithmetic");

		QFont tFont = constantItem->font( 0); tFont.setBold( true); tFont.setUnderline( true);
		constantItem->setFont( 0, tFont);
		arithmeticFun->setFont( 0, tFont);

		QList<QTreeWidgetItem*> rootItems;
		rootItems << constantItem << arithmeticFun;

		m_treeWidget->addTopLevelItems( rootItems);

		// Constants
		QTreeWidgetItem* constantColor = new QTreeWidgetItem(); constantColor->setText( 0, "Color\nProvides a constant value"); constantColor->setData( 0, Qt::UserRole, "ConstantColor");

		constantItem->addChild( constantColor);

		// Add
		QTreeWidgetItem* arithmeticAdd		= new QTreeWidgetItem; arithmeticAdd->setText( 0, "Add\nAdd two inputs");				
		QTreeWidgetItem* arithmeticSub		= new QTreeWidgetItem; arithmeticSub->setText( 0, "Subtract\nSubstract two inputs");
		QTreeWidgetItem* arithmeticMul		= new QTreeWidgetItem; arithmeticMul->setText( 0, "Multiply\nMultiply two inputs");
		QTreeWidgetItem* arithmeticDivide	= new QTreeWidgetItem; arithmeticDivide->setText( 0, "Divide\nDivide two inputs"); 

		arithmeticFun->addChild( arithmeticAdd);
		arithmeticFun->addChild( arithmeticSub);
		arithmeticFun->addChild( arithmeticMul);
		arithmeticFun->addChild( arithmeticDivide);

		m_treeWidget->expandAll();

		connect( m_treeWidget, SIGNAL(pressed(QModelIndex)), this, SLOT(OnSelectExpression()));
	}

	// 开启拖拽事件
	void UIMaterialExpression::OnSelectExpression()
	{
		QTreeWidgetItem* item = m_treeWidget->currentItem();
		if( item)
		{
			QDrag*    drag	    = new QDrag( this);
			QMimeData* mimeData = new QMimeData;
			mimeData->setText( item->data( 0, Qt::UserRole).toString());
			drag->setMimeData( mimeData);

			Qt::DropAction action = drag->exec( Qt::MoveAction, Qt::MoveAction);
			if( action)
			{
				int a = 10;
			}

		}
	}
}