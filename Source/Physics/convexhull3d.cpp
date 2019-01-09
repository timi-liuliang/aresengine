#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <assert.h>
#include "Physics/ConvexHull3D.h"

using namespace std;

namespace Ares
{
	namespace Incremental
	{
		// ☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
		//-----------------------------------------------------------------
		//  Randomized Incremental Algorithm 
		//	http://www.eecs.tufts.edu/~mhorn01/comp163/algorithm.html
		//-----------------------------------------------------------------
		//-------------------------------------
		// ( 三角形)面 2010-12-02   帝林
		//-------------------------------------
		class CTriangle
		{
		public:
			// 构造函数
			CTriangle( const Vector3* v1, const Vector3* v2,const Vector3* v3, const Vector3* vOrient);

			// 更换邻接三角形
			void ChangeNeighbor( CTriangle* pOld, CTriangle* pNew);

			// 获取邻接三角形
			CTriangle* GetNeighbor( int i){ return m_pNeighbors[i]; }

			// 获取顶点
			const Vector3* GetVertex( int i) { return m_vVertexs[i]; }

			// 是否可见
			bool IsVisible( const Vector3* vertex);

			// 设置邻接三角形
			void SetNeighbor(  CTriangle* neighbor1,  CTriangle* neighbor2,  CTriangle* neighbor3);

		private:
			const Vector3*  m_vVertexs[3];			// 顶点
			Vector3		    m_vNormal;				// 法线
			CTriangle*      m_pNeighbors[3];		// 凸包上三角形必有三个邻接三角形
		};

		// 构造函数,第四个顶点在前三个顶点所构成的三角形内部
		CTriangle::CTriangle( const Vector3* v1, const Vector3* v2,const Vector3* v3, const Vector3* vOrient)
		{
			m_vVertexs[0] = v1;
			m_vVertexs[1] = v2;
			m_vVertexs[2] = v3;

			// 计算法线
			m_vNormal = Vector3Cross( *v2 - *v1, *v3 - *v2);
			m_vNormal.Normalize();

			// 可见
			if( IsVisible( vOrient))
			{
				// 反转法线
				m_vNormal = -m_vNormal;

				const Vector3* tV = m_vVertexs[1];
				m_vVertexs[1] = m_vVertexs[2];
				m_vVertexs[2] = tV;
			}
		}

		// 是否可见
		bool CTriangle::IsVisible( const Vector3* vertex)
		{
			Vector3 centerPoint = ( (*m_vVertexs[0]) + (*m_vVertexs[1]) + (*m_vVertexs[2])) / 3.f;

			return Vector3Dot( *vertex - centerPoint, m_vNormal) >= 0.f;
		}

		// 设置邻接三角形
		void CTriangle::SetNeighbor(  CTriangle* neighbor1,  CTriangle* neighbor2,  CTriangle* neighbor3)
		{
			// neighbor
			m_pNeighbors[0] = neighbor1;
			m_pNeighbors[1] = neighbor2;
			m_pNeighbors[2] = neighbor3;
		}

		// 更换邻接三角形
		void CTriangle::ChangeNeighbor( CTriangle* pOld, CTriangle* pNew)
		{
			for ( int i=0; i<3; i++)
				if( m_pNeighbors[i] == pOld)
				{
					m_pNeighbors[i] = pNew;
					return;
				}
		}

		//------------------------------------
		// 边  2010-12-03   帝林
		//------------------------------------
		struct SEdge
		{
			const Vector3* m_pVertexs[2];		// 两顶点
			CTriangle*     m_pAdjTriNew;		// 新增三角形
			CTriangle*     m_pAdjBack;			// 不可见三角形
			CTriangle*     m_pAdjFront;			// 可见三角形 

			// 构造函数
			SEdge()
			{
				memset( this, 0, sizeof( SEdge));
			}

			// 获取可见三角形的不在此边上的顶点
			const Vector3* GetFrontAdjTriDelVertex()
			{
				for ( int i=0; i<3; i++)
				{				
					if( m_pAdjFront->GetVertex(i) != m_pVertexs[0] && m_pAdjFront->GetVertex(i) != m_pVertexs[1])
						return m_pAdjFront->GetVertex(i);
				}

				assert( false);

				return NULL;
			}
		};

