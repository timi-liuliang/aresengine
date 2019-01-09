#include "MOLogic.h"
#include <Engine/AresRoot.h>
#include <boost/array.hpp>
#include "MOCollisionBuildLogic.h"
#include "MOYNDMeshToAres.h"

//#define YND_SUPPORT
#define lineNum 10

namespace Ares
{
	// 构造函数
	ModelEdit::ModelEdit()
	{
		m_modelPath = "H:\\Aresengine\\trunk\\Bin\\data\\model\\";

		CreateWhiteGrid();
		CreateRedCross();
	}

	// 创建白色网格,确定模型位置
	void ModelEdit::CreateWhiteGrid()
	{
		m_whiteGrid = MOLogic_VisualShpae.CreateVisualShape();

		boost::array<Vector3, lineNum*8> vertexData;
		boost::array<WORD	, lineNum*8> indexData;

		for ( int i=0; i<lineNum;i++)
		{
			float j=float(i+1);
			vertexData[i*8+0] = Vector3(  lineNum,        j, 0); 
			vertexData[i*8+1] = Vector3( -lineNum,        j, 0);
			vertexData[i*8+2] = Vector3(        j,  lineNum, 0);
			vertexData[i*8+3] = Vector3(        j, -lineNum, 0);
			vertexData[i*8+4] = Vector3(  lineNum,       -j, 0); 
			vertexData[i*8+5] = Vector3( -lineNum,       -j, 0);
			vertexData[i*8+6] = Vector3(	   -j,  lineNum, 0);
			vertexData[i*8+7] = Vector3(	   -j, -lineNum, 0);
		}

		for( size_t i=0; i<indexData.size(); i++)
			indexData[i] = i;

		int vertexSizeInByte  = sizeof( Vector3) * vertexData.size();
		int indicesSizeInByte = sizeof(WORD) * indexData.size();
		m_whiteGrid->Set( RenderLayout::TT_LineList, vertexData.data(), vertexSizeInByte, indexData.data(), indicesSizeInByte, EF_R16UI);
		m_whiteGrid->GetTransform()->SetTrans( 0.f, 0.f, 0.f);
		m_whiteGrid->SetColor( ColorRGB( 0.6f, 0.6f, 0.6f));
		m_whiteGrid->SetVisible( true);
	}

	// 创建红色中线
	void ModelEdit::CreateRedCross()
	{
		m_redCross	= MOLogic_VisualShpae.CreateVisualShape();

		Vector3 vertexData[4];
		WORD	indexData[4] = { 0, 1, 2, 3 };

		vertexData[0] = Vector3(  -lineNum,			   0,  0); 
		vertexData[1] = Vector3(   lineNum,			   0,  0);
		vertexData[2] = Vector3(			 0, -lineNum,  0);
		vertexData[3] = Vector3(			 0,	 lineNum,  0);

		int vertexSizeInByte  = sizeof( Vector3) *  4;
		int indicesSizeInByte = sizeof(WORD) * 4;
		m_redCross->Set( RenderLayout::TT_LineList, vertexData, vertexSizeInByte, indexData, indicesSizeInByte, EF_R16UI);
		m_redCross->GetTransform()->SetTrans( Vector3::Zero);
		m_redCross->SetColor( ColorRGB( 0.7f, 0.0f, 0.0f));
		m_redCross->SetVisible( true);
	}

	// 设置实体
	void ModelEdit::SetResEntity( const char* resPath)
	{
		m_currEntity.reset();

		// 根据后缀名,区分类型
		path filePath(resPath);

		m_currResPath = resPath;

		// 加载角色
		if( filePath.extension()==".cha")
		{
		}
		else if( filePath.extension()==".model")
		{
			// 删除实体
			EntityEdit_Scene->DelEntity( "ModelEdit_Model");

			m_currEntity = AResSystem.LoadModel( resPath);

			// 添加实体
			EntityEdit_Scene->AddEntity( "ModelEdit_Model", m_currEntity);

			Zoom();

			MOLogic_Transform->UnSelect();
		}
		else if( filePath.extension()==".GMe")
		{/*
			ModelConverter::YNDToAres( resPath, m_modelPath, );

			// 1.计算存储路径
			path arespath( m_modelPath + path( resPath).filename().string());
			arespath.replace_extension( ".model");

			SetResEntity( arespath.string().c_str());*/
		}
		else if( filePath.extension()==".afx")
		{

		}
	}

	// Zoom
	void ModelEdit::Zoom()
	{
		if( m_currEntity)
			MOLogic_Camera->Zoom( GetModelWorldBounds());
	}

	// 获取模型类型
	ModelType ModelEdit::GetModelType()
	{
		if( m_currEntity)
			return MT_ARES;

		return MT_NULL;
	}

	// 获取模型包围盒半径
	float ModelEdit::GetModelRadius()
	{
		Rect3 worldBounds;
		if( m_currEntity)
		{
			worldBounds = m_currEntity->GetWorldBounds();		

			return std::max<float>( worldBounds.GetMinPoint().Length(), worldBounds.GetMaxPoint().Length());
		}

		return 10.f;
	}

	// 获取模型世界包围盒
	Rect3 ModelEdit::GetModelWorldBounds()
	{
		if( m_currEntity)
			return m_currEntity->GetWorldBounds();

		return Rect3( Vector3( -1.f, -1.f, -1.f), Vector3( 1.f, 1.f ,1.f));
	}

	// 更新
	void ModelEdit::FrameMove( float elapsedTime)
	{
		if( m_currEntity)
			m_currEntity->FrameMove( elapsedTime);
	}

	// 重新加载
	void ModelEdit::Reload()
	{
		// 根据后缀名,区分类型
		path filePath( m_currResPath);
		if( filePath.extension()==".model")
		{
			m_currEntity.reset();
			m_currEntity = AResSystem.LoadModel( m_currResPath.c_str());
				
			EntityEdit_Scene->AddEntity( "caodandan", m_currEntity);
		}
	}
}