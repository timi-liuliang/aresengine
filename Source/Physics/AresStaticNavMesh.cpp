#include <Physics/AresAStar.h>
#include <Physics/Shapes.h>
#include <Physics/DistVector3Triangle3.h>
#include <Physics/AresStaticNavMesh.h>
#include <Physics/AresStaticMeshTiledBuilder.h>

using namespace std;
using namespace boost;

#define DEFAULT_UP_DIR_IDX 2
#define NAV_VERSION	20130527

namespace Ares
{
	NavMeshFilter::NavMeshFilter()
	{
		m_includeFlags = INVALID;

		for( int i=0; i<Max_AreaType; i++)
			m_areaCost[i] = 1.f;
	}

	// 设置消耗
	void NavMeshFilter::SetCost( BYTE areaType, float cost)
	{
		A_ASSERT( areaType>=0 && areaType<Max_AreaType);

		m_areaCost[areaType] = cost;
	}

	// 获取消耗
	float NavMeshFilter::GetCost( BYTE areaType)
	{
		A_ASSERT( areaType>=0 && areaType<Max_AreaType);

		return m_areaCost[areaType];
	}

	// 设置区域是否可通行
	void NavMeshFilter::SetAreaPathable( BYTE areaType, bool value)
	{
		A_ASSERT( areaType>=0 && areaType<Max_AreaType);

		if( value)
			m_includeFlags |= 1<<areaType;
		else
			m_includeFlags &= ~(1<<areaType);
	}

	// 判断区域是否可通行
	bool NavMeshFilter::IsAreaPathable( BYTE areaType) const
	{
		A_ASSERT( areaType>=0 && areaType<Max_AreaType);

		return static_cast<bool>(m_includeFlags & (1<<areaType));
	}

	//------------------------------------
	// NavMeshDetoru 2013-05-28  帝林
	//------------------------------------
	class NavMeshDetour
	{
		// A*接点 2011-01-19  帝林
		struct SANode
		{
			int   m_ID;				// 接点的维一标识
			float m_f, m_g, m_h;	// Fitness, goal, heuristic
			int   m_numChildren;	// 子孙数

			SANode*  m_pParent;		// 父接点
			SANode*  m_pNext;		// 用在Open表和Closed表里(看成链表)
			SANode*  m_pChild[8];

			// 构造函数
			SANode( int id)
			{
				m_ID = id;
				m_numChildren = 0;
				m_pParent   = NULL;
				m_pNext     = NULL;
				memset( m_pChild, 0, sizeof(m_pChild));
			}
		};

		// Stack for propagation
		struct SAStack
		{
			SANode*  m_pData;
			SAStack* m_pNext;
		};

		enum EFindState
		{
			EM_FAILE,		// 不存在从起始点到目的地的路径
			EM_CONTINUE,	// 继续
			EM_OK,			// 查找结束
		};

	public:
		NavMeshDetour();
		~NavMeshDetour();

		// 设置导航图
		void SetNavMap( CStaticNavMesh* pNavMap);

		// 查找路过的多边形
		size_t FindPath( NavMeshFilter& filter, int startRef, int endRef, const Vector3& startPos, const Vector3& endPos, float walkRange, vector<PolyRef>& path);

		// 获取路径
		SANode* GetBestNode() { return m_pBest; }

	private:
		// 步进
		EFindState  Step();

		// 获取f最小结点
		SANode*  GetBest();

		// LinkChild
		void  LinkChild( SANode* pParent, SANode* pTemp);

		// 处理相连接点
		void DealWithChildren( SANode* pNode);

		// 更新父结点
		void  UpdateParents(SANode* pNode);

		// 添加结点到OpenList
		void AddToOpenList( SANode* pNodeAdd);

		// 检测列表
		SANode* CheckList( SANode* pNodeList, int id);

		// 重置
		bool  StepInitialize( int startRef, int endRef, const Vector3& vStart, const Vector3& vEnd);

		// 清除结点
		void ClearNodes();

		// Statck Fuctions  ( Push)
		void  Push( SANode* pNode);

		// 出栈
		SANode* Pop();

	private:
		int				m_iSID;
		int				m_iDID;			// 目的地ID
		Vector3			m_vStart;		// 开始点
		Vector3			m_vEnd;			// 目的地
		float			m_walkRange;	// 可寻路范围
		SANode*			m_pOpen;		// open list
		SANode*			m_pClosed;		// closed list
		SANode*			m_pBest;		// 当前最优结点
		SAStack*		m_pStack;
		CStaticNavMesh*	m_pNavMap;		// 导航图
		NavMeshFilter*	m_filter;		// 区域
	};

	// 构造函数
	NavMeshDetour::NavMeshDetour()
		: m_pNavMap( NULL)
		, m_pOpen( NULL)
		, m_pClosed( NULL)
		, m_pStack( NULL)
		, m_pBest( NULL)
		, m_filter( NULL)
	{
	}

	// 析构函数
	NavMeshDetour::~NavMeshDetour()
	{
		ClearNodes();
	}

	// 设置导航图
	void NavMeshDetour::SetNavMap( CStaticNavMesh* pNavMap)
	{
		m_pNavMap = pNavMap;
	}

	// 查找路过的多边形
	size_t NavMeshDetour::FindPath( NavMeshFilter& filter, int startRef, int endRef, const Vector3& startPos, const Vector3& endPos, float walkRange, vector<PolyRef>& path)
	{
		// 清空
		path.clear();

		m_walkRange = walkRange;
		m_filter	= &filter;

		// 初始化
		if( !StepInitialize( startRef, endRef, startPos, endPos))
			return 0;

		EFindState state = EM_CONTINUE;
		while ( EM_CONTINUE == state)
		{
			state = Step();
		}

		if ( state == EM_FAILE || !m_pBest)
		{
			m_pBest = NULL;
			return 0;
		}

		static vector<PolyRef> tPath;
		SANode* pPath = m_pBest;
		while( pPath)
		{
			tPath.push_back( pPath->m_ID);	

			pPath = pPath->m_pParent;
		}

		// 正序路径顺序
		while( tPath.size())
		{
			path.push_back( tPath.back());

			tPath.pop_back();
		}

		return path.size();
	}

