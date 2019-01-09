#include <QtGui/QtGui>
#include "MOModelConverter.h"
#include "MOYNDMeshToAres.h"
#include <Core/AresFileUtils.h>

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	MOModelConverter::MOModelConverter( QWidget* parent/* = 0*/)
		: QDialog( parent)
	{
		setupUi( this);

		// 关联消息
		connect( m_browserYND,			SIGNAL( clicked()), this, SLOT( SelectYNDLocation()));
		connect( m_browserAres,			SIGNAL( clicked()), this, SLOT( SelectAresLocation()));
		connect( m_yndToAresConverter,	SIGNAL( clicked()), this, SLOT( ConvertexFromYNDToAres()));
	}

	// 选择YND资源路径
	void MOModelConverter::SelectYNDLocation()
	{
		QString qFileDir = QFileDialog::getExistingDirectory( this, tr("Choose YND directory"), "", QFileDialog::ShowDirsOnly);

		QDir dir( QDir::currentPath());
		qFileDir = dir.relativeFilePath( qFileDir);

		if( !qFileDir.isEmpty())
		{
			if( qFileDir.length()>0 && qFileDir[qFileDir.length()-1]!='/')
				qFileDir += "/";

			m_yndsLocation->setText( qFileDir);
		}
	}

	// 选择Ares存储路径
	void MOModelConverter::SelectAresLocation()
	{
		QString qFileDir = QFileDialog::getExistingDirectory( this, tr("Choose Ares directory"), "", QFileDialog::ShowDirsOnly);

		QDir dir( QDir::currentPath());
		qFileDir = dir.relativeFilePath( qFileDir);

		if( !qFileDir.isEmpty())
		{
			if( qFileDir.length()>0 && qFileDir[qFileDir.length()-1]!='/')
				qFileDir += "/";

			m_aresLocation->setText( qFileDir);
		}
	}

	// 转换
	void MOModelConverter::ConvertexFromYNDToAres()
	{
		string yndRootLocation = m_yndsLocation->text().toStdString();
		string aresSaveLocation= m_aresLocation->text().toStdString();
		bool isReplaceExistAres= m_isReplaceExistFile->isChecked();

		// 递归查找所有文件
		FileUtils fileUtils;
		fileUtils.AddSearchPathRecursive( yndRootLocation.c_str());

		// 确定所有要转换的模型
		vector<string> yndFiles;
		fileUtils.SearchFilesByExt( yndFiles, yndRootLocation.c_str(), ".GMe");

		// 执行转换
		for( size_t i=0; i<yndFiles.size(); i++)
		{
			ModelConverter::YNDToAres( yndFiles[i].c_str(), aresSaveLocation.c_str(), fileUtils, isReplaceExistAres);
		}

	}
}