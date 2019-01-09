#pragma once

#include <vector>
#include <Core/AresCore.h>
#include <Physics/Shapes.h>

using namespace std;

namespace Ares
{
	//--------------------------------------
	// 层级阴影管理器 2013-2-22 帝林
	//--------------------------------------
	class Scene;
	class CSMMgr
	{
	public:
		// shader param type
		enum SPType
		{
			SP_SplitPos	  = 0,			// 观察crop
			SP_ViewCropRemap,			// Remap
			SP_DepthMap,				// 深度图
			SP_NumSplits,
			SP_Total,
		};

	public:
		CSMMgr();
		~CSMMgr();

		// 设置关联场景
		void SetScene( Scene* scene);

		// 设置太阳光朝向
		void SetLightDirection( const Vector3& dir);

		// 设置深度纹理分辨率
		void SetShadowMapResolution( size_t resolution=0);

		// 开始阴影渲染图阶段
		void RefreshShadowMaps( const Rect3& rect);

		// 开始阴影渲染图阶段
		void RefreshShadowMaps( Frustum3& frustum, size_t numSplits);

		// 获取shader绑定参数
		ShaderParamPtr GetSPAB( SPType type) const { return m_spab[type]; }

		// 获取深度纹理
		TexturePtr GetDepthTexture() { return m_depthTexture; }

	private:
		// 重新分配资源
		void ReAllocateRes();

		// 设置阴影接收者投身者区域
		void SetFocusRegion( const Rect3& rect);

		// 设置阴影接收者投身者区域(用于动态平行分隔阴影)
		void SetFocusRegion( const Frustum3& frustum, size_t numSplits);

		// 设置阴影接收者区域(点集)
		void SetFocusRegion( const vector<Vector3>& points, size_t index);

		// 计算自适应矩阵
		void CalculateCropMatrix( Matrix44& crop, const vector<Vector3>& points, const Matrix44& view);

		// 更新搜寻轴向包围盒
		void CalculateSearchBox3( Box3& searchBox, const vector<Vector3>& points, const Vector3& pos);

		// 更新SPAB
		void UpdateSPAB();

	private:
		Scene*					m_scene;			// 关联场景
		Vector3					m_dir;				// 光朝向
		size_t					m_resolution;		// 光照图分辨率
		size_t					m_numSplits;		// 分隔数
		Matrix44				m_remap;			// 偏移
		vector<Matrix44>		m_views;			// 观察矩阵
		vector<Matrix44>		m_crops;			// 自适应矩阵
		TexturePtr				m_depthTexture;		// 深度纹理
		vector<FrameBufferPtr>	m_depthBuffers;		// 深度缓冲
		vector<Box3>			m_searchBox3s;		// 搜寻包围盒
		float					m_splitSchemeWeight;// 分隔权重
		vector<float4>			m_splitPos;			// 分隔位置

		boost::array<ShaderParamPtr,SP_Total>	m_spab;	// ShaderParamAutoBind
	};
	typedef s_ptr<CSMMgr> CSMMgrPtr;
}