	// 重置
	bool NavMeshDetour::StepInitialize( int startRef, int endRef,const Vector3& vStart, const Vector3& vEnd)
	{
		ClearNodes();

		m_vStart = vStart;
		m_vEnd	 = vEnd;

		if( startRef == -1 || endRef == -1)
			return false;

		m_iSID = startRef;
		m_iDID = endRef;

		SANode* pT = new_ SANode( m_iSID);
		pT->m_g = 0;
		pT->m_h = m_pNavMap->CalcH( *m_filter, m_vStart, m_vEnd, m_iSID, -1);// abs( dx-sx) + abs(dy-sy);
		pT->m_f = pT->m_g + pT->m_h;

		m_pOpen = pT;

		return true;
	}

	// 步进
	NavMeshDetour::EFindState NavMeshDetour::Step()
	{
		// open列表为空,查找失败
		if ( !(m_pBest = GetBest()))
			return EM_FAILE;

		// 等于目的地ID(成功)
		if ( m_pBest->m_ID == m_iDID)
			return EM_OK;

		// 添加周围相连接点
		DealWithChildren( m_pBest);

		return EM_CONTINUE;
	}

	// 获取f最小结点
	NavMeshDetour::SANode* NavMeshDetour::GetBest()
	{
		if ( !m_pOpen) 
			return NULL;

		SANode*  pO = m_pOpen;
		SANode*  pC = m_pClosed;

		// 删除
		m_pOpen = m_pOpen->m_pNext;

		// 添加到close列表
		m_pClosed = pO;
		m_pClosed->m_pNext = pC;

		return pO;
	}

	// 处理相连接点
	void NavMeshDetour::DealWithChildren( SANode* pNode)
	{
		int childID[8];
		int numChild;
		m_pNavMap->GetPassAbleNeighbor( *m_filter, childID, numChild, pNode->m_ID, m_walkRange, m_vStart);

		for ( int i=0; i<numChild; i++)
		{
			SANode pChild(childID[i]);
			LinkChild( pNode, &pChild);
		}
	}

	// LinkChild
	void NavMeshDetour::LinkChild( SANode* pParent, SANode* pTemp)
	{
		int idFromFrom = pParent->m_pParent ? pParent->m_pParent->m_ID : -1;

		float g = pParent->m_g + m_pNavMap->CalcG( *m_filter, m_vStart, pParent->m_ID, pTemp->m_ID, idFromFrom);//UDFunc( m_pUDCost, pParent, pTemp, 0, m_pCBData);

		SANode* pCheck = NULL;

		// 是否在open列表中
		if( pCheck = CheckList( m_pOpen, pTemp->m_ID))
		{
			pParent->m_pChild[ pParent->m_numChildren++] = pCheck;
			if ( g < pCheck->m_g)
			{
				pCheck->m_pParent = pParent;
				pCheck->m_g = g;
				pCheck->m_f = g + pCheck->m_h;
			}
		}
		// 在Cloased表中
		else if ( pCheck = CheckList( m_pClosed, pTemp->m_ID))
		{
			pParent->m_pChild[ pParent->m_numChildren++] = pCheck;
			if ( g < pCheck->m_g)
			{
				pCheck->m_pParent = pParent;
				pCheck->m_g = g;
				pCheck->m_f = g + pCheck->m_h;

				UpdateParents( pCheck);
			}
		}
		// 未考察的结点
		else
		{
			SANode* pNew = new_ SANode( pTemp->m_ID);
			pNew->m_pParent = pParent;
			pNew->m_g = g;
			pNew->m_h = m_pNavMap->CalcH( *m_filter, m_vStart, m_vEnd,pParent->m_ID, pNew->m_ID);
			pNew->m_f = pNew->m_g + pNew->m_h;

			AddToOpenList( pNew);

			pParent->m_pChild[ pParent->m_numChildren++] =  pNew;
		}
	}

	// 更新父结点
	void  NavMeshDetour::UpdateParents(SANode* pNode)
	{
		float g = pNode->m_g;
		int c = pNode->m_numChildren;

		SANode* pKid = NULL;
		for ( int i=0; i<c; i++)
		{
			pKid = pNode->m_pChild[i];
			int idFromFrom = pNode->m_pParent ? pNode->m_pParent->m_ID : -1;
			float fNKg = m_pNavMap->CalcG( *m_filter, m_vStart, pNode->m_ID, pKid->m_ID, idFromFrom);
			if ( g+fNKg < pKid->m_g)
			{
				pKid->m_g = g + fNKg;
				pKid->m_f = pKid->m_g + pKid->m_h;
				pKid->m_pParent = pNode;

				Push( pKid);
			}
		}

		SANode* pParent;
		while( m_pStack)
		{
			pParent = Pop();
			c = pParent->m_numChildren;
			for( int i=0; i<c;i++)
			{
				pKid = pParent->m_pChild[i];
				int idFromFrom = pParent->m_pParent ? pParent->m_pParent->m_ID : -1;
				float fPKg = m_pNavMap->CalcG( *m_filter, m_vStart, pParent->m_ID, pKid->m_ID, idFromFrom);
				if ( pParent->m_g + fPKg< pKid->m_g)
				{
					pKid->m_g = pParent->m_g + fPKg;//UDFunc( m_pUDCost, pParent, pKid, 0, m_pCBData);
					pKid->m_f = pKid->m_g + pKid->m_h;
					pKid->m_pParent = pParent;

					Push( pKid);
				}
			}
		}
	}

