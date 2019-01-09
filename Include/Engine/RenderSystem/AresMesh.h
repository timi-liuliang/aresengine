#pragma once

#include <Core/AresCore.h>
#include <Physics/Shapes.h>

using namespace std;

namespace Ares
{
	//---------------------------------------
	// Mesh 2010-09-21   帝林
	//---------------------------------------
	class Mesh
	{	
	public:
		// VertexStreamFlag
		enum VertexStreamFlag
		{
			VSF_Position		= 1<<0,		// POSITION0	位置
			VSF_DiffuseUV		= 1<<1,		// TEXCOORD0	漫反射贴图uv
			VSF_LightmapUV		= 1<<2,		// TEXCOORD1	光照图uv
			VSF_Normal			= 1<<3,		// NORMAL0		法线
			VSF_TangentBinormal	= 1<<4,		// TANGENT0		切线	 BINORMAL0	副线		
			VSF_Color			= 1<<5,		// COLOR0		顶点色
			VSF_BoneInfulence	= 1<<6,		// BLENDWEIGHT0	骨骼权重 BLENDINDICES0骨骼索引
			VSF_Total			= 7,	
		};
		typedef map<VertexStreamFlag, GraphicBufferPtr> VertexBufferMap;

		// BoneInfluence :vertex bone weights
		struct BoneInfluence
		{		
			Vector4    m_weights;
			Vector4    m_indexs;

			// 序列化
			template <typename Archive> void serialize( Archive& ar, const unsigned int version)
			{
				ar & m_weights & m_indexs;
			}
		};

		// IDCard
		struct IDCard
		{
			string			m_name;				// 名称
			float			m_lightmapWidth;	// 光照图宽
			float			m_lightmapHeight;	// 光照图高
			Rect3			m_boundingBox;		// 包围盒

			vector<Vector3>	m_positions;		// 位置数据
			vector<Vector3>	m_normals;			// 法线数据
			vector<DWORD>	m_colors;			// 颜色数据
			vector<Vector2> m_uvsDiffuse;		// 漫反射纹理坐标
			vector<Vector2> m_uvsLightmap;		// 光照图纹理坐标
			vector<int>		m_boneIdxs;			// 骨骼索引
			vector<BoneInfluence>m_boneInfluence;// 骨骼索引权重
			vector<Int3>	m_faces;			// 面数据

			// Constructor
			IDCard()
				: m_lightmapWidth( 0)
				, m_lightmapHeight( 0)
			{}

			// 构建Mesh
			void ReBuildMesh( Mesh& mesh);

			// 序列化
			template <typename Archive> void serialize( Archive& ar, const unsigned int version)
			{
				ar & m_name;
				ar & m_lightmapWidth;
				ar & m_lightmapHeight;
				ar & m_boundingBox;

				ar & m_positions;
				ar & m_normals;
				ar & m_colors;
				ar & m_uvsDiffuse;
				ar & m_uvsLightmap;
				ar & m_boneIdxs;
				ar & m_boneInfluence;
				ar & m_faces;
			}
		};
		typedef s_ptr<IDCard> IDCardPtr;
		friend  struct IDCard;

	public:
		Mesh(); 

		// 构建RenderLayout flyweight
		RenderLayoutPtr BuildRenderLayout( UINT vertexDataFlag);

		// 获取顶点缓冲
		GraphicBufferPtr GetVertexGraphicsBuffer( VertexStreamFlag flag);

		// 获取索引缓冲
		GraphicBufferPtr GetIndexGraphicsBuffer( int& startIndex, int& primitiveCount);

		// 获取包围盒
		const Rect3& GetBoundingBox() const { return m_idcard->m_boundingBox; }

		// 获取骨骼索引
		const vector<int>& GetBoneIdxs() { return m_idcard->m_boneIdxs; }

		// 获取顶点数量
		virtual size_t GetVertexNum() { return m_idcard->m_positions.size(); }

		// 获取三角形数量
		virtual size_t GetTriNum() { return m_idcard->m_faces.size(); }

		// 获取IDCard
		IDCardPtr& GetIDCard() { return m_idcard; }

		// 映射VertexElementType
		static VertexElementType Mapping( VertexStreamFlag vertexStreamFlag);

	private:
		IDCardPtr					m_idcard;				// 标识信息
		VertexBufferMap				m_vertexBuffers;		// 顶点Buffer
		GraphicBufferPtr			m_indexBuffer;			// 索引Buffer
	};
	typedef s_ptr<Mesh> MeshPtr;
}