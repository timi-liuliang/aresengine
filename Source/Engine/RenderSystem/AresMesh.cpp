#include <Engine/RenderSystem/AresMesh.h>
#include <Engine/AresRoot.h>

#define MVF_Position	EF_BGR32F			// 位置格式
#define MVF_DiffuseUV	EF_GR32F			// 漫反射纹理坐标格式
#define MVF_LightmapUV	EF_GR32F			// 光照图UV格式
#define MVF_Normal		EF_BGR32F
#define MVF_Tangent		EF_BGR32F
#define MVF_Binormal	EF_BGR32F
#define MVF_Color		EF_BGR32F
#define MVF_Blendweight	EF_ABGR32F
#define MVF_BlendIdx	EF_ABGR32F

namespace Ares
{
	// 构建Mesh
	void Mesh::IDCard::ReBuildMesh( Mesh& mesh)
	{
		// 记录信息卡
		mesh.m_idcard = IDCardPtr( this);

		// 初始化骨骼数据
		if ( !m_boneInfluence.empty())
		{
			// 3.创建绑定顶点缓冲区
			ElementInitData initVertexData;
			initVertexData.m_rowPitch   = sizeof( BoneInfluence) * m_positions.size();
			initVertexData.m_slicePitch = 0;
			initVertexData.m_data		= m_boneInfluence.data();
			GraphicBufferPtr boneInfluenceBuffer = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
			mesh.m_vertexBuffers[VSF_BoneInfulence] = boneInfluenceBuffer;
		}

		// 构建顶点缓冲
		// VSF_Position
		if( m_positions.size())
		{
			ElementInitData initVertexData;
			initVertexData.m_rowPitch     = NumFormatBytes( MVF_Position) * m_positions.size();
			initVertexData.m_slicePitch   = 0;
			initVertexData.m_data		  = m_positions.data();
			GraphicBufferPtr bufferStream = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
			mesh.m_vertexBuffers[VSF_Position] = bufferStream;
		}

		// VSF_DiffuseUV
		if ( m_uvsDiffuse.size())
		{
			ElementInitData initVertexData;
			initVertexData.m_rowPitch     = NumFormatBytes( MVF_DiffuseUV) * m_uvsDiffuse.size();
			initVertexData.m_slicePitch   = 0;
			initVertexData.m_data		  = m_uvsDiffuse.data();
			GraphicBufferPtr bufferStream = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
			mesh.m_vertexBuffers[VSF_DiffuseUV]= bufferStream;
		}

		// VSF_LightmapUV
		if( m_uvsLightmap.size())
		{
			ElementInitData initVertexData;
			initVertexData.m_rowPitch	  = NumFormatBytes( MVF_LightmapUV) * m_uvsLightmap.size();
			initVertexData.m_slicePitch	  = 0;
			initVertexData.m_data		  = m_uvsLightmap.data();
			GraphicBufferPtr bufferStream = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
			mesh.m_vertexBuffers[VSF_LightmapUV] = bufferStream;
		}

		// VSF_Normal
		if( m_normals.size())
		{
			ElementInitData initVertexData;
			initVertexData.m_rowPitch     = NumFormatBytes( MVF_Normal) * m_normals.size();
			initVertexData.m_slicePitch   = 0;
			initVertexData.m_data		  = m_normals.data();
			GraphicBufferPtr bufferStream = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
			mesh.m_vertexBuffers[VSF_Normal]   = bufferStream;
		}

		// VSF_Color
		if( m_colors.size())
		{
			ElementInitData initVertexData;
			initVertexData.m_rowPitch     = NumFormatBytes( MVF_Color) * m_colors.size();
			initVertexData.m_slicePitch   = 0;
			initVertexData.m_data		  = m_colors.data();
			GraphicBufferPtr bufferStream = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
			mesh.m_vertexBuffers[VSF_Color]    = bufferStream;
		}

		// VSF_BoneInfulence
		if ( m_boneInfluence.size())
		{
			ElementInitData initVertexData;
			initVertexData.m_rowPitch     = sizeof( BoneInfluence) * m_boneInfluence.size();
			initVertexData.m_slicePitch   = 0;
			initVertexData.m_data		  = m_boneInfluence.data();
			GraphicBufferPtr bufferStream = ARenderDevice->CreateVertexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initVertexData);
			mesh.m_vertexBuffers[VSF_BoneInfulence] = bufferStream;
		}