	// 添加结点到OpenList
	void NavMeshDetour::AddToOpenList( SANode* pNodeAdd)
	{
		SANode*  pNode = m_pOpen;
		SANode*  pPrev = NULL;

		// 第一个
		if( !m_pOpen)
		{
			m_pOpen = pNodeAdd;
			m_pOpen->m_pNext = NULL;

			return;
		}

		// 根据f 添加到适当位置
		while( pNode)
		{
			if ( pNodeAdd->m_f > pNode->m_f)
			{
				pPrev = pNode;
				pNode = pNode->m_pNext;
			}
			else
			{
				if( pPrev)
				{
					pPrev->m_pNext = pNodeAdd;
					pNodeAdd->m_pNext = pNode;
				}
				else
				{
					pNodeAdd->m_pNext = m_pOpen;
					m_pOpen = pNodeAdd;
				}

				return;
			}
		}

		pPrev->m_pNext = pNodeAdd;
	}

	// Statck Fuctions  ( Push)
	void NavMeshDetour::Push( SANode* pNode)
	{
		if( !m_pStack)
		{
			m_pStack = new_ SAStack;
			m_pStack->m_pData = pNode;
			m_pStack->m_pNext = NULL;
		}
		else
		{
			SAStack* pT = new_ SAStack;
			pT->m_pData = pNode;
			pT->m_pNext = m_pStack;
			m_pStack = pT;
		}
	}

	// 出栈
	NavMeshDetour::SANode* NavMeshDetour::Pop()
	{
		SANode* pData = m_pStack->m_pData;
		SAStack* pT = m_pStack;

		m_pStack = m_pStack->m_pNext;

		if (pT) 
			delete pT;

		return pData;
	}

	// 检测列表
	NavMeshDetour::SANode* NavMeshDetour::CheckList( SANode* pNodeList, int id)
	{
		while( pNodeList)
		{
			if( pNodeList->m_ID == id)
				return pNodeList;

			pNodeList = pNodeList->m_pNext;
		}

		return NULL;
	}

	// 清除结点
	void NavMeshDetour::ClearNodes()
	{
		SANode* pT = NULL;
		while( m_pOpen)
		{
			pT = m_pOpen->m_pNext;

			delete m_pOpen;

			m_pOpen = pT;
		}

		while( m_pClosed)
		{
			pT = m_pClosed->m_pNext;

			delete m_pClosed;

			m_pClosed = pT;
		}
	}

	// 射线与三角形碰撞
	bool Intersect( const Vector3& line_orig,const Vector3& line_dir,const Vector3& tri_v0, const Vector3& tri_v1,const Vector3& tri_v2, float& fDist)
	{
		// Find vectors for two edges sharing vert0
		Vector3 edge1, edge2;
		edge1 = tri_v1 - tri_v0;
		edge2 = tri_v2 - tri_v0;

		// Begin calculating determinant
		Vector3 pVec;
		pVec = Vector3Cross( (Vector3&)line_dir, (Vector3&)edge2);

		// If determinant is near zero, ray lies in plane of triangle
		float det = Vector3Dot( (Vector3&)edge1, pVec);

		Vector3 tVec;
		if ( det > 0)
		{
			tVec = line_orig - tri_v0;
		}
		else
		{
			tVec = tri_v0 - line_orig;
			det  = - det;
		}

		if ( det < 0.0001f)
		{
			return false;
		}

		// 测试边界
		float u = Vector3Dot( tVec, pVec);
		if ( u<0.0f || u>det)
		{
			return false;
		}

		// calculate v parameter and test bounds
		Vector3 qVec;
		qVec = Vector3Cross( tVec, edge1);

		float v = Vector3Dot( line_dir, qVec);
		if ( v<0.0f || u+v>det)
		{
			return false;
		}

		// calculate t, scale parameters, ray intersects triangle
		fDist = Vector3Dot( edge2, qVec);
		float fInvDet = 1.0f / det;
		fDist *= fInvDet;

		return true;
	}

	// 更换顶点数据
	static void SwitchVertex( Vector3* data, int vertexNum, int idx1, int idx2)
	{
		if( idx1 == idx2)
			return;

		for( int i=0; i<vertexNum; i++)
		{
			float tmp     = data[i][idx1];
			data[i][idx1] = data[i][idx2];
			data[i][idx2] = tmp;
		}
	}

	// 更换顶点数据
	static void SwitchVertex( float* data, int vertexNum, int idx1, int idx2)
	{
		if( idx1 == idx2)
			return;

		for( int i=0; i<vertexNum; i++)
		{
			float tmp      = data[i*3+idx1];
			data[i*3+idx1] = data[i*3+idx2];
			data[i*3+idx2] = tmp;
		}
	}

	// 三角形面积
	static inline float TriArea2D( const Vector3& a, const Vector3& b, const Vector3& c)
	{
		return ((b[0]*a[1] - a[0]*b[1]) + (c[0]*b[1] - b[0]*c[1]) + (a[0]*c[1] - c[0]*a[1])) * 0.5f;
	}

	// 构造函数
	CStaticNavMesh::CStaticNavMesh()
	{
		m_pHeader   = NULL;
		m_pData     = NULL;
		m_iDataSize = 0;
		m_upDirIdx  = 2;

		m_quadTree = new_ QuadTree<PolyRef>;
		m_detour   = new_ NavMeshDetour;
		m_detour->SetNavMap( this);
	}

	// 析构函数
	CStaticNavMesh::~CStaticNavMesh()
	{
		Clear();
	}

