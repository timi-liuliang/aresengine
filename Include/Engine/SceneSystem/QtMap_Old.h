#include "AresMap.h"
#include <string>
#include <vector>
#include "Engine/RenderSystem/AresRenderSystem.h"

using namespace std;

namespace Ares
{
	//------------------------------------
	// Quadtree地图 2011-08-16 帝林
	//------------------------------------
	class CQtMap : public IMap
	{
		typedef vector<IArea*> AreaList;

	public:
		// 构造函数
		CQtMap();

		// 析构函数
		~CQtMap();

	public:
		// 设置Window(中心点x,y; 半径radius)
		//virtual void SetNavWindow( float x, float y, float radius);

		// 更新
		//virtual void FrameMove( float fTime);

		// 渲染
		virtual void Render();

		// 渲染
		void RenderScene();

		// 渲染阴影贴图
		void RenderShadowmap();

		// 获取阴影投射体包围盒
		void SetShadowCastBoundingBox();

	public:
		// 获取当前加载进度
		//virtual float GetLoadingProgress();

		// 获取包围盒
		//virtual const Rect3& GetBoundingBox();

		// 获取包围盒
		//virtual bool GetBoundingBoxByLayer( Rect3& box, const char* layer, bool update=false);

	public:
		// 获取地图名
		//virtual const char* GetName();

		// 获取区域数量
		//virtual size_t GetAreaNum() { return m_areaList.size(); }

		// 获取区域
		//virtual IArea* GetArea( size_t idx) { return m_areaList[idx]; }

		// 获取区域
		//virtual IArea*	GetArea( const char* areaName);

	public:
		// 设置地图名
		//virtual void SetName( const char* name);

		// 设置位置
		//virtual void SetLocation( const char* location){ m_location = location; }

		// 添加区域
		//virtual bool AddArea( IArea* pArea);

	private:
		// 搜寻场景,使可见Renderable分组
		void SearchRenderable( const string& search, const Rect3& searchRect, const Frustum3* pOptionalFrustum);

		// 整理可见实体
		//void SearchAndTidy( const Rect3& searchRect, const Frustum3* pOptionalFrustum);

	public:
		// 加载
		virtual void Load( const char* fileName);

		// 保存
		virtual void Save( const char* fileName=0);

	public:
		// 加载版本
		void Load0_1( FILE* fileHandle, const char* fileName);

	private:
		int				m_version;			// 当前版本
		string			m_name;				// 地图名
		string			m_location;			// 位置
		AreaList		m_areaList;			// 区域列表
		SNavWindow		m_navWindow;		// 导航窗口
		RenderQueueMgr	m_renderQueueMgr;	// 渲染队列管理器
	};
}