		// 索引数据
		if( m_faces.size())
		{
			ElementInitData initIndexData;
			initIndexData.m_rowPitch   = m_faces.size() * sizeof( Int3);
			initIndexData.m_slicePitch = 0;
			initIndexData.m_data	   = m_faces.data();
			mesh.m_indexBuffer		   = ARenderDevice->CreateIndexBuffer( BU_Static, EAH_GPURead | EAH_Immutable, &initIndexData);
		}
	}

	// 构造函数
	Mesh::Mesh()
	{}

	// 获取RenderLayout
	GraphicBufferPtr Mesh::GetVertexGraphicsBuffer( VertexStreamFlag flag) 
	{ 
		VertexBufferMap::const_iterator it = m_vertexBuffers.find( flag);
		if( it != m_vertexBuffers.end())
			return it->second;

		return GraphicBufferPtr(); 
	}

	// 获取索引缓冲
	GraphicBufferPtr Mesh::GetIndexGraphicsBuffer( int& startIndex, int& primitiveCount)
	{
		startIndex     = 0;
		primitiveCount = this->GetTriNum();

		return m_indexBuffer;
	}

	// 映射VertexElementType
	VertexElementType Mesh::Mapping( VertexStreamFlag vertexStreamFlag)
	{
		VertexElementType result;

		switch( vertexStreamFlag)
		{
		case VSF_Position:
			{
				result.push_back( VertexElement(VEU_Position,     0, MVF_Position));
			}
			break;

		case VSF_DiffuseUV:
			{
				result.push_back( VertexElement(VEU_TextureCoord, 0, MVF_DiffuseUV));
			}
			break;

		case VSF_LightmapUV:
			{
				result.push_back( VertexElement(VEU_TextureCoord, 1, MVF_LightmapUV));
			}
			break;

		case VSF_Normal:
			{
				result.push_back( VertexElement(VEU_Normal,	      0, MVF_Normal));
			}
			break;

		case VSF_TangentBinormal:
			{
				result.push_back( VertexElement(VEU_Tangent,      0, MVF_Tangent));
				result.push_back( VertexElement(VEU_Binormal,     0, MVF_Binormal));
			}
			break;

		case VSF_Color:
			{
				result.push_back( VertexElement(VEU_Color,	   0, MVF_Color));
			}
			break;

		case VSF_BoneInfulence:
			{
				result.push_back( VertexElement(VEU_BlendWeight,  0, MVF_Blendweight));
				result.push_back( VertexElement(VEU_BlendIndex,   0, MVF_BlendIdx));
			}
			break;
		}

		return result;
	}

	static boost::array<Mesh::VertexStreamFlag, 7> vertexStreamFlags =
	{
		Mesh::VSF_Position,
		Mesh::VSF_DiffuseUV,
		Mesh::VSF_LightmapUV,
		Mesh::VSF_Normal,
		Mesh::VSF_TangentBinormal,
		Mesh::VSF_Color,
		Mesh::VSF_BoneInfulence,
	};

	// 构建RenderLayout flyweight
	RenderLayoutPtr Mesh::BuildRenderLayout( UINT vertexDataFlag)
	{
		// 1.判断Renderlayout是否已存在
		RenderLayoutPtr result = ARenderDevice->CreateRenderLayout();
		result->SetTopologyType( RenderLayout::TT_TriangleList);

		// 绑定顶点缓冲
		for ( size_t i=0; i<vertexStreamFlags.size(); i++)
		{
			if( vertexDataFlag & vertexStreamFlags[i])
			{
				GraphicBufferPtr bufPtr = GetVertexGraphicsBuffer( vertexStreamFlags[i]);
				if( bufPtr)
					result->BindVertexStream( bufPtr, Mesh::Mapping( vertexStreamFlags[i]));
			}
		}

		// 绑定索引缓冲
		{
			int startIdx, primitiveCount;
			GraphicBufferPtr idxBufPtr = GetIndexGraphicsBuffer( startIdx, primitiveCount);
			if( idxBufPtr)
			{
				result->BindIndexStream( idxBufPtr, EF_R32UI);

				result->SetStartIndexLocation( startIdx);
				result->SetNumIndices( primitiveCount*3);
			}
		}

		return result;
	}
}

BOOST_CLASS_EXPORT( Ares::Mesh::IDCard)