		//-------------------------------------------
		// 增量算法管理器 2010-12-02   帝林
		//-------------------------------------------
		class CIncrementalMgr
		{
		public:
			// 构造函数
			CIncrementalMgr();

			// 析构函数
			~CIncrementalMgr();

			// 通过Incremental Algorithm构建
			bool BuildByIncrementalAlgorithm( const Vector3* const pVertices, size_t iNum);

			// 获取凸包数据
			void GetConvexHull( Vector3*& vertexList, size_t*& idxList, size_t& vertexNum, size_t& triNum);

		private:
			// 1.Construct a tetrahedron by connecting the first four points
			bool Prepare();

			// 步进( 添加顶点)
			void Step();

			// 
			void StepA();

			//
			void StepB();

			//
			void StepC();

			// 获取Horizon Edge
			bool AddHorizonEdge( vector<SEdge>& horizonEdge, CTriangle* visibleTri);

			// 当前是否可见
			bool IsVisibleCur( CTriangle* pTri);

			// 首尾相接排序horizon
			void SortEdges();

			// 根据顶点查找Edge索引
			size_t FindHorizonEdgeIdx( const Vector3* pVertex, size_t cur);

			// 根据顶点获取可见三角形
			void GetVisibleTriangles( const Vector3* pV);

		private:
			size_t				m_iCurIdx;			// 当前处理的顶点索引
			vector<CTriangle*>	m_create;			// List of newly created faces
			vector<SEdge>		m_horizon;			// List of edges on the horizon
			vector<CTriangle*>	m_visible;			// List of faces visible to the current vertex
			set<CTriangle*>	    m_hullTriList;		// 凸包面集
			vector<Vector3>		m_vertexSet;		// 顶点集
			size_t              m_iNum;				// 顶点数
		};

		CIncrementalMgr::CIncrementalMgr()
		{
			m_iCurIdx    = 0;
			m_create.clear();
			m_horizon.clear();
			m_visible.clear();
			m_iNum       = 0;
		}

		// 析构函数
		CIncrementalMgr::~CIncrementalMgr()
		{
			for ( set<CTriangle*>::iterator iter = m_hullTriList.begin(); iter != m_hullTriList.end(); iter++)
			{
				delete (*iter);
			}

			m_hullTriList.clear();
		}
	

		// 通过Incremental Algorithm构建
		bool CIncrementalMgr::BuildByIncrementalAlgorithm( const Vector3* const pVertices, size_t iNum)
		{
			if( iNum < 4)
				return false;

			m_vertexSet.resize( iNum);
			for ( size_t i=0; i<iNum; i++)
				m_vertexSet[i] = pVertices[i];

			//sort( m_vertexSet.begin(), m_vertexSet.end());
			//m_vertexSet.erase( unique( m_vertexSet.begin(), m_vertexSet.end()), m_vertexSet.end());

			m_iNum = m_vertexSet.size();

			// 1.To begin the convex hull algorithm, we create a tetrahedron from
			// the first four vertices in the point cloud.( CW)
			Prepare();

			// 添加其余顶点
			while( m_iCurIdx<m_iNum)
			{
				Step();
			}

			return true;
		}

		// 获取凸包数据
		void CIncrementalMgr::GetConvexHull(  Vector3*& vertexList, size_t*& idxList, size_t& vertexNum, size_t& triNum)
		{
			delete[] vertexList;
			delete[] idxList;

			vertexList = NULL;
			idxList    = NULL;

			// 三角形数量
			triNum = m_hullTriList.size();
			idxList = new_ size_t[triNum * 3];

			int iCurrTri = 0;
			vector<const Vector3*> pVertex;
			for( set<CTriangle*>::iterator iter=m_hullTriList.begin(); iter != m_hullTriList.end(); iter++)
			{
				for ( int j=0; j<3; j++)
				{
					const Vector3* pV = (*iter)->GetVertex(j);

					// 该点是否存在
					size_t k=0;
					for ( ; k<pVertex.size(); k++)
						if ( pVertex[k] == pV)
						{
							idxList[ iCurrTri * 3 + j] = k;

							break;
						}

						// 未找到
						if ( k == pVertex.size())
						{
							pVertex.push_back(pV);
							idxList[ iCurrTri * 3 + j] = k;
						}
				}

				iCurrTri++;
			}

			// 复制顶点数据
			vertexNum = pVertex.size();
			if( vertexNum)
			{
				vertexList = new_ Vector3[ vertexNum];

				for ( size_t i=0; i<pVertex.size(); i++)
				{
					vertexList[i] = *pVertex[i];
				}
			}

		}

