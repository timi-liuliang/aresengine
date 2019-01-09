#pragma once

#include <Core/AresCore.h>
#include "AresLight.h"
#include <Engine/FxSystem/AresFxEntity.h>
#include "Engine/RenderSystem/CSMMgr.h"

using namespace boost;
using namespace boost::signals2;

namespace Ares
{
	//------------------------------------------
	// 方向光 2012-6-20 帝林
	//	   编辑器保证方向光一直位于场景的最高处
	//------------------------------------------
	class DirectionalLight : public Light
	{
	public:
		// shader param type
		enum SPType
		{
			//SP_ViewCrop	  = 0,			// 观察crop
			//SP_ViewCropRemap,			// Remap
			SP_Direction=0,				// 朝向
			SP_Color,					// 颜色
			//SP_DepthMap,				// 深度图
		};

	public:
		// 构造函数
		DirectionalLight();

		// 获取朝向
		virtual const Vector3& GetDirection() const { return m_direction; }

		// 设置朝向
		virtual void SetDirection( const Vector3& dir);

		// 获取shader绑定参数
		ShaderParamPtr GetSPAB( SPType type) const { return m_spab[type]; }

		// 设置使用LightShaft
		virtual void SetLightShaft( const char* fxPath);

		// 设置主光源
		void SetDomaintLight( bool isDomaint) { m_domaint = isDomaint; }

		// 获取Shaft中心坐标
		virtual float2 GetShaftCenterCoord();

		// 更新
		virtual void  FrameMove( float fTime);

		// 是否使用Shaft
		virtual bool IsUseLightShaft();

		// 获取Shaft
		virtual FxEntityPtr GetLightShaft() { return m_shaft; }	

		// 获取Shaft路径
		const string& GetShaftPath() const { return m_shaftPath; }

		// 获取层级阴影管理器
		CSMMgrPtr& GetCSMMgr() { return m_csmMgr; }

		// 提交渲染元素
		virtual void SubmitRenderElements( RenderElementMgr& renderElementMgr);

	private:
		// 更新SPAB
		void UpdateSPAB();

		// 更新Shaft位置 
		void UpdateShaftPos();

	private:
		bool			m_domaint;					// 是否为主光源
		float			m_distance;					// 向后搜索距离
		Vector3			m_pos;						// 临时位置
		Box3			m_searchBox3;				// 搜寻轴向包围盒
		Vector3			m_direction;				// 朝向	
		size_t			m_resolution;				// 分辨率
		TexturePtr		m_depthTextureRT;
		TexturePtr		m_depthTexture;				// 深度图
		FrameBufferPtr	m_depthBuffer;				// 深度buffer
		Matrix44		m_view;						// 观察矩阵
		Matrix44		m_crop;						// 自适应
		Matrix44		m_remap;					// Remap

		boost::array<ShaderParamPtr,5>	m_spab;				// ShaderParamAutoBind

		string			m_shaftPath;				// 资源路径
		FxEntityPtr		m_shaft;					// LightShaft
		CSMMgrPtr		m_csmMgr;					// 层级阴影管理器
	};
	typedef s_ptr<DirectionalLight> DirectionalLightPtr;
}