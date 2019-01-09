#include "MOAnimationEdit.h"
#include "MOLogic.h"
#include <QtGui/QtGui>
#include <Engine/RenderSystem/AresAnimationController.h>
#include <tolua++/tolua++.h>

using namespace Ares;

namespace QT_UI
{
	// 构造函数
	CAnimationEdit::CAnimationEdit( QWidget* parent/* = 0*/)
		: QDockWidget( parent)
	{
		setupUi( this);

		connect( m_animationList, SIGNAL(activated( const QString &)), this, SLOT( OnClicked()));
		connect( m_animationBox,  SIGNAL(clicked()), this, SLOT( InitAniList()));
		connect( m_loop,          SIGNAL(clicked()), this, SLOT( OnClicked()));

		// 时间事件
		//m_pTimer = new QTimer( this);
		//connect( m_pTimer, SIGNAL( timeout()), this, SLOT( InitAniList()));
		connect( m_test,   SIGNAL( clicked()), this, SLOT( SwitchWeapon()));

		// 骨骼选择事件
		connect( m_boneHirearchy, SIGNAL( itemSelectionChanged()), this, SLOT( ShowBoneCustomInfo()));

		connect( m_scaleX, SIGNAL( valueChanged(double)), this, SLOT(UpdateBoneCustomInfo()));
		connect( m_scaleY, SIGNAL( valueChanged(double)), this, SLOT(UpdateBoneCustomInfo()));
		connect( m_scaleZ, SIGNAL( valueChanged(double)), this, SLOT(UpdateBoneCustomInfo()));
		connect( m_transX, SIGNAL( valueChanged(double)), this, SLOT(UpdateBoneCustomInfo()));
		connect( m_transY, SIGNAL( valueChanged(double)), this, SLOT(UpdateBoneCustomInfo()));
		connect( m_transZ, SIGNAL( valueChanged(double)), this, SLOT(UpdateBoneCustomInfo()));

		m_iconMap["bone"] = QIcon(":/icon/Icon/bone.png");
	}

	// 析构函数
	CAnimationEdit::~CAnimationEdit()
	{

	}

	// 重置(模型选择更改)
	void CAnimationEdit::Reset()
	{
		m_animationList->clear();

		//m_pTimer->start( 800);
	}

	// 初始化动画列表
	void CAnimationEdit::InitAniList()
	{
		if( m_animationList->count() != 0)
			return;

		ModelPtr tEntity = MOLogic_ModelEdit->GetCurrEntity();
		if( !tEntity)
			return;

		// 动画控制器
		AnimationController* pController = tEntity->GetAnimationController();
		s_ptr<CAresAnimation> pAnimation = pController->GetAnimation();
		if( !pAnimation)
			return;

		int tNum = pAnimation->GetNumAnimationSet();
		for ( int i=0; i<tNum; i++)
		{
			QString name = QString::fromUtf8("中文");
			
			m_animationList->addItem( tr(pAnimation->GetAnimationSetName( i))); 
		}

		// 显示骨骼结构
		//ShowBoneHierarchy( pAnimation);

		// 更新常规属性显示
		//ShowBoneGeneralInfo( pAnimation);


		//m_pTimer->stop();
	}

	//// 显示骨骼的树型结构
	//void CAnimationEdit::ShowBoneHierarchy( IAresAnimation* animation)
	//{
	//	m_boneHirearchy->clear();

	//	if( !animation)
	//		return;

	//	if( !animation->GetNumBones())
	//		return;

	//	int sib = animation->GetRootBone();
	//	while ( sib != -1)
	//	{
	//		const char* name = animation->GetBoneName( sib);
	//		QTreeWidgetItem* item = new QTreeWidgetItem( m_boneHirearchy,  QStringList(name));
	//		item->setIcon( 0, m_iconMap["bone"]);

	//		AddBone( animation, item, sib);

	//		sib = animation->GetBoneSibingIdx( sib);
	//	}
	//}

