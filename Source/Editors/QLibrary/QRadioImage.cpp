#include <QtGui/QtGui>
#include "QRadioImage.h"
#include <string>

using namespace std;

namespace QT_UI
{
	// 构造函数
	QRadioImage::QRadioImage( QWidget* parent/* = 0*/)
		: QRadioButton( parent)
	{
	}


	// 重载
	void  QRadioImage::paintEvent(QPaintEvent * e)
	{
		QPainter painter( this);
		painter.drawPixmap( 0, 0, width(), height(), m_image);

		QPen pen;
		pen.setWidth( 1);

		if( this->isChecked())	
			pen.setColor( QColor( 255.f, 50.f, 0.f));
		else
			pen.setColor( QColor( 0.f, 0.f, 0.f));

		painter.setPen( pen);
		painter.drawRect( 0, 0, width()-1, height()-1);

		painter.end();

		//QRadioButton::paintEvent( e);
	}

	// 鼠标双击事件
	void  QRadioImage::mouseDoubleClickEvent ( QMouseEvent * event )
	{
		QString fileName = QFileDialog::getOpenFileName( this, tr("Change image"), "", tr("*.dds \n *.jpg \n *.png \n *.tga"));

		// 取得相对路径
		QDir dir( QDir::currentPath());
		fileName = dir.relativeFilePath( fileName);

		if( !fileName.isEmpty())
		{
			SetImage( fileName.toStdString().c_str());
		}
	}

	// 设置显示图片
	bool  QRadioImage::SetImage( const char* resPath)
	{
		bool succed = false;

		m_resPath = resPath;

		if( !m_image.load( resPath))
		{
			char name[128];
			_splitpath( resPath, NULL, NULL, name, NULL);
			char pDest[1024];
			sprintf( pDest, "%s.png", name);

			//Logic_Main->CopyTexture( resPath, pDest, D3DXIFF_PNG);

			succed |= m_image.load( pDest);

			remove( pDest);
		}

		emit imageChanged( resPath);

		return succed;
	}
}