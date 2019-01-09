//--------------------------------------
// http://www.quyougame.com AresRender
//--------------------------------------
#pragma once

#include <d3dx9.h>
#include <stdio.h>
#include <Core/AresCore.h>
#include "../Foundation/Ares3DEngine_h.h"

namespace Ares
{
	//--------------------------------------------------
	// IBaseGeometry (基本几何体) 2010-07-28 帝林
	//--------------------------------------------------
	class ARES_EXPORT IBaseGeometry
	{
		struct SHeader;
	public:
		enum EGeometryType
		{
			EM_GT_NULL = 0, // 什么都没有
			EM_GT_ANY,		// 任意形状
			EM_GT_LINE,		// 线段 
			EM_GT_RAY,		// 射线,与线段差距不大
			EM_GT_AABB,		// AABB包围盒
			EM_GT_OBB,
			EM_GT_CONE,
			EM_GT_CYCLE,	// 圆
		};

		// 构造函数
		IBaseGeometry();

		// 析构函数
		virtual ~IBaseGeometry();

		// 返回颜色
		const DWORD& GetColor();

		// 返回几何体类型
		EGeometryType GetType();

	public:
		// 返回世界矩阵
		const Matrix44& GetWorldMatrix();

		// 设置世界矩阵
		void SetWorldMatrix( const Matrix44& mat);

		// 绘制
		virtual void  Draw()=0;

		// 设置颜色
		void  SetColor( DWORD color);

		// 设置位置
		void  SetPosition( float _x, float _y, float _z);

		// 设置缩放
		void  SetScale( float fScale);

		// 设置可见性
		void  SetVisible( bool visible);

		// 是否可见
		bool  IsVisible();

	protected:
		SHeader*	   m_header;		// 数据头
		EGeometryType  m_type;
		DWORD          m_dwColor;		// 基本体颜色
		bool           m_bVisible;		// 是否可见
		FLOAT          m_fScale;		// 缩放
	};

	//--------------------------------------------------
	// 任意 2010-08-05   帝林
	//--------------------------------------------------
	class IGeometryAny : public IBaseGeometry
	{
	public:
		// 直接设置顶点索引数据
		virtual void SetParam(UINT32 vertexCount, void* vertexData, D3DPRIMITIVETYPE type, UINT32 indexNum, void* indexData, D3DFORMAT format=D3DFMT_INDEX16)=0;
	};

	//--------------------------------------------------
	// 线段 2010-07-28   帝林
	//--------------------------------------------------
	class ILine : public IBaseGeometry
	{
	public:
		// 设置线段的参数
		virtual void SetParam( float _x0, float _y0, float _z0, float _x1, float _y1, float _z1)=0;

		// 设置参数
		virtual void SetParam( Vector3& pointBegin, Vector3& face)=0;
	};

	//-------------------------------------------------
	// 盒子  2010-07-28   帝林
	//-------------------------------------------------
	class IBox : public IBaseGeometry
	{
	public:
		// AABB包围盒
		virtual void SetParam( float _x0, float _y0, float _z0, float _x1, float _y1, float _z1)=0;
	};

	//--------------------------------------------------
	// 圆锥体 2010-08-03 13:31  帝林
	//--------------------------------------------------
	class ICone : public IBaseGeometry
	{
	public:
		// 设置创建
		virtual void SetRadiusAndHeight( float _radius, float _height)=0;

		// 设置朝向
		virtual void SetFace( Vector3& face)=0;
	};

	//--------------------------------------------------
	// 基础显示图球 圆  2010-08-05 17:16 帝林
	//-------------------------------------------------
	class IRenderCycle : public IBaseGeometry
	{
	public:
		// 设置半径
		virtual void SetRadius( float _radius)=0;

		// 设置面向
		virtual void SetFace( Vector3& face)=0;
	};

	//--------------------------------------------------
	//  Sky    2010-05-31 13：51  帝林
	//--------------------------------------------------
	class ISurfaceMaterial;
	class ISkyModel
	{
	public:
		// 指定太阳位置( for sun shaft)
		virtual void SetSunPos( float _x, float _y, float _z)=0;

		// 更新
		virtual void FrameMove(float fTime)=0;
	};

	//---------------------------------------------------
	// 镜头眩光   2010-06-08   帝林
	//---------------------------------------------------
	class ILensFlare
	{
	public:
		// 初始化,指定纹理资源路径
		virtual void Initialize( char* texturePath, WORD totalNum=9, WORD numPerLine=3)=0;

		// 更新
		virtual void Update()=0;

		// 渲染
		virtual void Render()=0;

		// 设置眩光颜色
		virtual void SetLightColor( DWORD color)=0;

		// 设置发光体所在方向
		virtual void SetLightVector( Vector3& sunLightVector)=0;
	};

	//----------------------------------
	// 基本几何体 2011-08-15  帝林
	//----------------------------------
	class IGeometryMgr
	{
	public:
		// 创建Line
		virtual ILine* CreateLine( IBaseGeometry::EGeometryType type = IBaseGeometry::EM_GT_LINE)=0;

		// 创建圆锥体
		virtual ICone* CreateCone()=0;

		// 创建Box
		virtual IBox* CreateBox( IBaseGeometry::EGeometryType type = IBaseGeometry::EM_GT_AABB)=0;

		// 创建圆
		virtual IRenderCycle* CreateCycle()=0;

		// 创建任意基本形
		virtual IGeometryAny* CreateGeometryAny()=0;

		// 渲染
		virtual void Render()=0;
	};
}