	//// 添加骨骼
	//void CAnimationEdit::AddBone( IAresAnimation* animation,QTreeWidgetItem* parentItem,  size_t parentIdx)
	//{
	//	int sib	  = animation->GetBoneFirstChildIdx( parentIdx);
	//	while ( sib != -1)
	//	{
	//		const char* name = animation->GetBoneName( sib);
	//		QTreeWidgetItem* item = new QTreeWidgetItem( parentItem, QStringList(name));
	//		item->setIcon( 0, m_iconMap["bone"]);

	//		// 递归
	//		AddBone( animation, item, sib);

	//		sib = animation->GetBoneSibingIdx( sib);
	//	}
	//}

	// 单击
	void CAnimationEdit::OnClicked()
	{
		ModelPtr pEntity = MOLogic_ModelEdit->GetCurrEntity();
		if( pEntity)
		{
			int index = m_animationList->currentIndex();

			// 动画控制器
			AnimationController* pController = pEntity->GetAnimationController();
			//pController->GetAnimationDesc( 0)->m_loop  = true;
			//pController->GetAnimationDesc( 1)->m_loop  = true;
			//pController->GetAnimationDesc( 2)->m_loop  = true;
			//pController->GetAnimationDesc( 3)->m_layer = 1;
			//pController->GetAnimationDesc( 3)->m_loop  = true;
			//pController->GetAnimationDesc( 4)->m_layer = 2;
			//pController->GetAnimationDesc( 4)->m_weight = 0.5f;
			//pController->GetAnimationDesc( 5)->m_layer = 1;

			//pController->GetAnimationDesc(index)->m_loop = m_loop->isChecked();

			//int boneIdx;

			//pController->GetAnimation()->GetBoneIdxByName( "Bip01 Head", boneIdx);
			//pController->GetAnimationDesc(4)->AddMixingTransform( boneIdx);

 			//if( pController->GetAnimationDesc( index)->m_layer != 1)
				pController->CrossFade( index, 0.f);
			//else
				//pController->CrossFadeQueued( index);

			//pController->PlayAnimationByIndex( index, m_loop->isChecked());

			// 创建一个指向lua解释器的指针
			//lua_State* ls = lua_open();

			//// 加载lua库
			//luaopen_base( ls);
			////luaL_openlibs( ls);
			//tolua_AresScriptSystem_open(ls);

			//// 加载脚本
			//int re = luaL_dofile( ls, "data\\lua\\blending.lua");

			//// 调用函数
			//lua_getglobal( ls, "playAnimation");
			//tolua_pushusertype( ls, pController, "Ares::IAnimationController");
			//tolua_pushusertype( ls, &index, "size_t");
			//tolua_pushboolean( ls, m_loop->isChecked());

			//const char* errMsg = NULL;
			//int result = lua_pcall( ls, 3, 0, 0);
			//if( result)
			//{
			//	errMsg = lua_tostring( ls, -1);
			//	int breakPoint = 10;
			//}

			//lua_close(ls);
		}
	}

	// 更换武器
	void CAnimationEdit::SwitchWeapon()
	{
		QString qFileName = QFileDialog::getOpenFileName( this, tr("model file"), "", tr("*.m1"));

		QDir dir( QDir::currentPath());
		qFileName = dir.relativeFilePath( qFileName);

		// 更改标题
		m_weapon->setText( qFileName.toStdString().c_str());

		//MOLogic_Main->SwitchWeapon( qFileName.toStdString().c_str());
	}

	// 显示常规信息
	/*void CAnimationEdit::ShowBoneGeneralInfo( IAresAnimation* animation)
	{
		// 重置
		m_generalProperty->clear();
		m_generalProperty->setRowCount( 2);
		m_generalProperty->setColumnCount( 2);

		// 标题
		QStringList header; 
		header<<"Property"<<"Value"; 
		m_generalProperty->setHorizontalHeaderLabels(header); 

		// 显示骨骼数量
		int tNum = animation->GetNumBones();
		char boneNum[1024];
		itoa( tNum, boneNum, 10);
		m_generalProperty->setItem(0,0,new QTableWidgetItem(tr("BoneNum:")));  m_generalProperty->setItem( 0, 1, new QTableWidgetItem( tr( boneNum))); 
	}*/

