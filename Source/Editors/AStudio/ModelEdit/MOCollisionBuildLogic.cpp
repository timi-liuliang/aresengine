#include <Engine/AresRoot.h>
#include "MOLogic.h"
#include <Core/3rdParty/RapidXml/rapidxml_utils.hpp>
#include <Core/3rdParty/RapidXml/rapidxml.hpp>
#include <Core/3rdParty/RapidXml/rapidxml_print.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace Ares
{
	CollisionBuildLogic::CollisionBuildLogic()
		: m_selectedIdx( INVALID)
	{
	}

	// 添加shape
	void CollisionBuildLogic::AddShape( VisualShapeType type)
	{
		switch( type)
		{
		case VST_Capsule:
			{
				VisualCapsule* shape = MOLogic_VisualShpae.CreateCapsule();
				
				shape->SetShape( Capsule3( Segment3( Vector3::Zero, Vector3( 0.f, 0.f, 1.f)), 0.5f));
				shape->SetColor( ColorRGB( 1.f, 1.f, 1.f));
				
				m_visualShapes.push_back( shape);
			}
			break;
		}
	}

	// 设置当前选中形状
	void CollisionBuildLogic::SetSelected( size_t idx)
	{
		if( idx >= m_visualShapes.size())
			return;

		m_selectedIdx = idx;

		// 显示TransformAxis
		MOLogic_Transform->SetSelected( GetSelected()->GetTransform());
	}

	// 获取当前选中形状
	VisualShape* CollisionBuildLogic::GetSelected()
	{
		if( m_selectedIdx<m_visualShapes.size())
			return m_visualShapes[m_selectedIdx];

		return NULL;
	}

	// 删除当前选中
	void CollisionBuildLogic::DeleteSelected()
	{
		if( m_selectedIdx<m_visualShapes.size())
		{	
			MOLogic_VisualShpae.DeleteVisualShape( m_visualShapes[m_selectedIdx]);

			m_visualShapes.erase( m_visualShapes.begin()+m_selectedIdx);

			// 显示TransformAxis
			MOLogic_Transform->UnSelect();
		}
	}

	// 网络填充模式
	void CollisionBuildLogic::SwitchToWireFrameFillMode()
	{
		for ( size_t i=0; i<m_visualShapes.size(); i++)
		{
			// 设置光栅化状态
			RasterizerStateDesc desc;
			desc.m_fillMode = FM_WireFrame;
			RasterizerStatePtr rasterizerState = ARenderDevice->CreateRasterizerState( desc);
			m_visualShapes[i]->GetMaterial()->SetRasterizerState( rasterizerState);
		}
	}

	// 正常填充模式
	void CollisionBuildLogic::SwitchToNormalFillMode()
	{
		for ( size_t i=0; i<m_visualShapes.size(); i++)
			m_visualShapes[i]->GetMaterial()->SetRasterizerState( RasterizerStatePtr());
	}

	// 还原VisualShape
	void CollisionBuildLogic::RestoreVisualShape( Shape* shape)
	{
		if( !shape)
			return;

		switch( shape->GetShapeType())
		{
		case ST_Capsule:
			{
				Capsule3* capsule = dynamic_cast<Capsule3*>(shape);

				VisualCapsule* shape = MOLogic_VisualShpae.CreateCapsule();

				shape->SetShape( *capsule);
				shape->SetColor( ColorRGB( 1.f, 1.f, 1.f));

				m_visualShapes.push_back( shape);
			}
			break;

		case ST_Compount:
			{
				CompoundShape* cs = dynamic_cast<CompoundShape*>( shape);
				for( int i=0; i<cs->GetNumChildShapes(); i++)
				{
					RestoreVisualShape( cs->GetChildShape( i));
				}
			}
			break;
		}
	}

	// 抽取物理外形
	Shape* CollisionBuildLogic::ExtractShape( VisualShape* vs)
	{
		Shape* result=NULL;
		switch( vs->GetType())
		{
		case VST_Capsule:
			{
				VisualCapsule* vCapsule = dynamic_cast<VisualCapsule*>(vs);
				Capsule3	   capsule  = vCapsule->GetCapsule();

				result = new_ Capsule3( capsule.m_segment, capsule.m_radius);
			}
			break;
		}

		return result;
	}

	// 加载物理碰撞模型
	void CollisionBuildLogic::LoadPhysicsShapes( const std::string& filePath)
	{
		Shape* shape=NULL;
		ifstream ifs( filePath);
		if( ifs.is_open())
		{
			boost::archive::binary_iarchive biarchive( ifs);
			biarchive >> shape;
		}

		// 显示
		for ( size_t i=0; i<m_visualShapes.size(); i++)
			MOLogic_VisualShpae.DeleteVisualShape( m_visualShapes[i]);

		m_visualShapes.clear();
		RestoreVisualShape( shape);

		SAFE_DELETE( shape);
	}

	// 导出物理碰撞模型
	void CollisionBuildLogic::SavePhysicsShapes( const std::string& filePath)
	{
		if( m_visualShapes.empty()) return;

		Shape* shape=NULL;
		if( m_visualShapes.size()>1)
		{
			// 组合外形
			shape = new_ CompoundShape();
			for( size_t i=0; i<m_visualShapes.size(); i++)
			{
				CompoundShape* cs = dynamic_cast<CompoundShape*>(shape);
				Shape*      child = ExtractShape( m_visualShapes[i]);
				if( cs && child)
				{
					cs->AddChildShape( Transform(), child);
				}
			}
		}
		else if( m_visualShapes.size()==1)
		{
			// 基础外形
			shape = ExtractShape( m_visualShapes[0]);
		}

		ofstream ofs( filePath);
		if( ofs.is_open())
		{
			boost::archive::binary_oarchive biarchive( ofs);
			biarchive << shape;
		}

		SAFE_DELETE( shape);
	}

	// 导出物理碰撞模型
	void CollisionBuildLogic::YNDSavePhysicsShapes( const std::string& filePath)
	{
		using namespace boost;
		using namespace rapidxml;

		if( m_visualShapes.empty())
			return;

		list<string> storeHelper;

		// 默认xml格式
		xml_document<>  doc;
		xml_node<>* xmlNode  = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
		xml_node<>* rootNode =  doc.allocate_node(node_element,"shapes");

		// shape 数量
		storeHelper.push_back( lexical_cast<string>( m_visualShapes.size()));
		xml_attribute<>* attributeNum = doc.allocate_attribute( "num", storeHelper.back().c_str());
		rootNode->append_attribute(attributeNum);

		doc.append_node( xmlNode);
		doc.append_node( rootNode);

		for ( size_t i=0; i<m_visualShapes.size(); i++)
		{
			xml_node<>* shapeNode = doc.allocate_node( node_element, "shape");
			switch( m_visualShapes[i]->GetType())
			{
			case VST_Capsule:
				{
					VisualCapsule* vCapsule = dynamic_cast<VisualCapsule*>(m_visualShapes[i]);
					Capsule3	   capsule  = vCapsule->GetCapsule();
					Vector3		   vertex0  = capsule.m_segment.GetVertex0();
					Vector3		   vertex1  = capsule.m_segment.GetVertex1();

					// 类型转换
					if( MOLogic_ModelEdit->GetModelType()==MT_YND)
					{
						std::swap( vertex0.y, vertex0.z);
						std::swap( vertex1.y, vertex1.z);
					}

					// 类型
					xml_attribute<>* attType = doc.allocate_attribute( "type", "capsule");
					shapeNode->append_attribute(attType);

					// 顶点0
					format tv0Fmt("%.4f,%.4f,%.4f"); tv0Fmt % vertex0.x % vertex0.y % vertex0.z;
					storeHelper.push_back( tv0Fmt.str());
					xml_attribute<>* attributeV0 = doc.allocate_attribute( "vertex0", storeHelper.back().c_str());
					shapeNode->append_attribute( attributeV0);

					// 顶点1
					format tv1Fmt("%.4f,%.4f,%.4f"); tv1Fmt % vertex1.x % vertex1.y % vertex1.z;
					storeHelper.push_back( tv1Fmt.str());
					xml_attribute<>* attributeV1 = doc.allocate_attribute( "vertex1", storeHelper.back().c_str());
					shapeNode->append_attribute( attributeV1);

					// 半径
					storeHelper.push_back( lexical_cast<string>( capsule.m_radius));
					xml_attribute<>* attributeNum = doc.allocate_attribute( "radius", storeHelper.back().c_str());
					shapeNode->append_attribute(attributeNum);
				}
				break;
			}

			rootNode->append_node( shapeNode);
		}

		// 写文件
		std::ofstream out( filePath.c_str());
		out << doc;
	}

	// 加载物理碰撞模型
	void CollisionBuildLogic::YNDLoadPhysicsShapes( const std::string& filePath)
	{
		using namespace boost;
		using namespace rapidxml;

		for ( size_t i=0; i<m_visualShapes.size(); i++)
			MOLogic_VisualShpae.DeleteVisualShape( m_visualShapes[i]);

		m_visualShapes.clear();

		try
		{
			file<> fdoc( filePath.c_str());
			xml_document<>  doc;
			doc.parse<0>( fdoc.data());

			xml_node<>* pRootNode = doc.first_node("shapes");
			if( !pRootNode) return;

			for( xml_node<>* shapeNode = pRootNode->first_node( "shape"); shapeNode; shapeNode=shapeNode->next_sibling("shape"))
			{
				if( string( shapeNode->first_attribute("type")->value())==string("capsule"))
				{
					Vector3 vertex0;
					Vector3 vertex1;
					float	radius=1e30f;

					sscanf( shapeNode->first_attribute("vertex0")->value(), "%f,%f,%f", &vertex0.x, &vertex0.y, &vertex0.z);
					sscanf( shapeNode->first_attribute("vertex1")->value(), "%f,%f,%f", &vertex1.x, &vertex1.y, &vertex1.z);
					radius	= lexical_cast<float>(shapeNode->first_attribute("radius")->value());

					VisualCapsule* shape = MOLogic_VisualShpae.CreateCapsule();

					// 类型转换
					if( MOLogic_ModelEdit->GetModelType()==MT_YND)
					{
						std::swap( vertex0.y, vertex0.z);
						std::swap( vertex1.y, vertex1.z);
					}

					shape->SetShape( Capsule3( Segment3( vertex0, vertex1), radius));
					shape->SetColor( ColorRGB( 1.f, 1.f, 1.f));

					m_visualShapes.push_back( shape);
				}
			}
		}
		catch ( ...)
		{

		}
	}
}