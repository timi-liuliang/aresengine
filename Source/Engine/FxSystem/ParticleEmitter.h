//#ifndef ARES_PARTICLE_H
//#define ARES_PARTICLE_H
//
//#include <vector>
//#include <list>
//#include "AresEffect.h"
//#include "Sprite.h"
//
//using namespace std;
//
//namespace Ares
//{
//	#define D3DFVF_PARTICLE (D3DFVF_XYZ | D3DFVF_PSIZE| D3DFVF_DIFFUSE) 
//
//	struct VERTEX_PARTICLE
//	{
//		Vector3     vPosition;  // 粒子位置	
//		float       pointSize;	// 位置和尺寸决定了绘理坐标
//		DWORD       color;		// 粒子颜色
//	};
//
//	//-----------------------------------------
//	// 粒子发射器 CParticle的目类(使用者)
//	//-----------------------------------------
//	class  ITexture;
//	class  IRenderMethod;
//	struct SRenderEntry;
//	class  CParticleEmitter : public IParticleEmitter
//	{
//	public:
//		// 构造函数
//		CParticleEmitter();
//
//		// 析构函数
//		~CParticleEmitter();
//
//		// 初始化
//		virtual bool Initialize(char* cTextureFileName);
//
//		// 每帧更新 
//		virtual void FrameMove(float fTime);
//
//		// 提交到渲染队列( 使用特效 队列索引)
//		virtual void SubmitToRenderQueue( const char* searchName);
//
//		// 渲染
//		virtual void Render(SRenderEntry** entry, int iSameEntry, U32Flags activateionFlags);
//
//	private:
//		UINT                     m_iMaxSprites;		  // 场景中最大点精灵数	
//		float					 m_fEmitRateMin;      // 单位粒子/秒
//		float					 m_fEmitRateMax;      // 
//
//        CPtr<IVertexBuffer>      m_pVertexBuffer;     // 顶点缓冲
//		CPtr<ITexture>   	     m_pTexture;	      // 粒子纹理
//		vector<SSPrite>          m_Particles;         // 粒子数组
//		list<int>				 m_idleParticleIndex; // 记录不活动的Particle下标
//		CPtr<ISurfaceMaterial>	 m_pMaterial;		  // 材质 
//		VERTEX_PARTICLE*         m_pVertexData;		  // 内存中存储数据	
//	};
//}
//
//#endif