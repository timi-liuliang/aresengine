//#pragma once
//
//namespace Ares
//{
//	class CBufferMgr
//	{
//	public:
//		// 渲染用顶点格式声明
//		struct SPRITE_VERTEX
//		{
//			Vector3 m_pos;		// 粒子位置	
//			float   m_size;		// 位置和尺寸决定了绘理坐标
//			DWORD   m_color;	// 粒子颜色
//		};
//
//		// 获取点精灵Buffer
//		static SPtr<IVertexBuffer>& GetSpriteBuffer();
//
//		// 获取点精灵用格式
//		static SPtr<IVertexDeclaration>& GetSpritieDeclaration();
//
//	private:
//		static SPtr<IVertexBuffer>      m_spriteBuffer;		// 点精灵用Buffer
//		static SPtr<IVertexDeclaration> m_spriteDeclaration;// 点精灵格式声明
//	};
//
//	#define SPRITE_VERTEX  CBufferMgr::SPRITE_VERTEX
//}