#include <Core/AresFile.h>
#include <boost/format.hpp>
#include <Engine/AresRoot.h>
#include <Engine/RenderSystem/AresTextureSet.h>

namespace Ares
{
	// 构造函数
	TextureSet::Node::Node()
		: m_id( INVALID)
	{
		m_child[0] = INVALID;
		m_child[1] = INVALID;
	}

	// 是否为叶结点
	bool TextureSet::Node::IsLeaf() const
	{ 
		return m_child[0]==INVALID && m_child[1]==INVALID; 
	}

	// 构造函数
	TextureSet::TextureSet()
	{

	}

	// 构造函数
	TextureSet::TextureSet( TexturePtr& texture)
	{
		Init( texture);

		// 插入默认主节点
		Node rootNode;
		rootNode.m_rc = IRect( 0, 0, m_width, m_height);
		m_nodes.reserve( 256);
		m_nodes.push_back( rootNode);
	}

	// 初始化
	void TextureSet::Init( TexturePtr& texture)
	{
		m_texture = texture;

		m_width  = static_cast<int>(texture->GetWidth(0));
		m_height = static_cast<int>(texture->GetHeight(0));
	}

	// 获取块纹理Viewport
	const Vector4 TextureSet::GetViewport( int nodeIdx) const
	{
		A_ASSERT( nodeIdx>=0 && nodeIdx<static_cast<int>(m_nodes.size()));

		Vector4	result;
		const IRect& tRc   = m_nodes[nodeIdx].m_rc;
		result.x = static_cast<float>(tRc.left)  / static_cast<float>(m_width);
		result.y = static_cast<float>(tRc.top)   / static_cast<float>(m_height);
		result.z = static_cast<float>(tRc.width) / static_cast<float>(m_width);
		result.w = static_cast<float>(tRc.height)/ static_cast<float>(m_height);

		return result;
	}

	// 插入函数
	int TextureSet::Insert( TexturePtr& subTex)
	{
		if( !subTex)
			return INVALID;

		int nodeIdx = Insert( 0, subTex);

		return OverWrite( nodeIdx, subTex);
	}

	// 覆写纹理
	int TextureSet::OverWrite( int nodeIdx, TexturePtr& subTex)
	{
		if( nodeIdx!=INVALID)
		{
			m_nodes[nodeIdx].m_id = nodeIdx;

			// copy pixels over from texture to pNode->m_rc part of texture
			const TRect<int>& rc = m_nodes[nodeIdx].m_rc;
			subTex->CopyToSubTexture2D( *m_texture, 0, 0, rc.left, rc.top, rc.width, rc.height, 0, 0, 0, 0, subTex->GetWidth(0), subTex->GetHeight(0));

			return nodeIdx;
		}

		return INVALID;
	}

	// 插入纹理
	int TextureSet::Insert( int nodeIdx, const TexturePtr& subTex)
	{
		if( nodeIdx==INVALID)
			return INVALID;

		// if we're not a leaf then
		if( !m_nodes[nodeIdx].IsLeaf())
		{
			// 优先插入面积较小的块
			int child0Idx = m_nodes[nodeIdx].m_child[0];
			int child1Idx = m_nodes[nodeIdx].m_child[1];
			if( child0Idx!=INVALID && child1Idx!=INVALID)
			{
				if( m_nodes[child0Idx].m_rc.GetArea()>m_nodes[child1Idx].m_rc.GetArea())
					std::swap( child0Idx, child1Idx);
			}

			// try inserting into first child
			int newIdx = Insert( child0Idx, subTex);
			if( newIdx != INVALID)
				return newIdx;

			// no room, insert into second
			return Insert( child1Idx, subTex);
		}
		else
		{
			// if there is already a lightmap here, return
			if( m_nodes[nodeIdx].m_id != INVALID)
				return INVALID;

			int texWidth  = static_cast<int>(subTex->GetWidth( 0));
			int texHeight = static_cast<int>(subTex->GetHeight( 0));

			// if we're too small, return
			if( m_nodes[nodeIdx].m_rc.width<texWidth || m_nodes[nodeIdx].m_rc.height<texHeight)
				return INVALID;

			// if we'rs just right accept, "no surplus space"
			if( m_nodes[nodeIdx].m_rc.width==texWidth && m_nodes[nodeIdx].m_rc.height==texHeight)
				return nodeIdx;

			// otherwise, gotta split this node and create some kids
			Node node0, node1;

			// deside split pattern
			int dw = m_nodes[nodeIdx].m_rc.width  - texWidth;
			int dh = m_nodes[nodeIdx].m_rc.height - texHeight;

			const IRect& nRect = m_nodes[nodeIdx].m_rc;
			if( dw>dh)
			{
				node0.m_rc = IRect( nRect.left+texWidth, nRect.top, nRect.width-texWidth, nRect.height);
				node1.m_rc = IRect( nRect.left, nRect.top+texHeight, texWidth, nRect.height-texHeight);
			}
			else
			{
				node0.m_rc = IRect( nRect.left+texWidth, nRect.top, nRect.width-texWidth, texHeight);
				node1.m_rc = IRect( nRect.left, nRect.top+texHeight, nRect.width, nRect.height-texHeight);
			}

			// 添加有省余空间的子结点
			if( node0.m_rc.GetArea() > 0)
			{
				m_nodes[nodeIdx].m_child[0] = m_nodes.size();
				m_nodes.push_back( node0);
			}
			if( node1.m_rc.GetArea() > 0)
			{
				m_nodes[nodeIdx].m_child[1] = m_nodes.size();
				m_nodes.push_back( node1);
			}

			m_nodes[nodeIdx].m_rc.width = texWidth;
			m_nodes[nodeIdx].m_rc.height= texHeight;

			return nodeIdx;
		}
	}

	// 加载
	void TextureSet::Load( const char* filePath)
	{
		// 加载纹理
		string tePath;
		FileUtils::ReplaceExt( tePath, filePath, ".dds");

	#ifdef ARES_EDITOR_MODE
        TexturePtr tePtr = AResSystem.SyncLoadDDS( tePath.c_str(), EAH_GPURead | EAH_CPUWrite);
		Init( tePtr);
	#else
        TexturePtr tePtr = AResSystem.SyncLoadDDS( tePath.c_str(), EAH_GPURead | EAH_Immutable);
		Init( tePtr);
	#endif

		// 加载结点信息
		FILE* fileHandle = fopen( filePath, "rb");
		if( fileHandle)
		{
			// 保存上方向索引
			size_t numNode = 0;
			fread( &numNode, sizeof(numNode), 1, fileHandle);

			// 保存数据大小
			m_nodes.resize( numNode);
			fread( m_nodes.data(), sizeof( Node)*numNode, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);
		}
	}

	// 保存
	void TextureSet::Save( const char* filePath)
	{
		// 保存纹理
		string		  tePath;
		FileUtils::ReplaceExt( tePath, filePath, ".dds");
		AResSystem.SyncSaveTexture( m_texture, tePath.c_str());

		// 保存结点信息
		FILE* fileHandle = fopen( filePath, "wb");
		if( fileHandle)
		{
			// 保存上方向索引
			size_t numNode = m_nodes.size();
			fwrite( &numNode, sizeof(numNode), 1, fileHandle);

			// 保存数据大小
			fwrite( m_nodes.data(), sizeof( Node)*numNode, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);
		}
	}
}