	// 显示自定义骨骼信息
	void CAnimationEdit::ShowBoneCustomInfo()
	{/*
		QTreeWidgetItem* tItem = m_boneHirearchy->currentItem();
		SPtr<IM1Entity> pEntity = g_pLogic->GetCurrEntity();
		if( pEntity)
		{
			// 动画控制器
			IAnimationController* pController = pEntity->GetAnimationController();
			IAresAnimation*		  animation	  = pController->GetAnimation();

			int boneIdx;
			if( animation->GetBoneIdxByName( tItem->text(0).toStdString().c_str(), boneIdx))
			{
				// 获取自定义矩阵
				Matrix44 tCM;
				pController->GetCustomBone( tCM, boneIdx);

				// 显示
				string text;
				char   str[1024];
				for ( int i=0; i<4; i++)
				{
					sprintf( str, "%8.3f, %8.3f, %8.3f, %8.3f\n", tCM.m[i][0], tCM.m[i][1], tCM.m[i][2], tCM.m[i][3]);

					text += str;
				}

				m_customBone->setText( text.c_str());

				// 分解旋转缩放平移
				Vector3    tScale;
				D3DXQUATERNION tRotate;
				Vector3	   tTrans;
				D3DXMatrixDecompose( &tScale, &tRotate, &tTrans, &tCM);

				// 显示缩放
				m_scaleX->setValue( tScale.x); 
				m_scaleY->setValue( tScale.y);
				m_scaleZ->setValue( tScale.z);

				m_transX->setValue( tTrans.x);
				m_transY->setValue( tTrans.y);
				m_transZ->setValue( tTrans.z);
			
				// 显示骨骼的初始坐标系
				Matrix44 tCoord;
				animation->GetInitBoneTM( tCoord, boneIdx);

				g_pLogic->ShowBoneCoord( tCoord);
			}
		}*/
	}

	// 更新自定义骨骼信息
	void CAnimationEdit::UpdateBoneCustomInfo()
	{/*
		QTreeWidgetItem* tItem = m_boneHirearchy->currentItem();
		if( !tItem)
			return;

		SPtr<IM1Entity> pEntity = g_pLogic->GetCurrEntity();
		if( pEntity)
		{
			// 动画控制器
			IAnimationController* pController = pEntity->GetAnimationController();

			int boneIdx;
			if( pController->GetAnimation()->GetBoneIdxByName( tItem->text(0).toStdString().c_str(), boneIdx))
			{
				// 获取自定义矩阵
				Matrix44 tCM;
				pController->GetCustomBone( tCM, boneIdx);

				// 分解旋转缩放平移
				Vector3    tScale;
				D3DXQUATERNION tRotate;
				Vector3	   tTrans;
				D3DXMatrixDecompose( &tScale, &tRotate, &tTrans, &tCM);

				// 显示缩放
				tScale.x = m_scaleX->value();
				tScale.y = m_scaleY->value();
				tScale.z = m_scaleZ->value();

				tTrans.x = m_transX->value();
				tTrans.y = m_transY->value();
				tTrans.z = m_transZ->value();

				D3DXMatrixTransformation( &tCM, NULL, NULL, &tScale, NULL, &tRotate, &tTrans);
				pController->SetCustomBone( boneIdx, tCM);

				// 显示
				string text;
				char   str[1024];
				for ( int i=0; i<4; i++)
				{
					sprintf( str, "%8.3f, %8.3f, %8.3f, %8.3f\n", tCM.m[i][0], tCM.m[i][1], tCM.m[i][2], tCM.m[i][3]);

					text += str;
				}

				m_customBone->setText( text.c_str());
			}
		}*/
	}
}