	// 清除数据
	void CStaticNavMesh::Clear()
	{
		SAFE_DELETE_ARRAY( m_pData);
		SAFE_DELETE( m_quadTree);
		SAFE_DELETE( m_detour);
	}

	// 重置
	void CStaticNavMesh::Reset()
	{
		// 清除数据
		Clear();

		// 初始数据
		m_pHeader   = NULL;
		m_iDataSize = 0;
		m_upDirIdx  = 2;

		m_quadTree = new_ QuadTree<PolyRef>;
		m_detour   = new_ NavMeshDetour;
		m_detour->SetNavMap( this);
	}

	// 设置顶点上方向索引( 0-2)
	void CStaticNavMesh::SetUpDirIdx( int idx/*=2*/)
	{
		if( idx >=0 && idx <= 2)
		{
			m_upDirIdx = idx;
		}
	}

	// 查找路径 
	size_t CStaticNavMesh::FindPath( vector<SPathNode>&	pathResult, NavMeshFilter& filter, const Vector3& startPos, const Vector3& endPos, Vector3 startExtendBox, Vector3 endExtendBox, float inwardOffset/*=0.1f*/, float walkRagne)
	{
		Vector3 tStartPos = startPos;
		Vector3 tEndPos	  = endPos;

		pathResult.clear();

		SwitchVertex( &tStartPos, 1, m_upDirIdx, DEFAULT_UP_DIR_IDX);
		SwitchVertex( &tEndPos,   1, m_upDirIdx, DEFAULT_UP_DIR_IDX);

		PolyRef  iStartRef;
		PolyRef  iEndRef;
		if( FindNearstTwoPoly( iStartRef, iEndRef, startPos, endPos, startExtendBox, filter))
		{
			static vector<PolyRef> tPath;
			size_t tNumPolys = m_detour->FindPath( filter, iStartRef, iEndRef, startPos, endPos, walkRagne+20.f, tPath);
			if( tNumPolys)
			{
				FindStraightPath( startPos, endPos, tPath, pathResult, inwardOffset);
			}
		}

		for( size_t i=0; i<pathResult.size(); i++)
			SwitchVertex( &pathResult[i].m_vPos, 1, m_upDirIdx, DEFAULT_UP_DIR_IDX);

		return pathResult.size();
	}

	// 判断某点是否在寻路范围内
	bool CStaticNavMesh::IsPtOnNavZone( const Vector3& pos, const Vector3& extendBox)
	{
		static vector<PolyRef> polys;
		QueryPolygons( pos, extendBox, polys);
		for( size_t i=0; i<polys.size(); i++)
		{
			Vector3 closestPoint;
			if( CloestPointToPolyInVerticalDirection( polys[i], pos, closestPoint, -extendBox.z, extendBox.z))
				return true;
		}

		return false;
	}

	// 两点是否太近
	static bool IsTooClose( Vector3 v0, Vector3 v1)
	{
		float dx = std::abs(v1[0] - v0[0]);
		float dy = std::abs(v1[1] - v0[1]);
		float dz = std::abs(v1[2] - v0[2]);

		if( dx<0.1f && dy<0.1f && dz<3.f)
			return true;

		return false;
	}

	// 查找具体路径
	int CStaticNavMesh::FindStraightPath( const Vector3& vStart, const Vector3& vEnd, const vector<PolyRef>& path, vector<SPathNode>& straightPath, float inwardOffset/*=0.1f*/)
	{
		if( !m_pHeader)
			return 0;

		if( !path.size())
			return 0;

		// poly上最近开始点
		SPathNode closestStartNode;
		if( !CloestPointToPoly( path[0], vStart, closestStartNode.m_vPos))
			return 0;

		// 添加开始点
		SPathNode tStart;
		tStart.m_vPos = vStart;
			straightPath.push_back( tStart);

		// 添加第一个最近点，如果此点离开始点太近，则不添加
		CloestPointToPolyInVerticalDirection( path[0], vStart, closestStartNode.m_vPos, -1e30f, 1e30f);
		if( !IsTooClose( closestStartNode.m_vPos, vStart))
			straightPath.push_back( closestStartNode);

		// Poly上结束点
		SPathNode closestEndNode;
		if( !CloestPointToPoly( path.back(), vEnd, closestEndNode.m_vPos))
			return 0;

		Vector3 portalApex, portalLeft, portalRight;

		if( path.size() > 1)
		{
			portalApex = closestStartNode.m_vPos;
			portalLeft = portalApex;
			portalRight= portalApex;

			size_t apexIndex = 0;
			size_t leftIndex = 0;
			size_t rightIndex= 0;

			for ( size_t i=0; i<path.size(); i++)
			{
				Vector3 left, right;
				if( i < path.size()-1)
				{
					// get portal points
					GetPortalPoints( path[i], path[i+1], left, right, inwardOffset);
				}
				else
				{
					// End of the path
					left = closestEndNode.m_vPos;
					right= closestEndNode.m_vPos;
				}

				// Right vertex
				if(portalApex == portalRight)
				{
					portalRight = right;
					rightIndex = i;
				}
				else
				{
					// 
					if( TriArea2D( portalApex, portalRight, right) <= 0.f)
					{
						if( TriArea2D( portalApex, portalLeft, right) > 0.f)
						{
							portalRight = right;
							rightIndex = i;
						}
						else
						{
							portalApex = portalLeft;
							apexIndex = leftIndex;

							if( !(straightPath.back().m_vPos == portalApex))
							{
								SPathNode tNode;
								tNode.m_vPos = portalApex;

								straightPath.push_back( tNode);
							}

							portalLeft = portalApex;
							portalRight= portalApex;
							leftIndex = apexIndex;
							rightIndex= apexIndex;

							// Restart
							i = apexIndex;

							continue;
						}
					}
				}

				// Left Vertex
				if( portalApex == portalLeft)
				{
					portalLeft = left;
					leftIndex = i;
				}
				else
				{
					if( TriArea2D( portalApex, portalLeft, left) >= 0.f)
					{
						if( TriArea2D( portalApex, portalRight, left) <= 0.f)
						{
							portalLeft = left;
							leftIndex = i;
						}
						else
						{
							portalApex = portalRight;
							apexIndex = rightIndex;

							if( !( straightPath.back().m_vPos == portalApex))
							{
								SPathNode tNode;
								tNode.m_vPos = portalApex;

								straightPath.push_back( tNode);
							}

							portalLeft = portalApex;
							portalRight= portalApex;
							leftIndex = apexIndex;
							rightIndex= apexIndex;

							// Restart
							i = apexIndex;

							continue;
						}
					}
				}
			}
		}

		// Add end point
		straightPath.push_back( closestEndNode);

		return (int)straightPath.size();
	}