		// Construct a tetrahedron by connecting the first four points
		bool CIncrementalMgr::Prepare()
		{
			vector<Vector3>& p = m_vertexSet;

			CTriangle* pTri0 = new_ CTriangle( &p[0], &p[1], &p[2], &p[3]);
			CTriangle* pTri1 = new_ CTriangle( &p[0], &p[2], &p[3], &p[1]);
			CTriangle* pTri2 = new_ CTriangle( &p[0], &p[1], &p[3], &p[2]);
			CTriangle* pTri3 = new_ CTriangle( &p[1], &p[2], &p[3], &p[0]);

			pTri0->SetNeighbor( pTri1, pTri2, pTri3);
			pTri1->SetNeighbor( pTri0, pTri2, pTri3);
			pTri2->SetNeighbor( pTri0, pTri1, pTri3);
			pTri3->SetNeighbor( pTri0, pTri1, pTri2);

			m_hullTriList.insert( pTri0);
			m_hullTriList.insert( pTri1);
			m_hullTriList.insert( pTri2);
			m_hullTriList.insert( pTri3);

			m_iCurIdx = 4;

			return true;
		}

		// 步进( 添加顶点)
		void CIncrementalMgr::Step()
		{
			if( m_create.size() == 0)
			{
				StepA();
				StepB();
			}
			else
			{
				StepC();
			}
		}

		// StepA begins an incremental step of the algorithm
		// Identity the next vertex v  0(1)
		// Identity all triangles visible to v 0(F(v))
		// Find the list of horizon edges for v. O(F(v))
		// refers to the triangles visible to vertex v
		void CIncrementalMgr::StepA()
		{
			if( m_iCurIdx >= m_iNum)
				return;

			// Get list of visible triangle fov v
			const Vector3* v = &m_vertexSet[m_iCurIdx];

			GetVisibleTriangles( v);

			// If v is already inside the convex hull, try the next point
			if( m_visible.size() == 0)
			{
				m_iCurIdx++;
				StepA();

				return;
			}

			// Find horizon edges
			for ( size_t i=0; i<m_visible.size(); i++)
			{
				AddHorizonEdge( m_horizon, m_visible[i]);
			}
		}

		// StepB continues the incremental step by connecting vertex v to
		// each edge of the horizon
		void CIncrementalMgr::StepB()
		{
			if( m_iCurIdx >= m_iNum)
				return;

			const Vector3* v = &m_vertexSet[m_iCurIdx];

			// create new triangles to connect to the horizon O(v)
			// 首尾相接排序horizon
			SortEdges();

			for ( size_t i=0; i<m_horizon.size(); i++)
			{
				CTriangle* pTri = new_ CTriangle( m_horizon[i].m_pVertexs[0], m_horizon[i].m_pVertexs[1], v, m_horizon[i].GetFrontAdjTriDelVertex());

				m_hullTriList.insert( pTri);
				m_create.push_back( pTri);
				m_horizon[i].m_pAdjTriNew = pTri;
			}

			// 维护三角形的邻接数据
			for ( size_t i=0; i<m_create.size(); i++)
			{
				size_t idxLeft = i>0 ? i-1 : m_create.size()-1;
				size_t idxRight= (i+1) % m_create.size();

				m_create[i]->SetNeighbor( m_horizon[i].m_pAdjBack, m_create[idxLeft], m_create[idxRight]);
				m_horizon[i].m_pAdjBack->ChangeNeighbor( m_horizon[i].m_pAdjFront, m_create[i]);
			}
		}

		// StepC cleans up the process started in stepsA and B by removing
		// all of the previously visible facets (including the corresponding)
		// nodes and edges in the conflict graph
		void CIncrementalMgr::StepC()
		{
			// 清除
			for( size_t i=0; i<m_visible.size(); i++)
			{
				set<CTriangle*>::iterator pos = m_hullTriList.find( m_visible[i]);
				if( pos != m_hullTriList.end())
				{
					delete (*pos);
					m_hullTriList.erase( pos);
				}
			}

			m_create.clear();
			m_horizon.clear();
			m_visible.clear();

			m_iCurIdx++;
		}

