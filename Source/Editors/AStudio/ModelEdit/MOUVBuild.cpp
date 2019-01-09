#include <QtGui/QtGui>
#include "MOUVBuild.h"
#include "MOLogic.h"
#include <boost/lexical_cast.hpp>

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	UVBuild::UVBuild( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		connect( m_generate, SIGNAL(clicked()), this, SLOT(Build()));
		connect( m_restore,  SIGNAL(clicked()), this, SLOT(Restore()));
	}

	// 构建唯一UV
	void UVBuild::Build()
	{
		BuildSettings config;
		config.m_width		= lexical_cast<int>(m_texWidth->text().toStdString());
		config.m_height		= lexical_cast<int>(m_texHeight->text().toStdString());
		config.m_gutter		= m_gutter->value();

		if( m_useStretch->isChecked())
		{
			config.m_maxStretch = static_cast<float>(m_maxStretch->value());
			config.m_maxCharts  = 0;
		}
		else
		{
			config.m_maxStretch = 0.f;
			config.m_maxCharts  = m_maxCharts->value();
		}

		MOLogic_LightmapUVBuild->Build( config);
		MOLogic_LightmapUVBuild->Save();
	}

	// 还原模型
	void UVBuild::Restore()
	{
		MOLogic_LightmapUVBuild->Restore();
	}

	// 警告
	void UVBuild::Warning( const char* msg)
	{
		QMessageBox::warning( this, tr("UV生成失败"), tr( msg), QMessageBox::Ok | QMessageBox::Cancel);
	}
}