	// 获取poly包围盒
	void CStaticNavMesh::GetPolyBoundingBox( PolyRef iRef, quad_tree_rect& out)
	{
		const SStaticPoly* poly = GetPolyByRef( iRef);
		if( poly)
		{
			for (unsigned int i=0; i<poly->m_iNumVerts; i++)
			{
				const Vector3& pV = m_pHeader->m_pVerts[poly->m_indices[i]];

				out.union_point( pV[0], pV[1]);
			}
		}
	}

	// 获取路标号
	const vector<UINT>& CStaticNavMesh::GetPolyRoadIdsByNavMeshFilter( const NavMeshFilter& filter)
	{
		map<UINT, vector<UINT>>::iterator it = m_polyRoldIDs.find( filter.GetFlags());
		if( it==m_polyRoldIDs.end())
		{
			CStaticMeshTiledBuilder::ConstructRoadIDs( m_polyRoldIDs[filter.GetFlags()], *this, filter);
		}

		return m_polyRoldIDs[filter.GetFlags()];
	}

	// 加载
	bool CStaticNavMesh::Load( const char* fileName)
	{
		// 重置
		Reset();

		if( !fileName && !strlen( fileName))
			return false;

		FILE* fileHandle = fopen( fileName, "rb");
		if( fileHandle)
		{
			// 加载上方向索引
			fread( &m_upDirIdx, sizeof(m_upDirIdx), 1, fileHandle);

			// 保存数据大小
			fread( &m_iDataSize, sizeof( m_iDataSize), 1, fileHandle);

			// 保存数据实体
			m_pData = new_ unsigned char[m_iDataSize];
			fread( m_pData, m_iDataSize, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);

			// 数据指针游标
			unsigned char* dataIndex = m_pData;

			// 解析出header
			const int headerSize	 = sizeof( SStaticNavMeshHeader);
			m_pHeader				 = (SStaticNavMeshHeader*)dataIndex;	 dataIndex += headerSize;

			// 旧版本直接加载失败
			if( m_pHeader->m_iVersion != NAV_VERSION)
			{
				Reset();

				return false;
			}

			const int polyRoadIdSize = sizeof(int) * m_pHeader->m_iNumPolys;
			const int areaIDsSize= sizeof(unsigned char) * m_pHeader->m_iNumPolys;
			const int polySize		 = sizeof(SStaticPoly) * m_pHeader->m_iNumPolys;
			const int vertsSize		 = sizeof(Vector3) * m_pHeader->m_iNumVerts;
			const int vertFlagsSize  = sizeof(BYTE)*m_pHeader->m_iNumVerts;
			const int detailPolySize = sizeof(SStaticPolyDetail) * m_pHeader->m_iNumPolys;
			const int detailVertSize = sizeof(Vector3) * m_pHeader->m_iDNumVerts;
			const int detailTrisSize = sizeof(unsigned char) * 4 * m_pHeader->m_iDNumTris;
			const int quadTreeMemSize= sizeof(quad_tree_member<PolyRef>) * m_pHeader->m_iNumPolys;

			A_ASSERT( m_iDataSize == headerSize + polyRoadIdSize + areaIDsSize + polySize + vertsSize + vertFlagsSize + detailPolySize + detailVertSize + detailTrisSize + quadTreeMemSize);

			//m_pHeader->m_pPolyRoadIDs	 = (UINT*)dataIndex;					 dataIndex += polyRoadIdSize;
			m_pHeader->m_areaIDs		 = (unsigned char*)dataIndex;			 dataIndex += areaIDsSize;
			m_pHeader->m_pPolys			 = (SStaticPoly*)dataIndex;				 dataIndex += polySize;
			m_pHeader->m_pVerts			 = (Vector3*)dataIndex;					 dataIndex += vertsSize;
			m_pHeader->m_pVertFlags		 = (BYTE*)dataIndex;					 dataIndex += vertFlagsSize;
			m_pHeader->m_pDetailPolys	 = (SStaticPolyDetail*)dataIndex;		 dataIndex += detailPolySize;
			m_pHeader->m_pDetailVerts	 = (Vector3*)dataIndex;					 dataIndex += detailVertSize;
			m_pHeader->m_pDetailTris	 = (unsigned char*)dataIndex;			 dataIndex += detailTrisSize;
			m_pHeader->m_pQuadTreeMember = (quad_tree_member<PolyRef>*)dataIndex;dataIndex += quadTreeMemSize;

			// Add to quadtree
			float x0 = m_pHeader->m_vMin.x,
				y0 = m_pHeader->m_vMin.y,
				z0 = m_pHeader->m_vMin.z,
				x1 = m_pHeader->m_vMax.x,
				y1 = m_pHeader->m_vMax.y,
				z1 = m_pHeader->m_vMax.z;

            quad_tree_rect quadTreeArea( x0, x1, y0, y1);
			m_quadTree->create( quadTreeArea, 7);
			for ( UINT i=0; i<m_pHeader->m_iNumPolys; i++)
			{
				m_quadTree->add_member( &m_pHeader->m_pQuadTreeMember[i]);
			}

			return true;
		}

		return false;
	}

