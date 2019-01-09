#pragma once

#include <vector>
#include <map>
#include <Core/Graphics/AresRenderLayout.h>
#include <Core/Graphics/AresRenderMaterial.h>

using namespace std;

namespace Ares
{
	//-------------------------------------------------------------
	// 渲染队列 2012-6-21 帝林
	// 1.队列与材质息息相关(渲染的物体才需要队列,而渲染物必有材质)
	// 2.通过队列标记组合控制实体在多个阶段被渲染
	//-------------------------------------------------------------
	enum RenderQueueFlag
	{
		RQ_Sky					= 1<<0,	// Sky
		RQ_Opacity				= 1<<1,	// Opacity
		RQ_Terrain				= 1<<2,	// Terrain
		RQ_Water				= 1<<3,
		RQ_TransparentEffect	= 1<<4,	// Transparent Effect object
		RQ_SpaceDistorted		= 1<<5, // Space Distorted
		RQ_GodRay				= 1<<6,	// God ray
		RQ_AuxiliaryDisplay		= 1<<7,	// Auxiliary display
		RQ_UI					= 1<<8,	// UI display
		RQ_UserDefined_9		= 1<<9,
		RQ_UserDefined_10		= 1<<10,
		RQ_UserDefined_11		= 1<<11,
		RQ_UserDefined_12		= 1<<12,
		RQ_UserDefined_13		= 1<<13,
		RQ_UserDefined_14		= 1<<14,
		RQ_UserDefined_15		= 1<<15,
		RQ_UserDefined_16		= 1<<16,
		RQ_UserDefined_17		= 1<<17,
		RQ_UserDefined_18		= 1<<18,
		RQ_UserDefined_19		= 1<<19,
		RQ_UserDefined_20		= 1<<20,
		RQ_UserDefined_21		= 1<<21,
		RQ_UserDefined_22		= 1<<22,
		RQ_UserDefined_23		= 1<<23,
		RQ_UserDefined_24		= 1<<24,
		RQ_UserDefined_25		= 1<<25,
		RQ_UserDefined_26		= 1<<26,
		RQ_UserDefined_27		= 1<<27,
		RQ_UserDefined_28		= 1<<28,
		RQ_UserDefined_29		= 1<<29,
		RQ_UserDefined_30		= 1<<30,
		RQ_UserDefined_31		= 1<<31,
		RQ_Max					= 32,				
	};

	//-----------------------------------------------------------------
	// RenderQueue 2010-04-02 18:04 -- 未知    帝林
	//-----------------------------------------------------------------
	class RenderQueue
	{
		typedef pair<RenderLayoutPtr, RenderMaterial*> Renderable;
	public:	
		RenderQueue();
		~RenderQueue();

		// render
		void Render( bool sort=true, bool clear=true);

		// add renderable
		void AddRenderable( RenderLayoutPtr& layout, RenderMaterial* material);

		// Sore for render accelerate
		void Sort();

		// clear renderable list
		void Reset() { m_renderables.clear(); }

		// get size
		size_t GetSize() { return m_renderables.size(); }

		// is empty
		bool IsEmpty() const { return m_renderables.empty(); }

	private:
		vector<Renderable>	m_renderables;
	};
}