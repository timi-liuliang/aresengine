#pragma once

#include <Core/AresCore.h>
#include <Engine/RenderSystem/SuperMaterial.h>
#include <Engine/RenderSystem/AresTerrain.h>

namespace Ares
{
	//----------------------------------------
	// ZWriteRenderable 2013-2-21 帝林
	//----------------------------------------
	class ZRenderable
	{
	public:
		ZRenderable( bool isZTest=false);
		~ZRenderable();

		// 渲染
		void Render( const Matrix44& worldViewProj);

		// 设置Renderlayout
		void SetRenderlayout( const RenderLayoutPtr& renderLayout) { m_layout = renderLayout;}

		// 设置纹理
		void SetMaskTexture( ShaderParamPtr mask);

	private:
		RenderLayoutPtr	m_layout;			// RenderGeometry
		SuperMaterial	m_material;			// RenderMaterial

		ShaderParamPtr	m_worldViewProj;	// 参数
		ShaderParamPtr	m_maskTexture;		// ZTest纹理
	};

	//--------------------------------------
	// ZWriteObject 2013-8-10 帝林
	//--------------------------------------
	class ZWriteObject
	{
	public:
		// 渲染
		virtual void Render( const Matrix44& worldViewProj)=0;

		// 执行写深度
		virtual void ExecZWrite()=0;
	};

	//--------------------------------------
	// ZWriteTerrain 2013-8-10 帝林
	//--------------------------------------
	class ZWriteTerrain
	{
	public:
		ZWriteTerrain( Terrain* terrain);
		~ZWriteTerrain();

		// 渲染
		virtual void Render( const Matrix44& worldViewProj);

		// 执行写深度
		virtual void ExecZWrite();

	private:
		vector<ZRenderable*>		m_zRenderables;		// 深度渲染
	};
}