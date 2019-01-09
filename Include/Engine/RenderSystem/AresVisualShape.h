#pragma once

#include <stdio.h>
#include <Core/AresCore.h>
#include <Core/AresSingleton.hpp>
#include <Physics/Shapes.h>
#include "Engine/RenderSystem/AresRenderable.h"
#include "Engine/Foundation/Ares3DEngine_h.h"
#include "Engine/SceneSystem/AresScene.h"

namespace Ares
{
	// 类型
	enum VisualShapeType
	{
		VST_Base,			// 基础	
		VST_Capsule,		// 胶囊
	};

	//--------------------------------------------------
	// VisualShape (基本几何体) 2010-07-28 帝林
	//--------------------------------------------------
	class VisualShape : public Renderable
	{
	public:
		VisualShape( Scene& scene, VisualShapeType type=VST_Base);
		virtual ~VisualShape();

		// 更新
		void FrameMove();

		// 返回类型
		VisualShapeType GetType() { return m_type; }

		// 获取转换控制器
		Transform* GetTransform() { return &m_transform; }

		// 返回颜色
		//const ColorRGB GetColor() { return *m_color; }

		// 设置固定大小
		void SetSizeFixed( bool isFixed, float sizePixel=100.f);

		// 设置颜色
		void  SetColor( ColorRGB color) { *m_color=color; }

		// 设置深度偏移
		void SetDepthBias( float depthBias) { *m_depthBias = depthBias; }

		// 设置可见性
		void  SetVisible( bool visible) { m_visible = visible; }

		// 是否可见
		bool  IsVisible() { return m_visible; }

		// 设置RenderLayout
		void Set(RenderLayout::TopologyType topologyType, Vector3* positions, int vertexSizeInByte, const void* indices, int indicesSizeInByte, ElementFormat indiceFormat=EF_R16UI, Vector3* normals=NULL, DWORD* colors=NULL);

	protected:
		VisualShapeType	m_type;			// 类型

		ShaderParamPtr	m_color;		// 颜色
		ShaderParamPtr  m_useNormal;	// 是否使用法线
		ShaderParamPtr	m_spWorld;		// 世界矩阵
		ShaderParamPtr	m_worldViewProj;// 位置世界矩阵
		ShaderParamPtr	m_depthBias;	// 深度偏移

		bool			m_visible;		// 是否可见
		bool			m_isFixedSize;	// 是否大小固定
		Transform		m_transform;	// 转换
		Scene&			m_scene;	    // 场景系统
	};
	typedef s_ptr<VisualShape> VisualShapePtr;

	//--------------------------------------------------
	// 线段 2010-07-28   帝林
	//--------------------------------------------------
	class ARES_EXPORT VisualSegment3 : public VisualShape
	{
	public:
		// constructor
		VisualSegment3( Scene& scene);

		// 设置线段的参数
		void Set( const Segment3& segment);

		// 设置线段参数( vertex0, vertex1)
		void Set( float x0, float y0, float z0, float x1, float y1, float z1);

	private:
		Segment3	m_segment;
	};
	typedef s_ptr<VisualSegment3> VisualSegmentPtr;

	//-------------------------------------------------
	// 盒子  2010-07-28   帝林
	//-------------------------------------------------
	class ARES_EXPORT VisualRect3 : public VisualShape
	{
	public:
		// constructor
		VisualRect3( Scene& scene);

		// AABB包围盒
		void Set( const Rect3& rect);

	private:
		Rect3		m_rect;
	};
	typedef s_ptr<VisualRect3> VisualRect3Ptr;

	//--------------------------------------------------
	// 轴向包围盒渲染 2013-7-3 帝林
	//--------------------------------------------------
	class VisualBox3 : public VisualShape
	{
	public:
		VisualBox3( Scene& scene);

		// 设置轴向包围盒
		void Set(const Box3& box);

	private:
		Box3			m_box3;
	};

	//--------------------------------------------------
	// 圆锥体 2010-08-03 13:31  帝林
	//--------------------------------------------------
	class VisualCone3 : public VisualShape
	{
	public:
		// constructor
		VisualCone3( Scene& scene);

		// 设置创建
		virtual void SetRadiusAndHeight( float _radius, float _height);

		// 设置朝向
		virtual void SetFace( Vector3& face);
	};
	typedef s_ptr<VisualCone3> VisualCone3Ptr;

	//--------------------------------------------------
	// 基础显示图球 圆  2010-08-05 17:16 帝林
	//-------------------------------------------------
	class VisualCycle3 : public VisualShape
	{
	public:
		// constructor
		VisualCycle3( Scene& scene);

		// 设置半径
		void SetRadius( float _radius);

		// 设置面向
		void SetFace( Vector3& face);
	};
	typedef s_ptr<VisualCycle3> VisualCycle3Ptr;

	//--------------------------------------------------
	// 胶囊体显示 2012-9-10 帝林
	//--------------------------------------------------
	class VisualCapsule : public VisualShape
	{
	public:
		// 构造函数
		VisualCapsule( Scene& scene);

		// 设置参数
		void SetShape( const Capsule3& capsule);

		// 设置参数
		void SetShape( float height, float radius);

		// 获取半径
		float GetRadius() { return m_capsuleInternal.m_radius; }

		// 获取高
		float GetHeight() { return m_capsuleInternal.m_segment.Length(); }

		// 获取胶囊
		Capsule3 GetCapsule();

	private:
		Capsule3		m_capsuleInternal;		// 只记录高度与半径
	};
	typedef s_ptr<VisualCapsule> VisualCapsulePtr;


	//--------------------------------------
	// 基本几何体渲染管理器 2011-08-15  帝林
	//--------------------------------------
	class RenderQueue;
	class VisualShapeMgr
	{
		typedef multimap< int, VisualShape*> VisualShapeList;
	public:
		// constructor
		VisualShapeMgr( Scene& scene);

		// deconstructor
		~VisualShapeMgr();

		// 创建任意基本形
		VisualShape* CreateVisualShape( int layer=0);

		// create line
		VisualSegment3* CreateSegment( int layer=0);

		// create aabb
		VisualRect3* CreateRect3( int layer=0);

		// 创建轴向包围盒
		VisualBox3* CreateBox3( int layer=0);

		// 创建圆锥体
		VisualCone3* CreateCone( int layer=0);

		// 创建圆
		VisualCycle3* CreateCycle( int layer=0);

		// 创建胶囊
		VisualCapsule* CreateCapsule( int layer=0);

		// 删除
		bool DeleteVisualShape( VisualShape* shape);

	public:
		// 更新
		void FrameMove();

		// 渲染
		void SubmitRenderElements( RenderElementMgr& renderElementMgr);		

	private:
		VisualShapeList  m_shapes;			// visual shapes
		Scene&			 m_scene;
	};
}