		// 获取Horizon Edge
		bool CIncrementalMgr::AddHorizonEdge( vector<SEdge>& horizonEdge, CTriangle* visibleTri)
		{
			int idx = 0;
			SEdge tEdge;

			for ( int i=0; i<3; i++)
			{
				if( !IsVisibleCur( visibleTri->GetNeighbor(i)))
				{
					tEdge.m_pAdjBack  = visibleTri->GetNeighbor(i);
					tEdge.m_pAdjFront = visibleTri;

					// 求邻接边
					for( int j=0; j<3; j++)
					{
						const Vector3* v1 = visibleTri->GetVertex(j);
						for( int k=0; k<3; k++)
						{
							const Vector3* v2 = visibleTri->GetNeighbor(i)->GetVertex(k);
							if( v1 == v2)
							{
								tEdge.m_pVertexs[idx] = v1;
								idx = ( idx+1) % 2;
							}
						}
					}

					horizonEdge.push_back( tEdge);
				}
			}

			return true;
		}

		// 当前是否可见
		bool CIncrementalMgr::IsVisibleCur( CTriangle* pTri)
		{
			for( size_t i=0; i<m_visible.size(); i++)
				if( m_visible[i] == pTri)
					return true;

			return false;
		}

		// 首尾相接排序horizon
		void CIncrementalMgr::SortEdges()
		{
			for ( size_t i=0; i<m_horizon.size()-1; i++)
			{
				size_t idx = FindHorizonEdgeIdx( m_horizon[i].m_pVertexs[1], i);
				if( idx != i+1)
				{
					SEdge tEdge;
					tEdge = m_horizon[idx];
					m_horizon[idx] = m_horizon[i+1];
					m_horizon[i+1] = tEdge;
					//swap( m_horizon[idx], m_horizon[i+1]);
				}
			}
		}

		// 根据顶点查找Edge索引
		size_t CIncrementalMgr::FindHorizonEdgeIdx( const Vector3* pVertex, size_t cur)
		{
			for ( size_t i=cur+1; i<m_horizon.size(); i++)
			{

				if( m_horizon[i].m_pVertexs[0] == pVertex)
					return i;

				if( m_horizon[i].m_pVertexs[1] == pVertex)
				{
					const Vector3* pV;
					pV = m_horizon[i].m_pVertexs[0];
					m_horizon[i].m_pVertexs[0] = m_horizon[i].m_pVertexs[1];
					m_horizon[i].m_pVertexs[1] = pV;

					return i;
				}
			}

			// 未查找到 出错
			assert( false);

			return -1;
		}

		// 根据顶点获取可见三角形
		void CIncrementalMgr::GetVisibleTriangles( const Vector3* pV)
		{
			for ( set<CTriangle*>::iterator iter=m_hullTriList.begin(); iter!=m_hullTriList.end(); iter++)
			{
				if ( (*iter)->IsVisible( pV))
				{
					m_visible.push_back( *iter);
				}
			}
		}
	}

	// 构造函数
	CConvexHull3D::CConvexHull3D()
	{
		m_iNumVertex  = 0;
		m_iNumFace    = 0;
		m_pVertexList = NULL;
		m_pFaceList   = NULL;
	}

	// 析构函数
	CConvexHull3D::~CConvexHull3D()
	{
		delete[] m_pVertexList;
		delete[] m_pFaceList;

		m_pVertexList = NULL;
		m_pFaceList   = NULL;
	}

	// 通过顶点集构建ConvexHull
	bool CConvexHull3D::Build( Vector3* pVertices, size_t iNum, ALGORITM_TYPE a_t/*=EM_RANDOMIZED_INCREMENTAL*/)
	{
#ifdef Ares_Debug		
		if( !pVertices)
			return false;
#endif
		switch( a_t)
		{
		case EM_RANDOMIZED_INCREMENTAL:
			{
				Incremental::CIncrementalMgr algMgr;

				algMgr.BuildByIncrementalAlgorithm( pVertices, iNum);

				// 获取数据
				algMgr.GetConvexHull( m_pVertexList, m_pFaceList, m_iNumVertex, m_iNumFace);
			}
			break;
		}

		return false;
	}
}