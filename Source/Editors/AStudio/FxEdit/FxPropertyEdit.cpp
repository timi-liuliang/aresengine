#include <QtGui/QtGui>
#include <Engine/AresRoot.h>
#include "FxPropertyEdit.h"
#include <EditorKit/DataTypeConversion.h>

namespace QT_UI
{
	// 构造函数
	FxPropertyEdit::FxPropertyEdit( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		m_fx3dpatchProperty		= new_ QProperty( "config/fxedit/fx3dpatch.property");
		m_particleSystemProperty= new_ QProperty( "config/fxedit/particlesystem.property");

		m_fx3dpatchProperty->m_model->Signal_ValueChanged.connect( boost::bind<void>( &FxPropertyEdit::RefreshPropertyUITo3DPatch, this));
		m_particleSystemProperty->m_model->Signal_ValueChanged.connect( boost::bind<void>( &FxPropertyEdit::RefreshPropertyUIToParticleSystem, this));
	}

	// 选中FxObject
	void FxPropertyEdit::OnSelectFxObject( FxObject* fxObject)
	{
		m_fxObject = fxObject;
		if( m_fxObject)
		{
			switch( m_fxObject->GetType())
			{
			case FOT_3DPatch:
				{
					m_treeView->setModel( m_fx3dpatchProperty->m_model);
					m_treeView->setItemDelegate( m_fx3dpatchProperty->m_delegate);

					RefreshProperty3DPatchToUI();
				}
				break;

			case FOT_ParticleSystem:
				{
					m_treeView->setModel( m_particleSystemProperty->m_model);
					m_treeView->setItemDelegate( m_particleSystemProperty->m_delegate);

					RefreshPropertyParticleSystemToUI();
				}
				break;
			}
		}
		else
		{
			m_treeView->setModel( NULL);
			m_treeView->setItemDelegate( NULL);
		}

		m_treeView->setAlternatingRowColors(true);
		m_treeView->expandAll();
	}

	// 更新界面参数到3D面片
	void FxPropertyEdit::RefreshProperty3DPatchToUI()
	{
		Fx3DPatch* fx3dpatch = dynamic_cast<Fx3DPatch*>( m_fxObject);
		if( fx3dpatch)
		{
			QString diffuseTexture  = fx3dpatch->GetRenderable()->GetTexture();
			QVariant billBoard		= fx3dpatch->GetRenderable()->IsBillBoard();
			QVariant renderQueue	= (INT)fx3dpatch->GetRenderable()->GetMaterial()->GetRenderQueue();
			QString  color			= DTC::ColorRGBToString( fx3dpatch->GetColor()).c_str();

			m_fx3dpatchProperty->m_model->setValue( "DiffuseTexture", diffuseTexture);
			m_fx3dpatchProperty->m_model->setValue( "Billboard", billBoard);
			m_fx3dpatchProperty->m_model->setValue( "RenderQueue", renderQueue);
			m_fx3dpatchProperty->m_model->setValue( "Color", color);
		}
	}

	// 更新3D面片属性到界面
	void FxPropertyEdit::RefreshPropertyUITo3DPatch()
	{
		Fx3DPatch* fx3dpatch = dynamic_cast<Fx3DPatch*>( m_fxObject);
		if( fx3dpatch)
		{
			QString  texturePath;
			bool	 isBillboard=false;
			BITFIELD renderQueue=RQ_TransparentEffect;
			ColorRGB path3DColor;

			QVariant variant;
			if( m_fx3dpatchProperty->m_model->findValue( variant, "DiffuseTexture"))  texturePath  = variant.toString();
			if( m_fx3dpatchProperty->m_model->findValue( variant, "Billboard"))		  isBillboard  = variant.toBool();
			if( m_fx3dpatchProperty->m_model->findValue( variant, "RenderQueue"))	  renderQueue  = variant.toInt();
			if( m_fx3dpatchProperty->m_model->findValue( variant, "Color"))			  path3DColor  = DTC::StringToColorRGB( variant.toString().toStdString());

			if( texturePath.length())
			{
				string path = texturePath.toLocal8Bit();
				fx3dpatch->GetRenderable()->SetTexture( path.c_str());
			}

			fx3dpatch->GetRenderable()->SetBillBoard( isBillboard);
			fx3dpatch->GetRenderable()->SetRenderQueue( renderQueue);
			fx3dpatch->SetColor( path3DColor);
		}
	}

	// 更新粒子系统属性到界面
	void FxPropertyEdit::RefreshPropertyParticleSystemToUI()
	{
		FxParticleSystem* particleSystem = dynamic_cast<FxParticleSystem*>( m_fxObject);
		if( particleSystem)
		{
			// 生命
			float lifeMin = particleSystem->GetModel()->GetLifeTimeMin();
			float lifeMax = particleSystem->GetModel()->GetLifeTimeMax();
			m_particleSystemProperty->m_model->setValue( "lifemin", lifeMin);
			m_particleSystemProperty->m_model->setValue( "lifemax", lifeMax);

			string gravity = DTC::Vector3ToString( particleSystem->GetGravity());
			m_particleSystemProperty->m_model->setValue( "gravity", gravity.c_str());

			FxRenderablePtr particleRender = particleSystem->GetRenderable();
			if( particleRender)
				m_particleSystemProperty->m_model->setValue( "texture", QString::fromLocal8Bit( particleRender->GetTexture()));
		}
	}

	// 更新属性界面到粒子系统
	void FxPropertyEdit::RefreshPropertyUIToParticleSystem()
	{
		FxParticleSystem* particleSystem = dynamic_cast<FxParticleSystem*>( m_fxObject);
		if( particleSystem)
		{
			QVariant variant, variant1;

			// 生命
			float lifeMin=1.f, lifeMax=1.f;
			if( m_particleSystemProperty->m_model->findValue( variant, "lifemin") && m_particleSystemProperty->m_model->findValue( variant1, "lifemax"))
			{
				lifeMin = variant.toFloat();
				lifeMax = variant.toFloat();

				particleSystem->GetModel()->SetLifeTime( lifeMin, lifeMax);
			}

			// 重力
			Vector3 gravity;
			if( m_particleSystemProperty->m_model->findValue( variant, "gravity"))  
				gravity  = DTC::StringToVector3( variant.toString().toStdString());

			particleSystem->SetGravity( gravity);

			// 贴图纹理
			FxRenderablePtr particleRender = particleSystem->GetRenderable();
			if( particleRender && m_particleSystemProperty->m_model->findValue( variant, "texture"))
			{
				string texture = variant.toString().toLocal8Bit();
				particleRender->SetTexture( texture.c_str());
			}
		}
	}
}