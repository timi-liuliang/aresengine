#include "SCLogic.h"
#include "LightMap.h"

namespace QT_UI
{
	// 构造函数
	Lightmap::Lightmap( QWidget* parent)
		: QMainWindow( parent)
	{
		setupUi( this);

		connect( m_testBake, SIGNAL(clicked()), this, SLOT(Bake()));
	}

	// 析构函数
	Lightmap::~Lightmap()
	{
	}

	// 烘焙主函数
	void Lightmap::Bake()
	{
		//LuaScriptPtr script = AResSystem->LoadLuaScript("H:\\AresEngine\\Sdk\\bin\\data\\script\\D3D11Render.lua");

		//script->ExecFunc( "OnInit");

		// 是否构建静态物体
		if( m_isBuildStatic->isChecked ())
		{
			// 是否只构建当前选中 
			if( m_isBuildSelectedOnly->isChecked ())
				BakeSelected();
		}
	}

	// 烘焙
	void Lightmap::BakeSelected()
	{
		RenderEntity* curEntity = SCLogic_EntityEdit->GetEditEntity<RenderEntity>();
		if( curEntity /*&& curEntity->IsStatic()*/)
			Logic_LightmapBake->AddTask( curEntity);
		else
			QMessageBox::warning( this, "Warning", "No static entity is selected!");
	}
}