	// 保存
	bool CStaticNavMesh::Save( const char* fileName)
	{
		if( !fileName && !strlen( fileName))
			return false;

		FILE* fileHandle = fopen( fileName, "wb");
		if( fileHandle)
		{
			// 保存上方向索引
			fwrite( &m_upDirIdx, sizeof(m_upDirIdx), 1, fileHandle);

			// 保存数据大小
			fwrite( &m_iDataSize, sizeof( m_iDataSize), 1, fileHandle);

			// 保存数据实体
			fwrite( m_pData, m_iDataSize, 1, fileHandle);

			fflush( fileHandle);
			fclose( fileHandle);

			return true;
		}

		return false;
	}

	// 求某点在垂直方向上与某多边形的交点
	bool CStaticNavMesh::CloestPointToPolyInVerticalDirection( int ref, const Vector3& center, Vector3& closetPoint, float minDist, float maxDist)
	{
		Vector3 dir( 0.f, 0.f, 1.f);
		const SStaticPoly* poly            = GetPolyByRef( ref);
		const SStaticPolyDetail* polyDetail= GetPolyDetailByRef( ref);

		for ( int i=0; i<polyDetail->m_iNumTris; i++)
		{
			const unsigned char* t = GetDetailTri( polyDetail->m_iTriBase + i);

			// 三角形的三个顶点
			Vector3 v[3];
			for ( int j=0; j<3; j++)
			{
				if( t[j] < poly->m_iNumVerts)
					v[j] = GetVertex( poly->m_indices[t[j]]);
				else
					v[j] = GetDetailVertex( polyDetail->m_iVertBase + (t[j] - poly->m_iNumVerts));
			}

			float dist = 1e30f;
			if( Intersect( center, dir, v[0], v[1], v[2], dist))
			{
				if( dist>=minDist && dist<=maxDist)
				{
					// 只可能有一个相交点
					closetPoint = center;
					closetPoint[2] += dist;

					return true;
				}
			}
		}

		return false;
	}

	// 求某点到某多边形最近的点
	bool CStaticNavMesh::CloestPointToPoly( int ref, const Vector3& center, Vector3& closetPoint)
	{
		float closestDisSqr                = FLT_BIG;
		const SStaticPoly* poly            = GetPolyByRef( ref);
		const SStaticPolyDetail* polyDetail= GetPolyDetailByRef( ref);

		for ( int i=0; i<polyDetail->m_iNumTris; i++)
		{
			const unsigned char* t = GetDetailTri( polyDetail->m_iTriBase + i);

			// 三角形的三个顶点
			Vector3 v[3];
			for ( int j=0; j<3; j++)
			{
				if( t[j] < poly->m_iNumVerts)
					v[j] = GetVertex( poly->m_indices[t[j]]);
				else
					v[j] = GetDetailVertex( polyDetail->m_iVertBase + (t[j] - poly->m_iNumVerts));
			}

			Triangle3 triangle( v[0], v[1], v[2]);
			DistVector3Triangle3 distVT( center, triangle);
			float d = distVT.GetSquared();
			if( d < closestDisSqr)
			{
				closetPoint   = distVT.m_closePointTriangle;
				closestDisSqr = d;
			}
		}

		return true;
	}

	// Finds the nearest navigation polygon around the center location
	bool  CStaticNavMesh::FindNearstPoly( PolyRef& polyRef, const Vector3& center, const Vector3& extends)
	{
		//A:
		polyRef = -1;

		static vector<PolyRef> polys; polys.clear();
		int nPolys = QueryPolygons( center, extends, polys);

		if( !nPolys)
			return false;

		// Find nearest polygon amongst the nearby polygons
		float nearestDistanceSqr = FLT_MAX;
		for ( int i=0; i<nPolys; i++)
		{
			PolyRef ref = polys[i];

			Vector3 closestPoint;
			if( !CloestPointToPoly( ref, center, closestPoint))
				continue;

			float d = ( center-closestPoint).LengthSq();
			if( d < nearestDistanceSqr)
			{
				nearestDistanceSqr = d;
				polyRef = ref; 
			}
		}

		//goto A;

		return true;
	}

