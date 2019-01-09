#include <Engine/RenderSystem/AresClothFabricBuilder.h>

namespace Ares
{
	// 构造函数
	GridClothBuilder::GridClothBuilder( ClothFabric& cloth, int width, int height, const Transform& transform)
	{
		m_width = width;
		m_height= height;

		// 初始化质点
		m_massPoints.resize( width*height);

		// Step1 : 添加结构性弹簧(h-1,w) (h+1,w) (h,w+1) (h,w-1)
		for ( int h=0; h<m_height; h++)
		{
			for ( int w=0; w<m_width; w++)
			{
				// 当前质点
				int curIdx;
				A_ASSERT( GetMassIndex( curIdx, w, h));

				// 初始位置
				m_massPoints[curIdx].m_position = Vector3( static_cast<float>(w), static_cast<float>(h), 0.f) * 0.1f;
				m_massPoints[curIdx].m_position = transform.TransformVector3( m_massPoints[curIdx].m_position);
				m_massPoints[curIdx].m_uv		= float2( (float)w/((float)m_width-1), (float)h/((float)m_height-1));

				// 结构性弹簧链接质点
				int linkIdx;
				Int2 stlinks[4] = { Int2( w, h-1), Int2( w, h+1), Int2( w-1, h), Int2( w+1, h)};
				for ( int i=0; i<4; i++)
				{
					if( GetMassIndex( linkIdx, stlinks[i].x, stlinks[i].y))
					{
						MassSpringSystem::Spring spring;
						spring.m_type = MassSpringSystem::ST_Structural;
						spring.m_rest = 0.1f;
						spring.m_left = curIdx < linkIdx ? curIdx : linkIdx;
						spring.m_right= curIdx < linkIdx ? linkIdx : curIdx;

						// 添加弹簧
						AddSpring( spring);
					}
				}

				// 剪切性弹簧质点链接
				Int2 shLinks[4] = { Int2( w-1, h-1), Int2( w+1, h+1), Int2( w+1, h-1), Int2( w-1, h+1)};
				for ( int i=0; i<4; i++)
				{
					if( GetMassIndex( linkIdx, shLinks[i].x, shLinks[i].y))
					{
						MassSpringSystem::Spring spring;
						spring.m_type = MassSpringSystem::ST_Shear;
						spring.m_rest = 0.1414f;
						spring.m_left = curIdx > linkIdx ? curIdx : linkIdx;
						spring.m_right= curIdx > linkIdx ? linkIdx : curIdx;

						// 添加弹簧
						AddSpring( spring);
					}
				}

				// 弯曲弹簧质点链接
				Int2 flLinks[4] = { Int2( w-2, h), Int2( w+2, h), Int2( w, h-2), Int2( w, h+2)};
				for ( int i=0; i<4; i++)
				{
					if( GetMassIndex( linkIdx, flLinks[i].x, flLinks[i].y))
					{
						MassSpringSystem::Spring spring;
						spring.m_type = MassSpringSystem::ST_Flexion;
						spring.m_rest = 0.2f;
						spring.m_left = curIdx > linkIdx ? curIdx : linkIdx;
						spring.m_right= curIdx > linkIdx ? linkIdx : curIdx;

						// 添加弹簧
						AddSpring( spring);
					}
				}
			}
		}

		// 构建索引
		for ( int h=0; h<m_height-1; h++)
		{
			for ( int w=0; w<m_width-1; w++)
			{
				int luIdx, ruIdx, ldIdx, rdIdx;
				GetMassIndex( luIdx,   w,   h);
				GetMassIndex( ruIdx, w+1,   h);
				GetMassIndex( ldIdx,   w, h+1);
				GetMassIndex( rdIdx, w+1, h+1);

				m_triIndices.push_back( luIdx);
				m_triIndices.push_back( ruIdx);
				m_triIndices.push_back( rdIdx);

				m_triIndices.push_back( luIdx);
				m_triIndices.push_back( rdIdx);
				m_triIndices.push_back( ldIdx);
			}
		}

		// 设置Fabric
		cloth.GetMassSpringSystem().Init( m_triIndices, m_massPoints, m_springs);

		//------------------------------ 测试用
		cloth.GetMassSpringSystem().GetMassPoint( 0).m_archor = true;
		cloth.GetMassSpringSystem().GetMassPoint( 5).m_archor = true;
		cloth.GetMassSpringSystem().GetMassPoint( 10).m_archor = true;
		cloth.GetMassSpringSystem().GetMassPoint( 15).m_archor = true;
		cloth.GetMassSpringSystem().GetMassPoint( 20).m_archor = true;
		cloth.GetMassSpringSystem().GetMassPoint( 25).m_archor = true;
		cloth.GetMassSpringSystem().GetMassPoint( 30).m_archor = true;

		//--------------------------------------------------
	}

	// 辅助函数,获取索引
	bool GridClothBuilder::GetMassIndex( int& index, int w, int h)
	{
		if( w>=0 && w<m_width && h>=0 && h<m_height)
		{
			index = h * m_width + w;

			return true;
		}

		return false;
	}

	// 添加弹簧
	void GridClothBuilder::AddSpring( MassSpringSystem::Spring& spring)
	{
		// 是否已存在
		for ( size_t i=0; i<m_springs.size(); i++)
		{
			if( m_springs[i].m_left==spring.m_left && m_springs[i].m_right==spring.m_right)
				return;
		}

		m_massPoints[spring.m_left].m_springs[m_massPoints[spring.m_left].m_numSpring]   = m_springs.size();					
		m_massPoints[spring.m_right].m_springs[m_massPoints[spring.m_right].m_numSpring] = m_springs.size();
		m_massPoints[spring.m_left].m_numSpring++;
		m_massPoints[spring.m_right].m_numSpring++;

		m_springs.push_back( spring);
	}
}