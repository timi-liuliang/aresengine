#include "QPropertyDelegate.h"
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include "QFileSelect.h"
#include "QColorSelect.h"
#include "QCheckBoxList.h"
#include <QtGui/QCheckBox>

namespace QT_UI
{
	// 构造函数
	QPropertyDelegate::QPropertyDelegate( QPropertyModel* model, QObject *parent/* = 0*/)
		: QItemDelegate( parent)
	{
		m_model = model;
	}

	// 是否拥有自定义渲染
	bool QPropertyDelegate::IsSupportCustomPaint( const QString& widgetType) const
	{
		if( widgetType=="ColorSelect")
			return true;

		return false;
	}

	// 自定义渲染
	void QPropertyDelegate::ItemDelegatePaint( QPainter *painter, const QString& widgetType, const QRect& rect, const QVariant& val) const
	{
		if( widgetType == "ColorSelect")
			QColorSelect::ItemDelegatePaint( painter, rect, val.toString().toStdString());
	}

	// 绘制函数
	void QPropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		QVariant userData = index.model()->data( index, Qt::UserRole);
		QString  widgetType = userData.toString();

		QVariant value;
		if( m_model->findValue( value, propertyName))
		{
			if( IsSupportCustomPaint( widgetType))
			{
				ItemDelegatePaint( painter, widgetType, option.rect, value);
			}
			else
			{
				QString text = value.toString();

				QStyleOptionViewItem myOption = option;
				myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

				drawDisplay(painter, myOption, myOption.rect, text);
				drawFocus(painter, myOption, myOption.rect);
			}
		}
		else
		{
			QItemDelegate::paint( painter, option, index);
		}
	}

	// createEditor, returns an editor widget
	QWidget* QPropertyDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		QVariant userData = index.model()->data( index, Qt::UserRole);
		QString  widgetType = userData.toString();

		if( widgetType == "spinBox")
			return new_ QSpinBox( parent);

		else if( widgetType == "doubleSpinBox")
			return new_ QDoubleSpinBox( parent);

		else if( widgetType == "LineEdit")
			return new_ QLineEdit( parent);

		else if( widgetType == "FileSelect")
			return new_ QFileSelect( parent);

		else if( widgetType == "ColorSelect")
			return new_ QColorSelect( parent);

		else if( widgetType == "RadioButton")
			return new_ QRadioButton( parent);

		else if( widgetType == "CheckBox")
			return new_ QCheckBox(parent);

		else if( widgetType == "CheckBoxList")
			return new_ QCheckBoxList( parent);

		else if( widgetType == "default")
			return QItemDelegate::createEditor( parent, option, index);

		else
			return NULL;
	}

	// 更新编辑器数据
	void  QPropertyDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
	{
		QVariant userData = index.model()->data( index, Qt::UserRole);
		QString  widgetType = userData.toString();

		QVariant value;
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		if( m_model->findValue( value, propertyName))
		{
			if( widgetType == "spinBox")
			{
				QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
				widget->setValue( value.toInt());
			}
			else if( widgetType == "doubleSpinBox")
			{
				QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
				widget->setValue( value.toFloat());
			}
			else if( widgetType == "LineEdit")
			{
				QLineEdit* widget = qobject_cast<QLineEdit*>(editor);
				widget->setText( value.toString());
			}
			else if( widgetType == "FileSelect")
			{
				QFileSelect* widget = qobject_cast<QFileSelect*>(editor);
				widget->SetPath( value.toString());
			}
			else if( widgetType == "ColorSelect")
			{
				QColorSelect* widget = qobject_cast<QColorSelect*>( editor);
				widget->SetColor( value.toString().toStdString());
			}
			else if( widgetType == "CheckBox")
			{
				QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
				widget->setChecked( value.toBool());
			}
			else if( widgetType == "CheckBoxList")
			{
				QCheckBoxList* widget = qobject_cast<QCheckBoxList*>(editor);
				widget->SetValue( value.toString());
			}
			else if( widgetType == "default")
			{
				QItemDelegate::setEditorData(editor, index);
			}
		}
	}

	void  QPropertyDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
	{
		QVariant userData = index.model()->data( index, Qt::UserRole);
		QString  widgetType = userData.toString();

		QVariant value;
		QString propertyName = index.model()->data( index, Qt::DisplayPropertyRole).toString();
		if( m_model->findValue( value, propertyName))
		{
			if( widgetType == "spinBox")
			{
				QSpinBox* widget = qobject_cast<QSpinBox*>(editor);
				this->m_model->setValue( propertyName, widget->value());
			}
			else if( widgetType == "doubleSpinBox")
			{
				QDoubleSpinBox* widget = qobject_cast<QDoubleSpinBox*>(editor);
				m_model->setValue( propertyName, widget->value());
			}
			else if( widgetType == "LineEdit")
			{
				QLineEdit* widget = qobject_cast<QLineEdit*>(editor);
				m_model->setValue( propertyName, widget->text());
			}
			else if( widgetType == "FileSelect")
			{
				QFileSelect* widget = qobject_cast<QFileSelect*>(editor);
				m_model->setValue( propertyName, widget->GetPath());
			}
			else if( widgetType == "ColorSelect")
			{
				QColorSelect* widget = qobject_cast<QColorSelect*>(editor);
				m_model->setValue( propertyName, widget->GetColor().c_str());
			}
			else if( widgetType == "CheckBox")
			{
				QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
				m_model->setValue( propertyName, widget->isChecked());
			}
			else if( widgetType=="CheckBoxList")
			{
				QCheckBoxList* widget = qobject_cast<QCheckBoxList*>(editor);
				m_model->setValue( propertyName, widget->GetValue());
			}
			else if( widgetType == "default")
			{
				QItemDelegate::setModelData(editor, model, index);
			}
		}
	}

	// 设置编辑器Geometry
	void QPropertyDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index ) const
	{
		editor->setGeometry(option.rect);
		editor->setFocus();

		//editor->setGeometry( QRect( 10, 10, 500, 500));
	}
}