	// 查找最近相联的两个Poly
	bool CStaticNavMesh::FindNearstTwoPoly( PolyRef& polyStart, PolyRef& polyEnd, const Vector3& startCenter, const Vector3& endCenter, const Vector3& extends, NavMeshFilter& filter)
	{
		polyStart = -1;
		polyEnd	  = -1;

		static vector<PolyRef> polyStarts;
		static vector<PolyRef> polyEnds;

		int numStartPolys = QueryPolygons( startCenter, extends, polyStarts);
		int numEndPolys	  = QueryPolygons( endCenter, extends, polyEnds);

		if( !numEndPolys || !numEndPolys)
			return false;

		// start polys
		static multimap<float, PolyRef> tStartPolys;
		static multimap<float, PolyRef> tEndPolys;

		// 清空(static)
		tStartPolys.clear();
		tEndPolys.clear();
		for ( int i=0; i<numStartPolys; i++)
		{
			PolyRef ref = polyStarts[i];
			if( !filter.IsAreaPathable( GetPolyAreaIDByRef(  ref)))
				continue;

			Vector3 closestPoint;
			if( !CloestPointToPoly( ref, startCenter, closestPoint))
				continue;

			float d = (startCenter-closestPoint).LengthSq();

			if( std::abs(startCenter.z - closestPoint.z) < extends.z)
				tStartPolys.insert( make_pair( d, ref));
		}

		// end polys
		for ( int i=0; i<numEndPolys; i++)
		{
			PolyRef ref = polyEnds[i];

			if( !filter.IsAreaPathable( GetPolyAreaIDByRef(  ref)))
				continue;

			Vector3 closestPoint;
			if( !CloestPointToPoly( ref, endCenter, closestPoint))
				continue;

			float d = (endCenter-closestPoint).LengthSq();

			if( std::abs(endCenter.z - closestPoint.z) < extends.z)
				tEndPolys.insert( make_pair( d, ref));;
		}

		if( !tStartPolys.size() || !tEndPolys.size())
			return false;

		// 获取路标号
		const vector<UINT>& roldIDs = GetPolyRoadIdsByNavMeshFilter( filter);

		// 寻找两个PolyRef
		for ( multimap<float, PolyRef>::const_iterator it_start=tStartPolys.begin(); it_start!=tStartPolys.end(); it_start++)
		{
			int roadIDStart = roldIDs[it_start->second];

			for ( multimap<float, PolyRef>::const_iterator it_end=tEndPolys.begin(); it_end!=tEndPolys.end(); it_end++)
			{
				int roadIDEnd = roldIDs[it_end->second];
				if( roadIDStart == roadIDEnd)
				{
					polyStart = it_start->second;
					polyEnd	  = it_end->second;

					return true;
				}
			}
		}

		return false;
	}

	// Returns polygons which touch the query box
	int  CStaticNavMesh::QueryPolygons( const Vector3& center, const Vector3& extents, vector<PolyRef>& polys)
	{
		polys.clear();

		if( !m_pHeader)
			return 0;

		quad_tree_rect searchRect;

		searchRect.x0 = clamp(center[0] - extents[0], m_pHeader->m_vMin.x, m_pHeader->m_vMax.x);
		searchRect.y0 = clamp(center[1] - extents[1], m_pHeader->m_vMin.y, m_pHeader->m_vMax.y);
		searchRect.x1 = clamp(center[0] + extents[0], m_pHeader->m_vMin.x, m_pHeader->m_vMax.x);
		searchRect.y1 = clamp(center[1] + extents[1], m_pHeader->m_vMin.y, m_pHeader->m_vMax.y);

		size_t numPoly = m_quadTree->search( searchRect, polys);

		return (int)polys.size();
	}

	// 获取可通行周围接点
	void CStaticNavMesh::GetPassAbleNeighbor( const NavMeshFilter& filter, int* childID, int& numChild,int fatherID, float walkRange, const Vector3& startPos) const
	{
		numChild = 0;

		SStaticPoly& poly = m_pHeader->m_pPolys[ fatherID];
		for ( int i=0; i<poly.m_iNumVerts; i++)
		{
			if( poly.m_neighbor[i]!=INVALID)
			{
				if( filter.IsAreaPathable( GetPolyAreaIDByRef(poly.m_neighbor[i])))
				{
					if( walkRange>0.f)
					{
						Vector3 middle;
						if( GetEdgeMiddlePoint( fatherID, poly.m_neighbor[i], middle))
						{
							middle = middle - startPos;
							if( abs( middle.x)<walkRange &&  abs( middle.y)<walkRange && abs( middle.z)<walkRange)
							{
								childID[numChild] = poly.m_neighbor[i];
								numChild++;
							}
						}
					}
					else
					{
						childID[numChild] = poly.m_neighbor[i];
						numChild++;
					}
				}
			}
		}
	}

	// 根据ID计算两多边形间h
	float CStaticNavMesh::CalcH(  NavMeshFilter& filter, const Vector3& vStart, const Vector3& vEnd, int idFrom, int idTo/*=-1*/)
	{
		Vector3 vMiddle;

		if( idTo == -1)
			return ( vStart-vEnd).Length() * filter.GetCost( GetPolyAreaIDByRef( idFrom));

		if( GetEdgeMiddlePoint( idFrom, idTo, vMiddle))
			return ( vEnd-vMiddle).Length() * filter.GetCost( GetPolyAreaIDByRef( idTo));

		// 此种情况应当不存在
		return 1e30f;
	}

	// 根据ID计算两多边形间g
	float CStaticNavMesh::CalcG( NavMeshFilter& filter, const Vector3& vStart, int idFrom, int idTo, int idFromFrom/*=-1*/)
	{
		Vector3 p0, p1;

		if( idFromFrom == -1)
		{
			p0 = vStart;
		}
		else
		{
			if( !GetEdgeMiddlePoint( idFromFrom, idFrom, p0))
				return 1e30f;
		}

		if( !GetEdgeMiddlePoint( idFrom, idTo, p1))
			return FLT_BIG;

		return ( p1-p0).Length() *  ( filter.GetCost( GetPolyAreaIDByRef( idFrom)) + filter.GetCost( GetPolyAreaIDByRef( idTo))) * 0.5f;
	}

	// 根据索引获得多边形
	const CStaticNavMesh::SStaticPoly* CStaticNavMesh::GetPolyByRef( PolyRef iRef) const
	{
		if( !m_pHeader || iRef >= m_pHeader->m_iNumPolys)
			return NULL;

		return &m_pHeader->m_pPolys[ iRef];
	}

	// 获取多边形区域ID
	BYTE CStaticNavMesh::GetPolyAreaIDByRef( PolyRef iRef) const
	{
		if( !m_pHeader || iRef >= m_pHeader->m_iNumPolys)
			return RC_NULL_AREA;

		return m_pHeader->m_areaIDs[iRef];
	}

	// 根据索引获得详细多边形
	const CStaticNavMesh::SStaticPolyDetail* CStaticNavMesh::GetPolyDetailByRef( PolyRef iRef)
	{
		if( !m_pHeader || iRef >= m_pHeader->m_iNumPolys)
			return NULL;

		return &m_pHeader->m_pDetailPolys[ iRef];
	}

	// Returns pointer to specifed vertex
	const unsigned char* CStaticNavMesh::GetDetailTri( int i)
	{
		return &m_pHeader->m_pDetailTris[i*4];
	}

	// 获取顶点
	const Vector3& CStaticNavMesh::GetDetailVertex( int i)
	{
		assert( i<m_pHeader->m_iDNumVerts);

		return m_pHeader->m_pDetailVerts[i];
	}

	// 标记
	bool CStaticNavMesh::IsApexVert( PolyRef idx) const
	{
		A_ASSERT( idx<m_pHeader->m_iNumVerts);

		return m_pHeader->m_pVertFlags[idx] & VF_APEX;

		return false;
	}

	// 获取顶点
	const Vector3& CStaticNavMesh::GetVertex( UINT i) const
	{
		assert( i<m_pHeader->m_iNumVerts);

		return m_pHeader->m_pVerts[i];
	}

	// 返回两多边形的共用点
	bool CStaticNavMesh::GetPortalPoints( int idFrom, int idTo, Vector3& vLeft, Vector3& vRight, float inwardOffset/*=0.1f*/) const
	{
		const SStaticPoly* pFromPoly = GetPolyByRef( idFrom);
		if( !pFromPoly)
			return false;

		// Find common edge between the polygons and return the segment and points
		for ( UINT i=0, j=pFromPoly->m_iNumVerts-1; i<pFromPoly->m_iNumVerts; j=i++)
		{
			UINT neighbour = pFromPoly->m_neighbor[j];
			if( neighbour==idTo)
			{
				// 获取相交点
				vLeft  = GetVertex( pFromPoly->m_indices[j]);
				vRight = GetVertex( pFromPoly->m_indices[i]);

				// 向内偏移
				Vector3 dirBias = vRight - vLeft;
				float   dirLenght  = dirBias.Length();
				float	inwardBias = std::min<float>( inwardOffset, dirLenght*0.5f);
				if( IsApexVert( pFromPoly->m_indices[j]))
					vLeft += dirBias * inwardBias / dirLenght;
				
				if( IsApexVert( pFromPoly->m_indices[i]))
					vRight -= dirBias * inwardBias / dirLenght;

				return true;
			}
		}

		return false;
	}

	// 返回两边形边缘线的中心点( returns edge middle point between two polygons)
	bool  CStaticNavMesh::GetEdgeMiddlePoint( int idFrom, int idTo, Vector3& vMiddle) const
	{
		Vector3  vLeft, vRight;

		if( !GetPortalPoints( idFrom, idTo, vLeft, vRight))
			return false;

		// 求中心点
		vMiddle = ( vLeft + vRight) * 0.5f;

		return true;
	}

	// 根据顶点获取多边形邻居
	void CStaticNavMesh::GetPolyNeighborByPoint( PolyRef polyID, UINT pointIdx, PolyRef& leftPoly, PolyRef& rightPoly)
	{
		leftPoly  = 0;
		rightPoly = 0;

		const SStaticPoly* origPoly = GetPolyByRef( polyID);
		if( !origPoly)
			return;

		// 查找邻居
		for ( UINT i=0; i<origPoly->m_iNumVerts; i++)
		{
			if( origPoly->m_indices[i]==pointIdx)
			{
				leftPoly = origPoly->m_neighbor[i];
				rightPoly= origPoly->m_neighbor[i?(i-1):(origPoly->m_iNumVerts-1)];

				break;
			}
		}
	}

	// 计算角平分线(钝角方向)
	bool CStaticNavMesh::CalcAngularBisector( Vector3& result, const Vector3& edge0, const Vector3& edge1)
	{
		result = edge0 + edge1;
		result.Normalize();

		return true;
	}

	// 计算沿角平分线方向偏移长度
	float CStaticNavMesh::CalcAngularBisectorInwardOffset( const Vector3& edgeDir, const Vector3& abDir, float inwardOffset)
	{
		float cosvalue = Vector3Dot( edgeDir, abDir);
		//assert( cosvalue>0 && cosvalue<1);
		if( cosvalue <= 0.f)
			int a = 10;

		float sinvalue = sqrtf( 1-cosvalue*cosvalue);
		if( sinvalue<0.2f)
			sinvalue = 1.f;

		return inwardOffset / sinvalue;
	}

	// 向内修正路径
	void CStaticNavMesh::AdjustPathPointInward( PathResult& oPath, float inwardOffset)
	{
		static PathResult path;

		path = oPath;
		if ( path.size()>2)
		{
			for( size_t i=1; i<path.size()-1; i++)
			{
				// 1计算边
				Vector3 edge0 = path[i].m_vPos - path[i-1].m_vPos; edge0.z = 0.f;
				Vector3 edge1 = path[i].m_vPos - path[i+1].m_vPos; edge1.z = 0.f;
				edge0.Normalize();
				edge1.Normalize();

				// 2.计算角平分线
				Vector3 angularBisector;
				if( CalcAngularBisector( angularBisector, edge0, edge1))
				{
					// 3.根据角平分线计算偏移量
					float abInward0 = CalcAngularBisectorInwardOffset( edge0, angularBisector, inwardOffset);
					float abInward1 = CalcAngularBisectorInwardOffset( edge1, angularBisector, inwardOffset);

					// 4.执行路径点偏移
					oPath[i].m_vPos += std::max<float>( abInward0, abInward1) * angularBisector;
				}
			}
		}
	}
}